//*****************************************//
//  cmidiin.cpp
//  by Gary Scavone, 2003-2004.
//
//  Simple program to test MIDI input and
//  use of a user callback function.
//
//*****************************************//

#include <cstdlib>
#include <iostream>
#include <rtmidi17/rtmidi17.hpp>

[[noreturn]] void usage()
{
  // Error function in case of incorrect command-line
  // argument specifications.
  std::cout << "\nuseage: cmidiin <port>\n";
  std::cout << "    where port = the device to use (default = 0).\n\n";
  exit(0);
}

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort(rtmidi::midi_in& rtmidi)
{
  std::cout << "\nWould you like to open a virtual input port? [y/N] ";

  std::string keyHit;
  std::getline(std::cin, keyHit);
  if (keyHit == "y")
  {
    rtmidi.open_virtual_port();
    return true;
  }

  std::string portName;
  unsigned int i = 0, nPorts = rtmidi.get_port_count();
  if (nPorts == 0)
  {
    std::cout << "No input ports available!" << std::endl;
    return false;
  }

  if (nPorts == 1)
  {
    std::cout << "\nOpening " << rtmidi.get_port_name() << std::endl;
  }
  else
  {
    for (i = 0; i < nPorts; i++)
    {
      portName = rtmidi.get_port_name(i);
      std::cout << "  Input port #" << i << ": " << portName << '\n';
    }

    do
    {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while (i >= nPorts);
    std::getline(std::cin, keyHit); // used to clear out stdin
  }

  rtmidi.open_port(i);

  return true;
}

int main(int argc, char**)
try
{
  rtmidi::midi_in midiin;

  // Minimal command-line check.
  if (argc > 2)
    usage();

  // Call function to select port.
  if (chooseMidiPort(midiin) == false)
    return 0;

  // Set our callback function.  This should be done immediately after
  // opening the port to avoid having incoming messages written to the
  // queue instead of sent to the callback function.
  midiin.set_callback([](const rtmidi::message& message) {
    auto nBytes = message.size();
    for (auto i = 0U; i < nBytes; i++)
      std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
    if (nBytes > 0)
      std::cout << "stamp = " << message.timestamp << std::endl;
  });

  // Don't ignore sysex, timing, or active sensing messages.
  midiin.ignore_types(false, false, false);

  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);
}
catch (const rtmidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  return EXIT_FAILURE;
}
