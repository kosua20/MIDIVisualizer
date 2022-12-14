#pragma once
#include <alsa/asoundlib.h>
#include <ostream>
#include <libremidi/detail/dummy.hpp>
#include <libremidi/detail/midi_api.hpp>
#include <libremidi/detail/raw_alsa_helpers.hpp>
#include <libremidi/libremidi.hpp>
#include <thread>
#include <atomic>
#include <sstream>

// Credits: greatly inspired from
// https://ccrma.stanford.edu/~craig/articles/linuxmidi/alsa-1.0/alsarawmidiout.c
// https://ccrma.stanford.edu/~craig/articles/linuxmidi/alsa-1.0/alsarawportlist.c
// Thanks Craig Stuart Sapp <craig@ccrma.stanford.edu>

namespace libremidi
{
struct midi_stream_decoder
{
  midi_in_api::in_data& data;
  midi_bytes bytes;
  message msg;

  midi_stream_decoder(midi_in_api::in_data& data)
    : data{data}
  {
    bytes.reserve(16);
  }

  void add_bytes(unsigned char* data, std::size_t sz)
  {
    for(std::size_t i = 0; i < sz; i++)
      bytes.push_back(data[i]);

    int read = 0;
    unsigned char* begin = bytes.data();
    unsigned char* end = bytes.data() + bytes.size();
    while((read = parse(begin, end)) && read > 0)
    {
      begin += read;
      this->data.on_message_received(std::move(msg));
    }

    // Remove the read bytes
    if(begin != bytes.data())
      bytes.erase(bytes.begin(), bytes.begin() + (begin - bytes.data()));
  }

  uint8_t runningStatusType_{};
  int parse(unsigned char* bytes, unsigned char* end)
  {
    int sz = end - bytes;
    if (sz == 0)
      return 0;

    msg.bytes.clear();

    if (((uint8_t)bytes[0] & 0xF) == 0xF)
    {
      // TODO special message
      return sz;
    }
    else if(((uint8_t)bytes[0] & 0xF8 ) == 0xF8)
    {
      // Clk messages
      msg.bytes.reserve( 1 );
      msg.bytes.push_back( *bytes++ );
      runningStatusType_ = msg.bytes[0];

      return 1;
    }    
    else
    {
      if (sz <= 1)
        return 0;

      // Normal message
      msg.bytes.reserve(3);

      // Setup first two bytes
      if (((uint8_t)bytes[0] & 0x80) == 0)
      {
        msg.bytes.push_back(runningStatusType_);
        msg.bytes.push_back(*bytes++);
      }
      else
      {
        if(sz < 2)
          return 0;

        msg.bytes.push_back(*bytes++);
        msg.bytes.push_back(*bytes++);
        runningStatusType_ = msg.bytes[0];
      }

      switch (message_type((uint8_t)runningStatusType_ & 0xF0))
      {
        case message_type::NOTE_OFF:
        case message_type::NOTE_ON:
        case message_type::POLY_PRESSURE:
        case message_type::CONTROL_CHANGE:
        case message_type::PITCH_BEND:
          if(sz < 3)
            return 0;

          msg.bytes.push_back(*bytes++);
          return 3;

        case message_type::PROGRAM_CHANGE:
        case message_type::AFTERTOUCH:
          return 2;

        default:
          // TODO
          return sz;
      }
    }
  }
};

class midi_in_raw_alsa final : public midi_in_default<midi_in_raw_alsa>
{
public:
  static const constexpr auto backend = "Raw ALSA";

  midi_in_raw_alsa(std::string_view clientName, unsigned int queueSizeLimit)
    : midi_in_default<midi_in_raw_alsa>{nullptr, queueSizeLimit}
  {
  }

  ~midi_in_raw_alsa() override
  {
    // Close a connection if it exists.
    midi_in_raw_alsa::close_port();
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::LINUX_ALSA_RAW;
  }

  void open_port(unsigned int portNumber, std::string_view) override
  {
    if (connected_)
    {
      warning("midi_in_raw_alsa::open_port: a valid connection already exists.");
      return;
    }

    auto device_list = get_device_enumerator();
    device_list.enumerate_cards();

    unsigned int num = device_list.inputs.size();
    if (portNumber >= num)
    {
      error<no_devices_found_error>("midi_in_raw_alsa::open_port: no MIDI output sources found.");
      return;
    }

    const int mode = SND_RAWMIDI_NONBLOCK;
    const char* portname = device_list.inputs[portNumber].device.c_str();
    int status = snd_rawmidi_open(&midiport_, nullptr, portname, mode);
    if (status < 0)
    {
      error<driver_error>("midi_in_raw_alsa::open_port: cannot open device.");
      return;
    }

    this->thread_ = std::thread{[this] {
      running_ = true;
      run_thread();
    }};

    connected_ = true;
  }

  void init_pollfd()
  {
    int num_fds = snd_rawmidi_poll_descriptors_count(this->midiport_);

    this->fds_.clear();
    this->fds_.resize(num_fds);

    snd_rawmidi_poll_descriptors(this->midiport_, fds_.data(), num_fds);
  }

  void run_thread()
  {
    static const constexpr int poll_timeout = 50; // in ms

    init_pollfd();

    while(this->running_)
    {
      // Poll
      int err = poll(fds_.data(), fds_.size(), poll_timeout);
      if (err < 0)
        return;

      if(!this->running_)
        return;

      // Read events
      unsigned short res{};
      err = snd_rawmidi_poll_descriptors_revents(this->midiport_, fds_.data(), fds_.size(), &res);
      if (err < 0)
        return;

      // Did we encounter an error during polling
      if (res & (POLLERR | POLLHUP))
        return;

      // Is there data to read
      if (res & POLLIN)
      {
        if (!read_input_buffer())
          return;
      }
    }
  }

  bool read_input_buffer()
  {
    static const constexpr int nbytes = 1024;

    unsigned char bytes[nbytes];
    const int err = snd_rawmidi_read(this->midiport_, bytes, nbytes);
    if(err > 0)
    {
      // err is the amount of bytes read in that case
      const int length = filter_input_buffer(bytes, err);
      if (length == 0)
        return true;

      // we have "length" midi bytes ready to be processed.
      decoder_.add_bytes(bytes, length);
      return true;
    }
    else if (err < 0 && err != -EAGAIN)
    {
      return false;
    }

    return true;
  }

  int filter_input_buffer(unsigned char* data, int size)
  {
    if(!filter_active_sensing_)
      return size;

    return std::remove(data, data + size, 0xFE) - data;
  }

  void close_port() override
  {
    if (connected_)
    {
      running_ = false;
      thread_.join();

      snd_rawmidi_close(midiport_);
      midiport_ = nullptr;
      connected_ = false;
    }
  }

  void set_client_name(std::string_view clientName) override
  {
    warning("midi_in_raw_alsa::set_client_name: unsupported.");
  }

  void set_port_name(std::string_view portName) override
  {
    warning("midi_in_raw_alsa::set_port_name: unsupported.");
  }

  unsigned int get_port_count() override
  {
    auto device_list = get_device_enumerator();
    device_list.enumerate_cards();

    return device_list.inputs.size();
  }

  std::string get_port_name(unsigned int portNumber) override
  {
    auto device_list = get_device_enumerator();
    device_list.enumerate_cards();

    if (portNumber < device_list.inputs.size())
    {
      return device_list.inputs[portNumber].pretty_name();
    }

    return {};
  }

  raw_alsa_helpers::enumerator get_device_enumerator() const noexcept
  {
    raw_alsa_helpers::enumerator device_list;
    device_list.error_callback = [this] (std::string_view text) {
      this->error<driver_error>(text);
    };
    return device_list;
  }

  snd_rawmidi_t* midiport_{};
  std::thread thread_;
  std::atomic_bool running_{};
  std::vector<pollfd> fds_;
  midi_stream_decoder decoder_{this->inputData_};

  bool filter_active_sensing_ = false;
};

class midi_out_raw_alsa final : public midi_out_default<midi_out_raw_alsa>
{
public:
  static const constexpr auto backend = "Raw ALSA";

  midi_out_raw_alsa(std::string_view)
  {
  }

  ~midi_out_raw_alsa() override
  {
    // Close a connection if it exists.
    midi_out_raw_alsa::close_port();
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::LINUX_ALSA_RAW;
  }

  void open_port(unsigned int portNumber, std::string_view) override
  {
    if (connected_)
    {
      warning("midi_out_raw_alsa::open_port: a valid connection already exists.");
      return;
    }

    auto device_list = get_device_enumerator();
    device_list.enumerate_cards();

    unsigned int num = device_list.outputs.size();
    if (portNumber >= num)
    {
      error<no_devices_found_error>("midi_out_raw_alsa::open_port: no MIDI output sources found.");
      return;
    }

    const int mode = SND_RAWMIDI_SYNC;
    const char* portname = device_list.outputs[portNumber].device.c_str();
    int status = snd_rawmidi_open(NULL, &midiport_, portname, mode);
    if (status < 0)
    {
      error<driver_error>("midi_out_raw_alsa::open_port: cannot open device.");
      return;
    }

    connected_ = true;
  }

  void close_port() override
  {
    if (connected_)
    {
      snd_rawmidi_close(midiport_);
      midiport_ = nullptr;
      connected_ = false;
    }
  }

  void set_client_name(std::string_view clientName) override
  {
    warning("midi_out_raw_alsa::set_client_name: unsupported.");
  }

  void set_port_name(std::string_view portName) override
  {
    warning("midi_out_raw_alsa::set_port_name: unsupported.");
  }

  unsigned int get_port_count() override
  {
    auto device_list = get_device_enumerator();
    device_list.enumerate_cards();

    return device_list.outputs.size();
  }

  std::string get_port_name(unsigned int portNumber) override
  {
    auto device_list = get_device_enumerator();
    device_list.enumerate_cards();

    if (portNumber < device_list.outputs.size())
    {
      return device_list.outputs[portNumber].pretty_name();
    }

    return {};
  }

  void send_message(const unsigned char* message, size_t size) override
  {
    if (!midiport_)
      error<invalid_use_error>(
          "midi_out_raw_alsa::send_message: trying to send a message without an open port.");

    if (!this->chunking)
    {
      write(message, size);
    }
    else
    {
      write_chunked(message, size);
    }
  }

  bool write(const unsigned char* message, size_t size)
  {
    if (snd_rawmidi_write(midiport_, message, size) < 0)
    {
      error<driver_error>("midi_out_raw_alsa::send_message: cannot write message.");
      return false;
    }

    return true;
  }

  std::size_t get_chunk_size() const noexcept
  {
    snd_rawmidi_params_t* param;
    snd_rawmidi_params_alloca(&param);
    snd_rawmidi_params_current(midiport_, param);

    std::size_t buffer_size = snd_rawmidi_params_get_buffer_size(param);
    return std::min(buffer_size, (std::size_t)chunking->size);
  }

  std::size_t get_available_bytes_to_write() const noexcept
  {
    snd_rawmidi_status_t* st{};
    snd_rawmidi_status_alloca(&st);
    snd_rawmidi_status(midiport_, st);

    return snd_rawmidi_status_get_avail(st);
  }

  // inspired from ALSA amidi.c source code
  void write_chunked(const unsigned char* const begin, size_t size)
  {
    const unsigned char* data = begin;
    const unsigned char* end = begin + size;

    const std::size_t chunk_size = std::min(get_chunk_size(), size);

    // Send the first buffer
    int len = chunk_size;

    if (!write(data, len))
      return;

    data += len;

    while (data < end)
    {
      // Wait for the buffer to have some space available
      const std::size_t written_bytes = data - begin;
      std::size_t available{};
      while ((available = get_available_bytes_to_write()) < chunk_size)
      {
        if (!chunking->wait(
                std::chrono::microseconds((chunk_size - available) * 320), written_bytes))
          return;
      };

      if (!chunking->wait(chunking->interval, written_bytes))
        return;

      // Write more data
      int len = end - data;

      // Maybe until the end of the sysex
      if (auto sysex_end = (unsigned char*)memchr(data, 0xf7, len))
        len = sysex_end - data + 1;

      if (len > chunk_size)
        len = chunk_size;

      if (!write(data, len))
        return;

      data += len;
    }
  }

  raw_alsa_helpers::enumerator get_device_enumerator() const noexcept
  {
    raw_alsa_helpers::enumerator device_list;
    device_list.error_callback = [this] (std::string_view text) {
      this->error<driver_error>(text);
    };
    return device_list;
  }

  snd_rawmidi_t* midiport_{};
};

struct raw_alsa_backend
{
  using midi_in = midi_in_raw_alsa;
  using midi_out = midi_out_raw_alsa;
  using midi_observer = observer_dummy;
  static const constexpr auto API = libremidi::API::LINUX_ALSA_RAW;
};
}
