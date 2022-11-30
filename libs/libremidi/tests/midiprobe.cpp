// midiprobe.cpp
//
// Simple program to check MIDI inputs and outputs.
//
// by Gary Scavone, 2003-2012.

#include <cstdlib>
#include <iostream>
#include <map>
#include <libremidi/libremidi.hpp>

int main()

try
{
  // Create an api map.
  std::map<libremidi::API, std::string> apiMap{
      {libremidi::API::MACOSX_CORE, "OS-X CoreMidi"},  {libremidi::API::WINDOWS_MM, "Windows MultiMedia"},
      {libremidi::API::UNIX_JACK, "Jack Client"},      {libremidi::API::LINUX_ALSA_SEQ, "Linux ALSA (sequencer)"},
      {libremidi::API::LINUX_ALSA_RAW, "Linux ALSA (raw)"},
      {libremidi::API::DUMMY, "Dummy (no driver)"},
  };

  auto apis = libremidi::available_apis();

  std::cout << "\nCompiled APIs:\n";
  for (auto& api : libremidi::available_apis())
  {
    std::cout << "  " << apiMap[api] << std::endl;
  }

  {
    libremidi::midi_in midiin;
    std::cout << "\nCurrent input API: " << apiMap[midiin.get_current_api()] << std::endl;

    // Check inputs.
    auto nPorts = midiin.get_port_count();
    std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";

    for (unsigned i = 0; i < nPorts; i++)
    {
      std::string portName = midiin.get_port_name(i);
      std::cout << "  Input Port #" << i + 1 << ": " << portName << '\n';
    }
  }

  {
    libremidi::midi_out midiout;
    std::cout << "\nCurrent output API: " << apiMap[midiout.get_current_api()] << std::endl;

    // Check outputs.
    auto nPorts = midiout.get_port_count();
    std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";

    for (unsigned i = 0; i < nPorts; i++)
    {
      std::string portName = midiout.get_port_name(i);
      std::cout << "  Output Port #" << i + 1 << ": " << portName << std::endl;
    }
    std::cout << std::endl;
  }
  return 0;
}
catch (const libremidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  return EXIT_FAILURE;
}
