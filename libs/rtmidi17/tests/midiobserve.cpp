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
  static std::map<rtmidi::API, std::string> apiMap{
      {rtmidi::API::MACOSX_CORE, "OS-X CoreMidi"}, {rtmidi::API::WINDOWS_MM, "Windows MultiMedia"},
      {rtmidi::API::WINDOWS_UWP, "Windows UWP"},   {rtmidi::API::UNIX_JACK, "Jack Client"},
      {rtmidi::API::LINUX_ALSA, "Linux ALSA"},     {rtmidi::API::DUMMY, "Dummy (no driver)"},
  };

  std::vector<std::unique_ptr<rtmidi::observer>> observers;
  std::cout << "\nCompiled APIs:\n";
  for (auto api : rtmidi::available_apis())
  {
    rtmidi::observer::callbacks cbs;
    cbs.input_added = [=](int i, std::string n) {
      std::cerr << apiMap[api] << " : input added " << i << " => " << n << "\n";
    };
    cbs.input_removed = [=](int i, std::string n) {
      std::cerr << apiMap[api] << " : input removed " << i << " => " << n << "\n";
    };
    cbs.output_added = [=](int i, std::string n) {
      std::cerr << apiMap[api] << " : output added " << i << " => " << n << "\n";
    };
    cbs.output_removed = [=](int i, std::string n) {
      std::cerr << apiMap[api] << " : output removed " << i << " => " << n << "\n";
    };
    std::cout << "  " << apiMap[api] << std::endl;
    observers.push_back(std::make_unique<rtmidi::observer>(api, cbs));
  }

  getchar();
  return 0;
}
catch (const rtmidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  return EXIT_FAILURE;
}
