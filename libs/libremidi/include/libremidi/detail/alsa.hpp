#pragma once
#include <alsa/asoundlib.h>
#include <atomic>
#include <map>
#include <pthread.h>
#include <libremidi/detail/midi_api.hpp>
#include <libremidi/libremidi.hpp>
#include <sstream>
#include <sys/time.h>
#include <thread>

//*********************************************************************//
//  API: LINUX ALSA SEQUENCER
//*********************************************************************//

// API information found at:
//   - http://www.alsa-project.org/documentation.php#Library

// The ALSA Sequencer API is based on the use of a callback function for
// MIDI input.
//
// Thanks to Pedro Lopez-Cabanillas for help with the ALSA sequencer
// time stamps and other assorted fixes!!!

// If you don't need timestamping for incoming MIDI events, define the
// preprocessor definition AVOID_TIMESTAMPING to save resources
// associated with the ALSA sequencer queues.

namespace libremidi
{
// This function is used to count or get the pinfo structure for a given port
// number.
inline unsigned int
portInfo(snd_seq_t* seq, snd_seq_port_info_t* pinfo, unsigned int type, int portNumber)
{
  snd_seq_client_info_t* cinfo{};
  int client;
  int count = 0;
  snd_seq_client_info_alloca(&cinfo);

  snd_seq_client_info_set_client(cinfo, -1);
  while (snd_seq_query_next_client(seq, cinfo) >= 0)
  {
    client = snd_seq_client_info_get_client(cinfo);
    if (client == 0)
      continue;
    // Reset query info
    snd_seq_port_info_set_client(pinfo, client);
    snd_seq_port_info_set_port(pinfo, -1);
    while (snd_seq_query_next_port(seq, pinfo) >= 0)
    {
      unsigned int atyp = snd_seq_port_info_get_type(pinfo);
      if (((atyp & SND_SEQ_PORT_TYPE_MIDI_GENERIC) == 0) && ((atyp & SND_SEQ_PORT_TYPE_SYNTH) == 0)
          && ((atyp & SND_SEQ_PORT_TYPE_APPLICATION) == 0))
        continue;

      unsigned int caps = snd_seq_port_info_get_capability(pinfo);
      if ((caps & type) != type)
        continue;
      if (count == portNumber)
        return 1;
      ++count;
    }
  }

  // If a negative portNumber was used, return the port count.
  if (portNumber < 0)
    return count;
  return 0;
}

// A structure to hold variables related to the ALSA API
// implementation.
struct alsa_data
{
  snd_seq_t* seq{};
  int vport{};
  snd_seq_port_subscribe_t* subscription{};
  snd_midi_event_t* coder{};
  unsigned int bufferSize{};
  pthread_t thread{};
  pthread_t dummy_thread_id{};
  snd_seq_real_time_t lastTime{};
  int queue_id{}; // an input queue is needed to get timestamped events
  int trigger_fds[2]{};
  std::vector<unsigned char> buffer;
};

class observer_alsa final : public observer_api
{
public:
  observer_alsa(observer::callbacks&& c) : observer_api{std::move(c)}
  {
    using namespace std::literals;
    int err = snd_seq_open(&seq_, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK);
    if (err < 0)
    {
      throw driver_error("observer_alsa: snd_seq_open failed");
    }

    client_ = snd_seq_client_id(seq_);

    auto N = snd_seq_poll_descriptors_count(seq_, POLLIN);
    descriptors_.resize(N);
    snd_seq_poll_descriptors(seq_, descriptors_.data(), N, POLLIN);

    err = snd_seq_set_client_name(seq_, "libremidi-observe");
    if (err < 0)
    {
      throw driver_error("observer_alsa: snd_seq_set_client_name failed");
    }

    err = snd_seq_create_simple_port(
        seq_, "libremidi-observe",
        SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_READ
            | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION);
    if (err < 0)
    {
      throw driver_error("observer_alsa: snd_seq_create_simple_port failed");
    }
    port_ = err;

    err = snd_seq_connect_from(seq_, port_, SND_SEQ_CLIENT_SYSTEM, SND_SEQ_PORT_SYSTEM_ANNOUNCE);
    if (err < 0)
    {
      throw driver_error("observer_alsa: snd_seq_connect_from failed");
    }

    running = true;
    poll_ = std::thread{[this] {
      while (this->running)
      {
        int err = poll(descriptors_.data(), descriptors_.size(), -1);
        if (err > 0)
        {
          snd_seq_event_t* ev;
          while (snd_seq_event_input(seq_, &ev) >= 0)
          {
            handle_event(ev);
          }
        }
        std::this_thread::sleep_for(1s);
      }
    }};
  }

  struct port_info
  {
    std::string name;
    int client{};
    int port{};
    bool isInput{};
    bool isOutput{};
  };

  port_info get_info(int client, int port)
  {
    port_info p;
    p.client = client;
    p.port = port;

    snd_seq_client_info_t* cinfo;
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_get_any_client_info(seq_, client, cinfo);

    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_get_any_port_info(seq_, client, port, pinfo);

    p.name = std::to_string(p.client) + ":" + std::to_string(p.port);

    if (auto name = snd_seq_client_info_get_name(cinfo))
      p.name += std::string(" ") + name;
    p.name += " - ";
    if (auto name = snd_seq_port_info_get_name(pinfo))
      p.name += std::string(" ") + name;

    auto cap = snd_seq_port_info_get_capability(pinfo);
    p.isInput = (cap & SND_SEQ_PORT_CAP_DUPLEX) | (cap & SND_SEQ_PORT_CAP_READ);
    p.isOutput = (cap & SND_SEQ_PORT_CAP_DUPLEX) | (cap & SND_SEQ_PORT_CAP_WRITE);

    return p;
  }

  void handle_event(snd_seq_event_t* ev)
  {
    switch (ev->type)
    {
      case SND_SEQ_EVENT_PORT_START:
      {
        auto p = get_info(ev->data.addr.client, ev->data.addr.port);
        if (p.client == client_)
          return;

        knownClients_[{p.client, p.port}] = p;
        if (p.isInput && callbacks_.input_added)
        {
          callbacks_.input_added(p.port, p.name);
        }

        if (p.isOutput && callbacks_.output_added)
        {
          callbacks_.output_added(p.port, p.name);
        }
        break;
      }
      case SND_SEQ_EVENT_PORT_EXIT:
      {
        auto p = get_info(ev->data.addr.client, ev->data.addr.port);
        if (p.client == client_)
          return;

        auto it = knownClients_.find({p.client, p.port});
        if (it != knownClients_.end())
        {
          p = it->second;
          knownClients_.erase(it);
        }

        if (p.isInput && callbacks_.input_removed)
        {
          callbacks_.input_removed(p.port, p.name);
        }

        if (p.isOutput && callbacks_.output_added)
        {
          callbacks_.output_removed(p.port, p.name);
        }
        break;
      }
      case SND_SEQ_EVENT_PORT_CHANGE:
      {
        break;
      }
      default:
        break;
    }
  }

  ~observer_alsa()
  {
    running = false;
    assert(poll_.joinable());
    poll_.join();

    snd_seq_delete_port(seq_, port_);
    snd_seq_close(seq_);
  }

private:
  snd_seq_t* seq_{};
  std::atomic_bool running{false};
  std::thread poll_;
  std::vector<pollfd> descriptors_;
  std::map<std::pair<int, int>, port_info> knownClients_;
  int client_{};
  int port_{};
};

class midi_in_alsa final : public midi_in_api
{
public:
  midi_in_alsa(std::string_view clientName, unsigned int queueSizeLimit)
      : midi_in_api{&data, queueSizeLimit}
  {
    // Set up the ALSA sequencer client.
    snd_seq_t* seq;
    int result = snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, SND_SEQ_NONBLOCK);
    if (result < 0)
    {
      error<driver_error>(
          "MidiInAlsa::initialize: error creating ALSA sequencer client "
          "object.");
      return;
    }

    // Set client name.
    snd_seq_set_client_name(seq, clientName.data());

    // Save our api-specific connection information.
    data.seq = seq;
    data.vport = -1;
    data.subscription = nullptr;
    data.dummy_thread_id = pthread_self();
    data.thread = data.dummy_thread_id;
    data.trigger_fds[0] = -1;
    data.trigger_fds[1] = -1;

    if (pipe(data.trigger_fds) == -1)
    {
      error<driver_error>("MidiInAlsa::initialize: error creating pipe objects.");
      return;
    }

    // Create the input queue
#ifndef AVOID_TIMESTAMPING
    data.queue_id = snd_seq_alloc_named_queue(seq, "RtMidi Queue");
    // Set arbitrary tempo (mm=100) and resolution (240)
    snd_seq_queue_tempo_t* qtempo;
    snd_seq_queue_tempo_alloca(&qtempo);
    snd_seq_queue_tempo_set_tempo(qtempo, 600000);
    snd_seq_queue_tempo_set_ppq(qtempo, 240);
    snd_seq_set_queue_tempo(data.seq, data.queue_id, qtempo);
    snd_seq_drain_output(data.seq);
#endif
  }

  ~midi_in_alsa() override
  {
    // Close a connection if it exists.
    midi_in_alsa::close_port();

    // Shutdown the input thread.
    if (inputData_.doInput)
    {
      inputData_.doInput = false;
      write(data.trigger_fds[1], &inputData_.doInput, sizeof(inputData_.doInput));

      if (!pthread_equal(data.thread, data.dummy_thread_id))
        pthread_join(data.thread, nullptr);
    }

    // Cleanup.
    close(data.trigger_fds[0]);
    close(data.trigger_fds[1]);
    if (data.vport >= 0)
      snd_seq_delete_port(data.seq, data.vport);
#ifndef AVOID_TIMESTAMPING
    snd_seq_free_queue(data.seq, data.queue_id);
#endif
    snd_seq_close(data.seq);
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::LINUX_ALSA;
  }

  void open_port(unsigned int portNumber, std::string_view portName) override
  {
    if (connected_)
    {
      warning("MidiInAlsa::openPort: a valid connection already exists!");
      return;
    }

    unsigned int nSrc = this->get_port_count();
    if (nSrc < 1)
    {
      error<no_devices_found_error>("MidiInAlsa::openPort: no MIDI input sources found!");
      return;
    }

    snd_seq_port_info_t* src_pinfo;
    snd_seq_port_info_alloca(&src_pinfo);
    if (portInfo(
            data.seq, src_pinfo, SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
            (int)portNumber)
        == 0)
    {
      std::ostringstream ost;
      ost << "MidiInAlsa::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
      error<invalid_parameter_error>(ost.str());
      return;
    }

    snd_seq_addr_t sender, receiver;
    sender.client = snd_seq_port_info_get_client(src_pinfo);
    sender.port = snd_seq_port_info_get_port(src_pinfo);
    receiver.client = snd_seq_client_id(data.seq);

    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_alloca(&pinfo);
    if (data.vport < 0)
    {
      snd_seq_port_info_set_client(pinfo, 0);
      snd_seq_port_info_set_port(pinfo, 0);
      snd_seq_port_info_set_capability(
          pinfo, SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE);
      snd_seq_port_info_set_type(
          pinfo, SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);
      snd_seq_port_info_set_midi_channels(pinfo, 16);
#ifndef AVOID_TIMESTAMPING
      snd_seq_port_info_set_timestamping(pinfo, 1);
      snd_seq_port_info_set_timestamp_real(pinfo, 1);
      snd_seq_port_info_set_timestamp_queue(pinfo, data.queue_id);
#endif
      snd_seq_port_info_set_name(pinfo, portName.data());
      data.vport = snd_seq_create_port(data.seq, pinfo);

      if (data.vport < 0)
      {
        error<driver_error>("MidiInAlsa::openPort: ALSA error creating input port.");
        return;
      }
      data.vport = snd_seq_port_info_get_port(pinfo);
    }

    receiver.port = data.vport;

    if (!data.subscription)
    {
      // Make subscription
      if (snd_seq_port_subscribe_malloc(&data.subscription) < 0)
      {
        error<driver_error>("MidiInAlsa::openPort: ALSA error allocation port subscription.");
        return;
      }
      snd_seq_port_subscribe_set_sender(data.subscription, &sender);
      snd_seq_port_subscribe_set_dest(data.subscription, &receiver);
      if (snd_seq_subscribe_port(data.seq, data.subscription))
      {
        snd_seq_port_subscribe_free(data.subscription);
        data.subscription = nullptr;
        error<driver_error>("MidiInAlsa::openPort: ALSA error making port connection.");
        return;
      }
    }

    if (inputData_.doInput == false)
    {
      // Start the input queue
#ifndef AVOID_TIMESTAMPING
      snd_seq_start_queue(data.seq, data.queue_id, nullptr);
      snd_seq_drain_output(data.seq);
#endif
      // Start our MIDI input thread.
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
      pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

      inputData_.doInput = true;
      int err = pthread_create(&data.thread, &attr, alsaMidiHandler, &inputData_);
      pthread_attr_destroy(&attr);
      if (err)
      {
        snd_seq_unsubscribe_port(data.seq, data.subscription);
        snd_seq_port_subscribe_free(data.subscription);
        data.subscription = nullptr;
        inputData_.doInput = false;
        error<thread_error>("MidiInAlsa::openPort: error starting MIDI input thread!");
        return;
      }
    }

    connected_ = true;
  }
  void open_virtual_port(std::string_view portName) override
  {
    if (data.vport < 0)
    {
      snd_seq_port_info_t* pinfo;
      snd_seq_port_info_alloca(&pinfo);
      snd_seq_port_info_set_capability(
          pinfo, SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE);
      snd_seq_port_info_set_type(
          pinfo, SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);
      snd_seq_port_info_set_midi_channels(pinfo, 16);
#ifndef AVOID_TIMESTAMPING
      snd_seq_port_info_set_timestamping(pinfo, 1);
      snd_seq_port_info_set_timestamp_real(pinfo, 1);
      snd_seq_port_info_set_timestamp_queue(pinfo, data.queue_id);
#endif
      snd_seq_port_info_set_name(pinfo, portName.data());
      data.vport = snd_seq_create_port(data.seq, pinfo);

      if (data.vport < 0)
      {
        error<driver_error>("MidiInAlsa::openVirtualPort: ALSA error creating virtual port.");
        return;
      }
      data.vport = snd_seq_port_info_get_port(pinfo);
    }

    if (inputData_.doInput == false)
    {
      // Wait for old thread to stop, if still running
      if (!pthread_equal(data.thread, data.dummy_thread_id))
        pthread_join(data.thread, nullptr);

        // Start the input queue
#ifndef AVOID_TIMESTAMPING
      snd_seq_start_queue(data.seq, data.queue_id, nullptr);
      snd_seq_drain_output(data.seq);
#endif
      // Start our MIDI input thread.
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
      pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

      inputData_.doInput = true;
      int err = pthread_create(&data.thread, &attr, alsaMidiHandler, &inputData_);
      pthread_attr_destroy(&attr);
      if (err)
      {
        if (data.subscription)
        {
          snd_seq_unsubscribe_port(data.seq, data.subscription);
          snd_seq_port_subscribe_free(data.subscription);
          data.subscription = nullptr;
        }
        inputData_.doInput = false;
        error<thread_error>("MidiInAlsa::openPort: error starting MIDI input thread!");
        return;
      }
    }
  }
  void close_port() override
  {
    if (connected_)
    {
      if (data.subscription)
      {
        snd_seq_unsubscribe_port(data.seq, data.subscription);
        snd_seq_port_subscribe_free(data.subscription);
        data.subscription = nullptr;
      }
      // Stop the input queue
#ifndef AVOID_TIMESTAMPING
      snd_seq_stop_queue(data.seq, data.queue_id, nullptr);
      snd_seq_drain_output(data.seq);
#endif
      connected_ = false;
    }

    // Stop thread to avoid triggering the callback, while the port is intended
    // to be closed
    if (inputData_.doInput)
    {
      inputData_.doInput = false;
      write(data.trigger_fds[1], &inputData_.doInput, sizeof(inputData_.doInput));

      if (!pthread_equal(data.thread, data.dummy_thread_id))
        pthread_join(data.thread, nullptr);
    }
  }
  void set_client_name(std::string_view clientName) override
  {
    snd_seq_set_client_name(data.seq, clientName.data());
  }
  void set_port_name(std::string_view portName) override
  {
    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_get_port_info(data.seq, data.vport, pinfo);
    snd_seq_port_info_set_name(pinfo, portName.data());
    snd_seq_set_port_info(data.seq, data.vport, pinfo);
  }
  unsigned int get_port_count() override
  {
    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_alloca(&pinfo);

    return portInfo(data.seq, pinfo, SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ, -1);
  }
  std::string get_port_name(unsigned int portNumber) override
  {
    snd_seq_client_info_t* cinfo;
    snd_seq_port_info_t* pinfo;
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);

    std::string stringName;
    if (portInfo(
            data.seq, pinfo, SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ, (int)portNumber))
    {
      int cnum = snd_seq_port_info_get_client(pinfo);
      snd_seq_get_any_client_info(data.seq, cnum, cinfo);
      std::ostringstream os;
      os << snd_seq_client_info_get_name(cinfo);
      os << ":";
      os << snd_seq_port_info_get_name(pinfo);
      os << " "; // These lines added to make sure devices are listed
      os << snd_seq_port_info_get_client(pinfo); // with full portnames added to ensure individual
                                                 // device names
      os << ":";
      os << snd_seq_port_info_get_port(pinfo);
      stringName = os.str();
      return stringName;
    }

    // If we get here, we didn't find a match.
    warning("MidiInAlsa::getPortName: error looking for port name!");
    return stringName;
  }

private:
  static void* alsaMidiHandler(void* ptr)
  {
    auto& data = *static_cast<midi_in_api::in_data*>(ptr);
    auto& apidata = *static_cast<alsa_data*>(data.apiData);

    double time{};
    bool continueSysex = false;
    bool doDecode = false;
    message message{};
    int poll_fd_count{};
    pollfd* poll_fds{};

    snd_seq_event_t* ev;

    apidata.bufferSize = 32;
    int result = snd_midi_event_new(0, &apidata.coder);
    if (result < 0)
    {
      data.doInput = false;
      std::cerr << "\nMidiInAlsa::alsaMidiHandler: error initializing MIDI "
                   "event parser!\n\n";
      return nullptr;
    }

    auto& buffer = apidata.buffer;
    buffer.clear();
    buffer.resize(apidata.bufferSize);

    snd_midi_event_init(apidata.coder);
    snd_midi_event_no_status(apidata.coder, 1); // suppress running status messages

    poll_fd_count = snd_seq_poll_descriptors_count(apidata.seq, POLLIN) + 1;
    poll_fds = (struct pollfd*)alloca(poll_fd_count * sizeof(struct pollfd));
    snd_seq_poll_descriptors(apidata.seq, poll_fds + 1, poll_fd_count - 1, POLLIN);
    poll_fds[0].fd = apidata.trigger_fds[0];
    poll_fds[0].events = POLLIN;

    while (data.doInput)
    {
      if (snd_seq_event_input_pending(apidata.seq, 1) == 0)
      {
        // No data pending
        if (poll(poll_fds, poll_fd_count, -1) >= 0)
        {
          if (poll_fds[0].revents & POLLIN)
          {
            bool dummy;
            read(poll_fds[0].fd, &dummy, sizeof(dummy));
          }
        }
        continue;
      }

      // If here, there should be data.
      result = snd_seq_event_input(apidata.seq, &ev);
      if (result == -ENOSPC)
      {
        std::cerr << "\nMidiInAlsa::alsaMidiHandler: MIDI input buffer overrun!\n\n";
        continue;
      }
      else if (result <= 0)
      {
        std::cerr << "\nMidiInAlsa::alsaMidiHandler: unknown MIDI input error!\n";
        perror("System reports");
        continue;
      }

      // This is a bit weird, but we now have to decode an ALSA MIDI
      // event (back) into MIDI bytes.  We'll ignore non-MIDI types.
      if (!continueSysex)
        message.bytes.clear();

      doDecode = false;
      switch (ev->type)
      {

        case SND_SEQ_EVENT_PORT_SUBSCRIBED:
#if defined(__LIBREMIDI_DEBUG__)
          std::cout << "MidiInAlsa::alsaMidiHandler: port connection made!\n";
#endif
          break;

        case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
#if defined(__LIBREMIDI_DEBUG__)
          std::cerr << "MidiInAlsa::alsaMidiHandler: port connection has closed!\n";
          std::cout << "sender = " << (int)ev->data.connect.sender.client << ":"
                    << (int)ev->data.connect.sender.port
                    << ", dest = " << (int)ev->data.connect.dest.client << ":"
                    << (int)ev->data.connect.dest.port << std::endl;
#endif
          break;

        case SND_SEQ_EVENT_QFRAME: // MIDI time code
          if (!(data.ignoreFlags & 0x02))
            doDecode = true;
          break;

        case SND_SEQ_EVENT_TICK: // 0xF9 ... MIDI timing tick
          if (!(data.ignoreFlags & 0x02))
            doDecode = true;
          break;

        case SND_SEQ_EVENT_CLOCK: // 0xF8 ... MIDI timing (clock) tick
          if (!(data.ignoreFlags & 0x02))
            doDecode = true;
          break;

        case SND_SEQ_EVENT_SENSING: // Active sensing
          if (!(data.ignoreFlags & 0x04))
            doDecode = true;
          break;

        case SND_SEQ_EVENT_SYSEX:
        {
          if ((data.ignoreFlags & 0x01))
            break;
          if (ev->data.ext.len > apidata.bufferSize)
          {
            apidata.bufferSize = ev->data.ext.len;
            buffer.resize(apidata.bufferSize);
          }
          doDecode = true;
          break;
        }

        default:
          doDecode = true;
      }

      if (doDecode)
      {
        uint64_t nBytes
            = snd_midi_event_decode(apidata.coder, buffer.data(), apidata.bufferSize, ev);
        if (nBytes > 0)
        {
          // The ALSA sequencer has a maximum buffer size for MIDI sysex
          // events of 256 bytes.  If a device sends sysex messages larger
          // than this, they are segmented into 256 byte chunks.  So,
          // we'll watch for this and concatenate sysex chunks into a
          // single sysex message if necessary.
          assert(nBytes < buffer.size());
          if (!continueSysex)
            message.bytes.assign(buffer.data(), buffer.data() + nBytes);
          else
            message.bytes.insert(message.bytes.end(), buffer.data(), buffer.data() + nBytes);

          continueSysex = ((ev->type == SND_SEQ_EVENT_SYSEX) && (message.bytes.back() != 0xF7));
          if (!continueSysex)
          {

            // Calculate the time stamp:
            message.timestamp = 0.0;

            // Method 1: Use the system time.
            // gettimeofday(&tv, (struct timezone *)nullptr);
            // time = (tv.tv_sec * 1000000) + tv.tv_usec;

            // Method 2: Use the ALSA sequencer event time data.
            // (thanks to Pedro Lopez-Cabanillas!).

            // Using method from:
            // https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html

            // Perform the carry for the later subtraction by updating y.
            snd_seq_real_time_t& x(ev->time.time);
            snd_seq_real_time_t& y(apidata.lastTime);
            if (x.tv_nsec < y.tv_nsec)
            {
              int nsec = (y.tv_nsec - x.tv_nsec) / 1000000000 + 1;
              y.tv_nsec -= 1000000000 * nsec;
              y.tv_sec += nsec;
            }
            if (x.tv_nsec - y.tv_nsec > 1000000000)
            {
              int nsec = (x.tv_nsec - y.tv_nsec) / 1000000000;
              y.tv_nsec += 1000000000 * nsec;
              y.tv_sec -= nsec;
            }

            // Compute the time difference.
            time = x.tv_sec - y.tv_sec + (x.tv_nsec - y.tv_nsec) * 1e-9;

            apidata.lastTime = ev->time.time;

            if (data.firstMessage == true)
              data.firstMessage = false;
            else
              message.timestamp = time;
          }
          else
          {
#if defined(__LIBREMIDI_DEBUG__)
            std::cerr << "\nMidiInAlsa::alsaMidiHandler: event parsing error or "
                         "not a MIDI event!\n\n";
#endif
          }
        }
      }

      snd_seq_free_event(ev);
      if (message.bytes.size() == 0 || continueSysex)
        continue;

      data.on_message_received(std::move(message));
    }

    snd_midi_event_free(apidata.coder);
    apidata.coder = nullptr;
    apidata.thread = apidata.dummy_thread_id;
    return nullptr;
  }
  alsa_data data;
};

class midi_out_alsa final : public midi_out_api
{
public:
  midi_out_alsa(std::string_view clientName)
  {
    // Set up the ALSA sequencer client.
    snd_seq_t* seq{};
    int result1 = snd_seq_open(&seq, "default", SND_SEQ_OPEN_OUTPUT, SND_SEQ_NONBLOCK);
    if (result1 < 0)
    {
      error<driver_error>(
          "MidiOutAlsa::initialize: error creating ALSA sequencer client "
          "object.");
      return;
    }

    // Set client name.
    snd_seq_set_client_name(seq, clientName.data());

    // Save our api-specific connection information.
    data.seq = seq;
    data.vport = -1;
    data.bufferSize = 32;
    data.coder = nullptr;
    int result = snd_midi_event_new(data.bufferSize, &data.coder);
    if (result < 0)
    {
      error<driver_error>(
          "MidiOutAlsa::initialize: error initializing MIDI event "
          "parser!\n\n");
      return;
    }
    snd_midi_event_init(data.coder);
  }

  ~midi_out_alsa() override
  {
    // Close a connection if it exists.
    midi_out_alsa::close_port();

    // Cleanup.
    if (data.vport >= 0)
      snd_seq_delete_port(data.seq, data.vport);
    if (data.coder)
      snd_midi_event_free(data.coder);
    snd_seq_close(data.seq);
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::LINUX_ALSA;
  }

  void open_port(unsigned int portNumber, std::string_view portName) override
  {
    if (connected_)
    {
      warning("MidiOutAlsa::openPort: a valid connection already exists!");
      return;
    }

    unsigned int nSrc = this->get_port_count();
    if (nSrc < 1)
    {
      error<no_devices_found_error>("MidiOutAlsa::openPort: no MIDI output sources found!");
      return;
    }

    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_alloca(&pinfo);
    if (portInfo(
            data.seq, pinfo, SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE, (int)portNumber)
        == 0)
    {
      std::ostringstream ost;
      ost << "MidiOutAlsa::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
      error<invalid_parameter_error>(ost.str());
      return;
    }

    snd_seq_addr_t sender{}, receiver{};
    receiver.client = snd_seq_port_info_get_client(pinfo);
    receiver.port = snd_seq_port_info_get_port(pinfo);
    sender.client = snd_seq_client_id(data.seq);

    if (data.vport < 0)
    {
      data.vport = snd_seq_create_simple_port(
          data.seq, portName.data(), SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
          SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);
      if (data.vport < 0)
      {
        error<driver_error>("MidiOutAlsa::openPort: ALSA error creating output port.");
        return;
      }
    }

    sender.port = data.vport;

    // Make subscription
    if (snd_seq_port_subscribe_malloc(&data.subscription) < 0)
    {
      snd_seq_port_subscribe_free(data.subscription);
      error<driver_error>("MidiOutAlsa::openPort: error allocating port subscription.");
      return;
    }
    snd_seq_port_subscribe_set_sender(data.subscription, &sender);
    snd_seq_port_subscribe_set_dest(data.subscription, &receiver);
    snd_seq_port_subscribe_set_time_update(data.subscription, 1);
    snd_seq_port_subscribe_set_time_real(data.subscription, 1);
    if (snd_seq_subscribe_port(data.seq, data.subscription))
    {
      snd_seq_port_subscribe_free(data.subscription);
      error<driver_error>("MidiOutAlsa::openPort: ALSA error making port connection.");
      return;
    }

    connected_ = true;
  }

  void open_virtual_port(std::string_view portName) override
  {
    if (data.vport < 0)
    {
      data.vport = snd_seq_create_simple_port(
          data.seq, portName.data(), SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
          SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);

      if (data.vport < 0)
      {
        error<driver_error>("MidiOutAlsa::openVirtualPort: ALSA error creating virtual port.");
      }
    }
  }

  void close_port() override
  {
    if (connected_)
    {
      snd_seq_unsubscribe_port(data.seq, data.subscription);
      snd_seq_port_subscribe_free(data.subscription);
      data.subscription = nullptr;
      connected_ = false;
    }
  }

  void set_client_name(std::string_view clientName) override
  {
    snd_seq_set_client_name(data.seq, clientName.data());
  }

  void set_port_name(std::string_view portName) override
  {
    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_get_port_info(data.seq, data.vport, pinfo);
    snd_seq_port_info_set_name(pinfo, portName.data());
    snd_seq_set_port_info(data.seq, data.vport, pinfo);
  }

  unsigned int get_port_count() override
  {
    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_alloca(&pinfo);
    return portInfo(data.seq, pinfo, SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE, -1);
  }

  std::string get_port_name(unsigned int portNumber) override
  {
    snd_seq_client_info_t* cinfo;
    snd_seq_port_info_t* pinfo;
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);

    std::string stringName;
    if (portInfo(
            data.seq, pinfo, SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
            (int)portNumber))
    {
      int cnum = snd_seq_port_info_get_client(pinfo);
      snd_seq_get_any_client_info(data.seq, cnum, cinfo);
      std::ostringstream os;
      os << snd_seq_client_info_get_name(cinfo);
      os << ":";
      os << snd_seq_port_info_get_name(pinfo);
      os << " "; // These lines added to make sure devices are listed
      os << snd_seq_port_info_get_client(pinfo); // with full portnames added to ensure individual
                                                 // device names
      os << ":";
      os << snd_seq_port_info_get_port(pinfo);
      stringName = os.str();
      return stringName;
    }

    // If we get here, we didn't find a match.
    warning("MidiOutAlsa::getPortName: error looking for port name!");
    return stringName;
  }

  void send_message(const unsigned char* message, size_t size) override
  {
    int64_t result{};
    unsigned int nBytes = static_cast<unsigned int>(size);
    if (nBytes > data.bufferSize)
    {
      data.bufferSize = nBytes;
      result = snd_midi_event_resize_buffer(data.coder, nBytes);
      if (result != 0)
      {
        error<driver_error>(
            "MidiOutAlsa::sendMessage: ALSA error resizing MIDI event "
            "buffer.");
        return;
      }
    }

    snd_seq_event_t ev;
    snd_seq_ev_clear(&ev);
    snd_seq_ev_set_source(&ev, data.vport);
    snd_seq_ev_set_subs(&ev);
    snd_seq_ev_set_direct(&ev);

    result = snd_midi_event_encode(data.coder, message, nBytes, &ev);
    if (result < nBytes)
    {
      warning("MidiOutAlsa::sendMessage: event parsing error!");
      return;
    }

    // Send the event.
    result = snd_seq_event_output(data.seq, &ev);
    if (result < 0)
    {
      warning("MidiOutAlsa::sendMessage: error sending MIDI message to port.");
      return;
    }
    snd_seq_drain_output(data.seq);
  }

private:
  alsa_data data;
};

struct alsa_backend
{
  using midi_in = midi_in_alsa;
  using midi_out = midi_out_alsa;
  using midi_observer = observer_alsa;
  static const constexpr auto API = libremidi::API::LINUX_ALSA;
};
}
