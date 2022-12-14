#pragma once
#include <libremidi/detail/midi_api.hpp>
#include <libremidi/libremidi.hpp>

namespace libremidi
{
class observer_dummy final : public observer_api
{
public:
  observer_dummy(observer::callbacks&& c) : observer_api{std::move(c)}
  {
  }

  ~observer_dummy()
  {
  }
};

class midi_in_dummy final : public midi_in_api
{
public:
  midi_in_dummy(std::string_view /*clientName*/, unsigned int queueSizeLimit)
      : midi_in_api{nullptr, queueSizeLimit}
  {
    warning("midi_in_dummy: This class provides no functionality.");
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::DUMMY;
  }

  void open_port(unsigned int /*portNumber*/, std::string_view /*portName*/) override
  {
  }

  void open_virtual_port(std::string_view /*portName*/) override
  {
  }

  void close_port() override
  {
  }

  void set_client_name(std::string_view /*clientName*/) override
  {
  }

  void set_port_name(std::string_view /*portName*/) override
  {
  }

  unsigned int get_port_count() override
  {
    return 0;
  }

  std::string get_port_name(unsigned int /*portNumber*/) override
  {
    using namespace std::literals;
    return ""s;
  }
};

class midi_out_dummy final : public midi_out_api
{
public:
  explicit midi_out_dummy(std::string_view /*clientName*/)
  {
    warning("midi_out_dummy: This class provides no functionality.");
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::DUMMY;
  }

  void open_port(unsigned int /*portNumber*/, std::string_view /*portName*/) override
  {
  }
  void open_virtual_port(std::string_view /*portName*/) override
  {
  }
  void close_port() override
  {
  }
  void set_client_name(std::string_view /*clientName*/) override
  {
  }
  void set_port_name(std::string_view /*portName*/) override
  {
  }
  unsigned int get_port_count() override
  {
    return 0;
  }
  std::string get_port_name(unsigned int /*portNumber*/) override
  {
    using namespace std::literals;
    return ""s;
  }
  void send_message(const unsigned char* /*message*/, size_t /*size*/) override
  {
  }
};

struct dummy_backend
{
  using midi_in = midi_in_dummy;
  using midi_out = midi_out_dummy;
  using midi_observer = observer_dummy;
  static const constexpr auto API = libremidi::API::DUMMY;
};
}
