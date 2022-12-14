#include <cmath>
#include <sstream>
#include <thread>

#if !defined(LIBREMIDI_HEADER_ONLY)
#  include <libremidi/libremidi.hpp>
#endif

#include <libremidi/detail/midi_api.hpp>
#if !__has_include(<weak_libjack.h>) && !__has_include(<jack/jack.h>)
#  if defined(LIBREMIDI_JACK)
#    undef LIBREMIDI_JACK
#  endif
#endif
#if !defined(LIBREMIDI_ALSA) && !defined(LIBREMIDI_JACK) && !defined(LIBREMIDI_COREAUDIO) \
    && !defined(LIBREMIDI_WINMM)
#  define LIBREMIDI_DUMMY
#endif

#if defined(LIBREMIDI_ALSA)
#  include <libremidi/detail/alsa.hpp>
#  include <libremidi/detail/raw_alsa.hpp>
#endif

#if defined(LIBREMIDI_JACK)
#  include <libremidi/detail/jack.hpp>
#endif

#if defined(LIBREMIDI_COREAUDIO)
#  include <libremidi/detail/coreaudio.hpp>
#endif

#if defined(LIBREMIDI_WINMM)
#  include <libremidi/detail/winmm.hpp>
#endif

#if defined(LIBREMIDI_WINUWP)
#  include <libremidi/detail/winuwp.hpp>
#endif

#if defined(LIBREMIDI_EMSCRIPTEN)
#  include <libremidi/detail/emscripten.hpp>
#endif

#if defined(LIBREMIDI_DUMMY)
#  include <libremidi/detail/dummy.hpp>
#endif

namespace libremidi
{

// The order here will control the order of the API search in
// the constructor.
template <typename unused, typename... Args>
constexpr auto make_tl(unused, Args...)
{
  return std::tuple<Args...>{};
}
static constexpr auto available_backends = make_tl(
    0
#if defined(LIBREMIDI_ALSA)
      , raw_alsa_backend {}
    , alsa_backend {}
#endif
#if defined(LIBREMIDI_COREAUDIO)
    , core_backend {}
#endif
#if defined(LIBREMIDI_JACK)
    , jack_backend {}
#endif
#if defined(LIBREMIDI_WINMM)
    , winmm_backend {}
#endif
#if defined(LIBREMIDI_WINUWP)
    , winuwp_backend {}
#endif
#if defined(LIBREMIDI_EMSCRIPTEN)
    , emscripten_backend {}
#endif
#if defined(LIBREMIDI_DUMMY)
    , dummy_backend {}
#endif
);

// There should always be at least one back-end.
static_assert(std::tuple_size_v<decltype(available_backends)> >= 1);

template <typename F>
auto for_all_backends(F&& f)
{
  std::apply([&](auto&&... x) { (f(x), ...); }, available_backends);
}

template <typename F>
auto for_backend(libremidi::API api, F&& f)
{
  for_all_backends([&](auto b) {
    if (b.API == api)
      f(b);
  });
}

LIBREMIDI_INLINE midi_exception::~midi_exception() = default;
LIBREMIDI_INLINE no_devices_found_error::~no_devices_found_error() = default;
LIBREMIDI_INLINE invalid_device_error::~invalid_device_error() = default;
LIBREMIDI_INLINE memory_error::~memory_error() = default;
LIBREMIDI_INLINE invalid_parameter_error::~invalid_parameter_error() = default;
LIBREMIDI_INLINE invalid_use_error::~invalid_use_error() = default;
LIBREMIDI_INLINE driver_error::~driver_error() = default;
LIBREMIDI_INLINE system_error::~system_error() = default;
LIBREMIDI_INLINE thread_error::~thread_error() = default;

LIBREMIDI_INLINE midi_in::~midi_in() = default;
LIBREMIDI_INLINE midi_out::~midi_out() = default;

LIBREMIDI_INLINE
bool chunking_parameters::default_wait(std::chrono::microseconds time_to_wait, int written_bytes) {
  std::this_thread::sleep_for(time_to_wait);
  return true;
}

[[nodiscard]] LIBREMIDI_INLINE std::vector<libremidi::API> available_apis() noexcept
{
  std::vector<libremidi::API> apis;
  for_all_backends([&](auto b) { apis.push_back(b.API); });
  return apis;
}

[[nodiscard]] LIBREMIDI_INLINE std::unique_ptr<observer_api>
open_midi_observer(libremidi::API api, observer::callbacks&& cb)
{
  std::unique_ptr<observer_api> ptr;

  for_backend(api, [&](auto b) {
    ptr = std::make_unique<typename decltype(b)::midi_observer>(std::move(cb));
  });

  return ptr;
}

[[nodiscard]] LIBREMIDI_INLINE std::unique_ptr<midi_in_api>
open_midi_in(libremidi::API api, std::string_view clientName, unsigned int queueSizeLimit)
{
  std::unique_ptr<midi_in_api> ptr;

  for_backend(api, [&](auto b) {
    ptr = std::make_unique<typename decltype(b)::midi_in>(clientName, queueSizeLimit);
  });

  return ptr;
}

[[nodiscard]] LIBREMIDI_INLINE std::unique_ptr<midi_out_api>
open_midi_out(libremidi::API api, std::string_view clientName)
{

  std::unique_ptr<midi_out_api> ptr;

  for_backend(
      api, [&](auto b) { ptr = std::make_unique<typename decltype(b)::midi_out>(clientName); });

  return ptr;
}

LIBREMIDI_INLINE observer::observer(libremidi::API api, observer::callbacks cbs)
    : impl_{open_midi_observer(api, std::move(cbs))}
{
}

LIBREMIDI_INLINE
observer::~observer() = default;

LIBREMIDI_INLINE
libremidi::API midi_in::get_current_api() const noexcept
{
  return rtapi_->get_current_api();
}

LIBREMIDI_INLINE
void midi_in::open_port(unsigned int portNumber, std::string_view portName)
{
  rtapi_->open_port(portNumber, portName);
}

LIBREMIDI_INLINE
void midi_in::open_virtual_port(std::string_view portName)
{
  rtapi_->open_virtual_port(portName);
}

LIBREMIDI_INLINE
void midi_in::close_port()
{
  rtapi_->close_port();
}

LIBREMIDI_INLINE
bool midi_in::is_port_open() const noexcept
{
  return rtapi_->is_port_open();
}

LIBREMIDI_INLINE
void midi_in::set_callback(message_callback callback)
{
  (static_cast<midi_in_api*>(rtapi_.get()))->set_callback(std::move(callback));
}

LIBREMIDI_INLINE
void midi_in::cancel_callback()
{
  (static_cast<midi_in_api*>(rtapi_.get()))->cancel_callback();
}

LIBREMIDI_INLINE
unsigned int midi_in::get_port_count()
{
  return rtapi_->get_port_count();
}

LIBREMIDI_INLINE
std::string midi_in::get_port_name(unsigned int portNumber)
{
  return rtapi_->get_port_name(portNumber);
}

LIBREMIDI_INLINE
void midi_in::ignore_types(bool midiSysex, bool midiTime, bool midiSense)
{
  (static_cast<midi_in_api*>(rtapi_.get()))->ignore_types(midiSysex, midiTime, midiSense);
}

LIBREMIDI_INLINE
message midi_in::get_message()
{
  return (static_cast<midi_in_api*>(rtapi_.get()))->get_message();
}

LIBREMIDI_INLINE
bool midi_in::get_message(message& msg)
{
  return (static_cast<midi_in_api*>(rtapi_.get()))->get_message(msg);
}

LIBREMIDI_INLINE
void midi_in::set_error_callback(midi_error_callback errorCallback)
{
  rtapi_->set_error_callback(std::move(errorCallback));
}

LIBREMIDI_INLINE
libremidi::API midi_out::get_current_api() noexcept
{
  return rtapi_->get_current_api();
}

LIBREMIDI_INLINE
void midi_out::open_port(unsigned int portNumber, std::string_view portName)
{
  rtapi_->open_port(portNumber, portName);
}

LIBREMIDI_INLINE
void midi_out::open_virtual_port(std::string_view portName)
{
  rtapi_->open_virtual_port(portName);
}

LIBREMIDI_INLINE
void midi_out::close_port()
{
  rtapi_->close_port();
}

LIBREMIDI_INLINE
bool midi_out::is_port_open() const noexcept
{
  return rtapi_->is_port_open();
}

LIBREMIDI_INLINE
unsigned int midi_out::get_port_count()
{
  return rtapi_->get_port_count();
}

LIBREMIDI_INLINE
std::string midi_out::get_port_name(unsigned int portNumber)
{
  return rtapi_->get_port_name(portNumber);
}

LIBREMIDI_INLINE
void midi_out::send_message(const std::vector<unsigned char>& message)
{
  send_message(message.data(), message.size());
}

LIBREMIDI_INLINE
void midi_out::send_message(const libremidi::message& message)
{
  send_message(message.bytes.data(), message.bytes.size());
}

#if LIBREMIDI_HAS_SPAN
LIBREMIDI_INLINE
void midi_out::send_message(std::span<unsigned char> message)
{
  send_message(message.data(), message.size());
}

#endif
LIBREMIDI_INLINE
void midi_out::send_message(const unsigned char* message, size_t size)
{
  (static_cast<midi_out_api*>(rtapi_.get()))->send_message(message, size);
}

LIBREMIDI_INLINE
void midi_out::set_error_callback(midi_error_callback errorCallback) noexcept
{
  rtapi_->set_error_callback(std::move(errorCallback));
}

LIBREMIDI_INLINE
std::string get_version() noexcept
{
  return std::string{LIBREMIDI_VERSION};
}

LIBREMIDI_INLINE
midi_in::midi_in(libremidi::API api, std::string_view clientName, unsigned int queueSizeLimit)
{
  if (api != libremidi::API::UNSPECIFIED)
  {
    // Attempt to open the specified API.
    if ((rtapi_ = open_midi_in(api, clientName, queueSizeLimit)))
    {
      return;
    }

    // No compiled support for specified API value.  Issue a warning
    // and continue as if no API was specified.
    std::cerr << "\nRtMidiIn: no compiled support for specified API argument!\n\n" << std::endl;
  }

  // Iterate through the compiled APIs and return as soon as we find
  // one with at least one port or we reach the end of the list.
  for (const auto& api : available_apis())
  {
    rtapi_ = open_midi_in(api, clientName, queueSizeLimit);
    if (rtapi_ && rtapi_->get_port_count() != 0)
    {
      break;
    }
  }

  if (rtapi_)
  {
    return;
  }
}

LIBREMIDI_INLINE
void midi_in::set_client_name(std::string_view clientName)
{
  rtapi_->set_client_name(clientName);
}

LIBREMIDI_INLINE
void midi_in::set_port_name(std::string_view portName)
{
  rtapi_->set_port_name(portName);
}

LIBREMIDI_INLINE
midi_out::midi_out(libremidi::API api, std::string_view clientName)
{
  if (api != libremidi::API::UNSPECIFIED)
  {
    // Attempt to open the specified API.
    rtapi_ = open_midi_out(api, clientName);
    if (rtapi_)
    {
      return;
    }

    // No compiled support for specified API value.  Issue a warning
    // and continue as if no API was specified.
    std::cerr << "\nRtMidiOut: no compiled support for specified API argument!\n\n" << std::endl;
  }

  // Iterate through the compiled APIs and return as soon as we find
  // one with at least one port or we reach the end of the list.
  for (const auto& api : available_apis())
  {
    rtapi_ = open_midi_out(api, clientName);
    if (rtapi_ && rtapi_->get_port_count() != 0)
    {
      break;
    }
  }

  if (rtapi_)
  {
    return;
  }

  // It should not be possible to get here because the preprocessor
  // definition LIBREMIDI_DUMMY is automatically defined if no
  // API-specific definitions are passed to the compiler. But just in
  // case something weird happens, we'll thrown an error.
  throw midi_exception{"RtMidiOut: no compiled API support found ... critical error!!"};
}

LIBREMIDI_INLINE
void midi_out::set_client_name(std::string_view clientName)
{
  rtapi_->set_client_name(clientName);
}

LIBREMIDI_INLINE
void midi_out::set_port_name(std::string_view portName)
{
  rtapi_->set_port_name(portName);
}

LIBREMIDI_INLINE
void midi_out::set_chunking_parameters(std::optional<chunking_parameters> parameters)
{
  rtapi_->set_chunking_parameters(std::move(parameters));
}
}
