#pragma once
#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <mutex>
#include <libremidi/detail/midi_api.hpp>
#include <libremidi/libremidi.hpp>
#include <string>
#include <thread>
#include <vector>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Devices.Midi.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>

namespace libremidi
{

inline void winrt_init()
{
  // init_apartment should only be called on the threads we own.
  // Since we're the library we don't own the threads we are called from,
  // so we should not perform this initialization ourselves.
  // winrt::init_apartment();
}

namespace
{
using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Devices::Midi;
using namespace winrt::Windows::Devices::Enumeration;
using namespace winrt::Windows::Storage::Streams;

// Helper function to allow waiting for aynchronous operation completion
// from the thread in STA. The only benefit from it compared to the
// get() function from winrt is that we avoid an assertion if waiting
// from the STA thread.
template <typename T>
auto get(T const& async)
{
  if (async.Status() != AsyncStatus::Completed)
  {
    slim_mutex m;
    slim_condition_variable cv;
    bool completed = false;

    async.Completed([&](auto&&, auto&&) {
      {
        slim_lock_guard const guard(m);
        completed = true;
      }

      cv.notify_one();
    });

    slim_lock_guard guard(m);
    cv.wait(m, [&] { return completed; });
  }

  return async.GetResults();
}

class observer_winuwp_internal
{
public:
  struct port_info
  {
    hstring id;
    hstring name;
  };

  observer_winuwp_internal(hstring deviceSelector)
  {
    initialize(deviceSelector);
  }
  ~observer_winuwp_internal()
  {
    terminate();
  }

  std::vector<port_info> get_ports() const
  {
    std::lock_guard<std::mutex> lock(portListMutex_);
    return portList_;
  }

  unsigned int get_port_count() const
  {
    std::lock_guard<std::mutex> lock(portListMutex_);
    return static_cast<unsigned int>(portList_.size());
  }

  bool get_port_info(unsigned int portNumber, port_info& portInfo) const
  {
    std::lock_guard<std::mutex> lock(portListMutex_);
    if (portNumber >= portList_.size())
      return false;
    portInfo = portList_[portNumber];
    return true;
  }

  hstring get_port_id(unsigned int portNumber) const
  {
    std::lock_guard<std::mutex> lock(portListMutex_);
    return portNumber < portList_.size() ? portList_[portNumber].id : hstring{};
  }

  std::string get_port_name(unsigned int portNumber) const
  {
    std::lock_guard<std::mutex> lock(portListMutex_);
    return portNumber < portList_.size() ? to_string(portList_[portNumber].name) : std::string{};
  }

  event_token PortAdded(TypedEventHandler<int, hstring> const& handler)
  {
    return portAddedEvent_.add(handler);
  }

  void PortAdded(event_token const& token) noexcept
  {
    portAddedEvent_.remove(token);
  }

  event_token PortRemoved(TypedEventHandler<int, hstring> const& handler)
  {
    return portRemovedEvent_.add(handler);
  }

  void PortRemoved(event_token const& token) noexcept
  {
    portRemovedEvent_.remove(token);
  }

private:
  observer_winuwp_internal(const observer_winuwp_internal&) = delete;
  observer_winuwp_internal& operator=(const observer_winuwp_internal&) = delete;

private:
  void initialize(hstring deviceSelector)
  {
    deviceWatcher_ = DeviceInformation::CreateWatcher(deviceSelector);

    evTokenOnDeviceAdded_
        = deviceWatcher_.Added({this, &observer_winuwp_internal::on_device_added});
    evTokenOnDeviceRemoved_
        = deviceWatcher_.Removed({this, &observer_winuwp_internal::on_device_removed});
    evTokenOnDeviceUpdated_
        = deviceWatcher_.Updated({this, &observer_winuwp_internal::on_device_updated});
    evTokenOnDeviceEnumerationCompleted_ = deviceWatcher_.EnumerationCompleted(
        {this, &observer_winuwp_internal::on_device_enumeration_completed});

    deviceWatcher_.Start();
  }

  void terminate()
  {
    deviceWatcher_.Stop();
    deviceWatcher_.EnumerationCompleted(evTokenOnDeviceEnumerationCompleted_);
    deviceWatcher_.Updated(evTokenOnDeviceUpdated_);
    deviceWatcher_.Removed(evTokenOnDeviceRemoved_);
    deviceWatcher_.Added(evTokenOnDeviceAdded_);
  }

  void on_device_added(DeviceWatcher sender, DeviceInformation deviceInfo)
  {
    int portNumber = -1;
    hstring name;
    {
      std::lock_guard<std::mutex> lock(portListMutex_);
      portNumber = static_cast<int>(portList_.size());
      name = deviceInfo.Name();
      portList_.push_back({deviceInfo.Id(), deviceInfo.Name()});
    }
    portAddedEvent_(portNumber, name);
  }

  void on_device_removed(DeviceWatcher sender, DeviceInformationUpdate deviceUpdate)
  {
    const auto id = deviceUpdate.Id();
    auto pred = [&id](const port_info& portInfo) { return portInfo.id == id; };
    int portNumber = -1;
    hstring name;
    {
      std::lock_guard<std::mutex> lock(portListMutex_);
      auto iter = std::find_if(portList_.begin(), portList_.end(), pred);
      if (iter != portList_.end())
      {
        portNumber = static_cast<int>(std::distance(portList_.begin(), iter));
        name = iter->name;
        portList_.erase(iter);
      }
    }
    if (portNumber >= 0)
      portRemovedEvent_(portNumber, name);
  }

  void on_device_updated(DeviceWatcher sender, DeviceInformationUpdate deviceUpdate)
  {
  }

  void on_device_enumeration_completed(DeviceWatcher sender, IInspectable const&)
  {
  }

private:
  std::vector<port_info> portList_;
  mutable std::mutex portListMutex_;

  DeviceWatcher deviceWatcher_{nullptr};
  event_token evTokenOnDeviceAdded_;
  event_token evTokenOnDeviceRemoved_;
  event_token evTokenOnDeviceUpdated_;
  event_token evTokenOnDeviceEnumerationCompleted_;

  winrt::event<TypedEventHandler<int, hstring>> portAddedEvent_;
  winrt::event<TypedEventHandler<int, hstring>> portRemovedEvent_;
};

class observer_winuwp final : public observer_api
{
public:
  observer_winuwp(observer::callbacks&& c) : observer_api{std::move(c)}
  {
    evTokenOnInputAdded_
        = internalInPortObserver_.PortAdded({this, &observer_winuwp::on_input_added});
    evTokenOnInputRemoved_
        = internalInPortObserver_.PortRemoved({this, &observer_winuwp::on_input_removed});
    evTokenOnOutputAdded_
        = internalOutPortObserver_.PortAdded({this, &observer_winuwp::on_output_added});
    evTokenOnOutputRemoved_
        = internalOutPortObserver_.PortRemoved({this, &observer_winuwp::on_output_removed});
  }

  ~observer_winuwp()
  {
    internalInPortObserver_.PortAdded(evTokenOnInputAdded_);
    internalInPortObserver_.PortRemoved(evTokenOnInputRemoved_);
    internalOutPortObserver_.PortAdded(evTokenOnOutputAdded_);
    internalOutPortObserver_.PortRemoved(evTokenOnOutputRemoved_);
  }

  static observer_winuwp_internal& get_internal_in_port_observer()
  {
    return internalInPortObserver_;
  }

  static observer_winuwp_internal& get_internal_out_port_observer()
  {
    return internalOutPortObserver_;
  }

  void on_input_added(int portNumber, hstring name)
  {
    if (callbacks_.input_added)
      callbacks_.input_added(portNumber, to_string(name));
  }

  void on_input_removed(int portNumber, hstring name)
  {
    if (callbacks_.input_removed)
      callbacks_.input_removed(portNumber, to_string(name));
  }

  void on_output_added(int portNumber, hstring name)
  {
    if (callbacks_.output_added)
      callbacks_.output_added(portNumber, to_string(name));
  }

  void on_output_removed(int portNumber, hstring name)
  {
    if (callbacks_.output_removed)
      callbacks_.output_removed(portNumber, to_string(name));
  }

private:
  static observer_winuwp_internal internalInPortObserver_;
  static observer_winuwp_internal internalOutPortObserver_;

  event_token evTokenOnInputAdded_;
  event_token evTokenOnInputRemoved_;
  event_token evTokenOnOutputAdded_;
  event_token evTokenOnOutputRemoved_;
};

observer_winuwp_internal observer_winuwp::internalInPortObserver_(MidiInPort::GetDeviceSelector());
observer_winuwp_internal
    observer_winuwp::internalOutPortObserver_(MidiOutPort::GetDeviceSelector());

class midi_in_winuwp final : public midi_in_default<midi_in_winuwp>
{
public:
  static const constexpr auto backend = "UWP";
  midi_in_winuwp(std::string_view, unsigned int queueSizeLimit)
      : midi_in_default{nullptr, queueSizeLimit}
  {
    winrt_init();
  }

  ~midi_in_winuwp() override
  {
    if (port_)
      port_.Close();
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::WINDOWS_UWP;
  }

  void open_port(unsigned int portNumber, std::string_view) override
  {
    if (connected_)
    {
      warning("midi_in_winuwp::openPort: a valid connection already exists!");
      return;
    }

    const auto id = get_port_id(portNumber);
    if (!id.empty())
    {
      port_ = get(MidiInPort::FromIdAsync(id));
      if (port_)
      {
        port_.MessageReceived([=](auto&, auto args) {
          const auto& msg = args.Message();

          auto reader = DataReader::FromBuffer(msg.RawData());
          array_view<uint8_t> bs;
          reader.ReadBytes(bs);

          double t = static_cast<double>(msg.Timestamp().count());
          inputData_.on_message_received(libremidi::message{{bs.begin(), bs.end()}, t});
        });
      }
    }
  }

  void close_port() override
  {
    if (connected_)
    {
      if (port_)
      {
        port_.Close();
      }
    }
  }

  unsigned int get_port_count() override
  {
    auto& observer = observer_winuwp::get_internal_in_port_observer();
    return observer.get_port_count();
  }

  std::string get_port_name(unsigned int portNumber) override
  {
    auto& observer = observer_winuwp::get_internal_in_port_observer();
    return observer.get_port_name(portNumber);
  }

private:
  hstring get_port_id(unsigned int portNumber)
  {
    auto& observer = observer_winuwp::get_internal_in_port_observer();
    return observer.get_port_id(portNumber);
  }

private:
  winrt::Windows::Devices::Midi::MidiInPort port_{nullptr};
};

class midi_out_winuwp final : public midi_out_default<midi_out_winuwp>
{
public:
  static const constexpr auto backend = "UWP";
  midi_out_winuwp(std::string_view)
  {
    winrt_init();
  }

  ~midi_out_winuwp() override
  {
    close_port();
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::WINDOWS_UWP;
  }

  void open_port(unsigned int portNumber, std::string_view) override
  {
    if (connected_)
    {
      warning("midi_out_winuwp::open_port: a valid connection already exists!");
      return;
    }

    const auto id = get_port_id(portNumber);
    if (!id.empty())
    {
      port_ = get(MidiOutPort::FromIdAsync(id));
    }
  }

  void close_port() override
  {
    if (connected_)
    {
      if (port_)
      {
        port_.Close();
      }
    }
  }

  unsigned int get_port_count() override
  {
    auto& out_ports_observer = observer_winuwp::get_internal_out_port_observer();
    return static_cast<unsigned int>(out_ports_observer.get_ports().size());
  }

  std::string get_port_name(unsigned int portNumber) override
  {
    auto& observer = observer_winuwp::get_internal_out_port_observer();
    return observer.get_port_name(portNumber);
  }

  void send_message(const unsigned char* message, size_t size) override
  {
    if (!connected_)
      return;

    InMemoryRandomAccessStream str;
    DataWriter rb(str);
    rb.WriteBytes(
        winrt::array_view<const uint8_t>{(const uint8_t*)message, (const uint8_t*)message + size});
    port_.SendBuffer(rb.DetachBuffer());
  }

private:
  hstring get_port_id(unsigned int portNumber)
  {
    auto& observer = observer_winuwp::get_internal_out_port_observer();
    return observer.get_port_id(portNumber);
  }

private:
  winrt::Windows::Devices::Midi::IMidiOutPort port_{nullptr};
};

struct winuwp_backend
{
  using midi_in = midi_in_winuwp;
  using midi_out = midi_out_winuwp;
  using midi_observer = observer_winuwp;
  static const constexpr auto API = libremidi::API::WINDOWS_UWP;
};
} // namespace
} // namespace libremidi
