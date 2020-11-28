// midiprobe.cpp
//
// Simple program to check MIDI inputs and outputs.
//
// by Gary Scavone, 2003-2012.

#include <cstdlib>
#include <iostream>
#include <map>
#include <rtmidi17/rtmidi17.hpp>

int main()

try
{
  // Create an api map.
  std::map<rtmidi::API, std::string> apiMap{
      {rtmidi::API::MACOSX_CORE, "OS-X CoreMidi"}, {rtmidi::API::WINDOWS_MM, "Windows MultiMedia"},
      {rtmidi::API::UNIX_JACK, "Jack Client"},     {rtmidi::API::LINUX_ALSA, "Linux ALSA"},
      {rtmidi::API::DUMMY, "Dummy (no driver)"},
  };

  auto apis = rtmidi::available_apis();

  std::cout << "\nCompiled APIs:\n";
  for (auto& api : rtmidi::available_apis())
  {
    std::cout << "  " << apiMap[api] << std::endl;
  }

  {
    rtmidi::midi_in midiin;
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
    rtmidi::midi_out midiout;
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
catch (const rtmidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  return EXIT_FAILURE;
}
