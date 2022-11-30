#pragma once

#if defined(__EMSCRIPTEN__)
#include <libremidi/detail/midi_api.hpp>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <map>
#include <emscripten.h>

namespace libremidi
{
class observer_emscripten;
class midi_in_emscripten;
namespace webmidi_helpers
{
struct device_information
{
  std::string id;
  std::string name;
  bool connected{};
};
}

class observer_emscripten final : public observer_api
{
  using device = webmidi_helpers::device_information;
public:
  observer_emscripten(observer::callbacks&& c);
  ~observer_emscripten();

  void update(const std::vector<device>& current_inputs, const std::vector<device>& current_outputs);

private:
  std::vector<device> m_known_inputs;
  std::vector<device> m_known_outputs;
};

class midi_in_emscripten final : public midi_in_default<midi_in_emscripten>
{
public:
  static const constexpr auto backend = "Emscripten";

  midi_in_emscripten(std::string_view clientName, unsigned int queueSizeLimit);
  ~midi_in_emscripten() override;

  libremidi::API get_current_api() const noexcept override;

  void open_port(unsigned int portNumber, std::string_view) override;
  void close_port() override;

  void set_client_name(std::string_view clientName) override;
  void set_port_name(std::string_view portName) override;

  void on_input(message msg);

  unsigned int get_port_count() override;
  std::string get_port_name(unsigned int portNumber) override;

private:
  int portNumber_{};
};

class midi_out_emscripten final : public midi_out_default<midi_out_emscripten>
{
public:
  static const constexpr auto backend = "Raw ALSA";

  midi_out_emscripten(std::string_view);
  ~midi_out_emscripten() override;

  libremidi::API get_current_api() const noexcept override;

  void open_port(unsigned int portNumber, std::string_view) override;
  void close_port() override;

  void set_client_name(std::string_view clientName) override;
  void set_port_name(std::string_view portName) override;

  unsigned int get_port_count() override;
  std::string get_port_name(unsigned int portNumber) override;

  void send_message(const unsigned char* message, size_t size) override;

private:
  int portNumber_{};
};

}
extern "C"
{
EMSCRIPTEN_KEEPALIVE
void libremidi_devices_poll();

EMSCRIPTEN_KEEPALIVE
void libremidi_devices_input(int port, double timestamp, int len, char* bytes);
}
#endif
