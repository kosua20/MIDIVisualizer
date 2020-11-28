//*****************************************//
//  sysextest.cpp
//  by Gary Scavone, 2003-2005.
//
//  Simple program to test MIDI sysex sending and receiving.
//
//*****************************************//

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <rtmidi17/rtmidi17.hpp>
#include <thread>
#include <typeinfo>

[[noreturn]] void usage()
{
  std::cout << "\nuseage: sysextest N\n";
  std::cout << "    where N = length of sysex message to send / receive.\n\n";
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
  }

  std::cout << std::endl;
  rtmidi.open_port(i);

  return true;
}

bool chooseMidiPort(rtmidi::midi_out& rtmidi)
{
  std::cout << "\nWould you like to open a virtual output port? [y/N] ";

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
    std::cout << "No output ports available!" << std::endl;
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
      std::cout << "  Output port #" << i << ": " << portName << '\n';
    }

    do
    {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while (i >= nPorts);
  }

  std::cout << std::endl;
  rtmidi.open_port(i);

  return true;
}

int main(int argc, char* argv[])
try
{
  using namespace std::literals;
  rtmidi::midi_out midiout;
  rtmidi::midi_in midiin;

  // Minimal command-line check.
  if (argc != 2)
    usage();
  auto nBytes = (unsigned int)atoi(argv[1]);

  // Don't ignore sysex, timing, or active sensing messages.
  midiin.ignore_types(false, true, true);

  if (chooseMidiPort(midiin) == false)
    return 0;
  if (chooseMidiPort(midiout) == false)
    return 0;

  midiin.set_callback([](const rtmidi::message& message) {
    auto nBytes = message.size();
    for (auto i = 0U; i < nBytes; i++)
      std::cout << "Byte " << i << " = " << (int)message[i] << ", ";
    if (nBytes > 0)
      std::cout << "stamp = " << message.timestamp << std::endl;
  });

  std::vector<unsigned char> message;
  message.push_back(0xF6);
  midiout.send_message(message);
  std::this_thread::sleep_for(500ms); // pause a little

  // Create a long sysex message of numbered bytes and send it out ... twice.
  for (int n = 0; n < 2; n++)
  {
    message.clear();
    message.push_back(240);
    for (auto i = 0U; i < nBytes; i++)
      message.push_back(i % 128);

    message.push_back(247);
    midiout.send_message(message);

    std::this_thread::sleep_for(500ms); // pause a little
  }
}
catch (rtmidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  return 0;
}
