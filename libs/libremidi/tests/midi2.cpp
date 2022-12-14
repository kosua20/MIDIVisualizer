#include <chrono>
#include <cstdlib>
#include <iostream>
#include <libremidi/libremidi.hpp>
#include <thread>
#include <typeinfo>

int main(int argc, char* argv[])
try
{
  using namespace std::literals;
  std::cout << std::hex;

  libremidi::midi_out midiout;
  libremidi::midi_in midiin;
  // Don't ignore sysex, timing, or active sensing messages.
  midiin.ignore_types(false, false, false);

  midiin.set_callback([](const libremidi::message& message) {
    auto nBytes = message.size();
    for (auto i = 0U; i < nBytes; i++)
      std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
    if (nBytes > 0)
      std::cout << "stamp = " << message.timestamp << std::endl;
  });

  std::string name = "A-88MK2";
  int in_idx = -1;
  int out_idx = -1;
  for (std::size_t i = 0; i < midiin.get_port_count(); i++) {
    if (midiin.get_port_name(i).find(name) != std::string::npos)
    {
      std::cout << i << " = " << midiin.get_port_name(i) << std::endl;
      in_idx = i;
      break;
    }
  }

  for (std::size_t i = 0; i < midiout.get_port_count(); i++) {
    if(midiout.get_port_name(i).find(name) != std::string::npos)
    {
      std::cout << i << " = " << midiout.get_port_name(i) << std::endl;
      out_idx = i;
      break;
    }
  }

  if (in_idx == -1 || out_idx == -1) {
    std::cerr << "MIDI 2 device not found (searching for a Roland A-88 MKII";
    return 1;
  }
  midiin.open_port(in_idx);
  midiout.open_port(out_idx);

  // Multibytes are all LSB first
  std::vector<unsigned char> message{
    0xF0, // Sysex start
    0x7E, // Universal sysex
    0x7F, // Device ID: 7F = To MIDI port / 00-0F : to MIDI channel
    0x0D, // MIDI-CI
    0x70, // Discovery
    0x01, // MIDI-CI version (v1.1 == 0x01)
    0x01, 0x02, 0x03, 0x04, // Source MUID (must be randomly generated on each reboot)
    0x0F, 0x7F, 0x7F, 0x7F, // Destination MUID (here, broadcast)
    0x01, 0x02, 0x03,       // Device manufacturer (sysex id)
    0x01, 0x02,             // Device family
    0x01, 0x02,             // Device family model number
    0x01, 0x02, 0x03, 0x04, // Device software revision level
    0b00001110,             // Capability inquiry category supported (everything supported in this case)
    0x0F, 0x0F, 0x0F, 0x0F, // Receivable maximum sysex message size
    0xF7
  };
  midiout.send_message(message);
  std::this_thread::sleep_for(3000ms); // maximum allowed timeout is 3s
}
catch (libremidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  return 0;
}
