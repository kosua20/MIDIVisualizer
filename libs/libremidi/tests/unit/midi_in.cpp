#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <libremidi/libremidi.hpp>

#if defined(LIBREMIDI_JACK)
#include <jack/jack.h>
TEST_CASE("poly aftertouch", "[midi_in]")
{
  libremidi::midi_out midi_out{libremidi::API::UNIX_JACK, "libremidi-test-out"};
  midi_out.open_port();

  libremidi::midi_in midi{libremidi::API::UNIX_JACK, "libremidi-test"};
  midi.open_port();

  jack_options_t opt = JackNullOption;
  jack_status_t status;
  auto jack_client = jack_client_open("libremidi-tester", opt, &status);
  jack_activate(jack_client);

  jack_connect(jack_client, "libremidi-test-out:libremidi Output", "libremidi-test:libremidi Input");

  while(!midi.get_message().bytes.empty())
    ;

  midi_out.send_message(libremidi::message::poly_pressure(0, 60, 100));
  sleep(1);
  libremidi::message mess = midi.get_message();
  REQUIRE(mess.bytes == libremidi::message::poly_pressure(0, 60, 100).bytes);
}
#endif
