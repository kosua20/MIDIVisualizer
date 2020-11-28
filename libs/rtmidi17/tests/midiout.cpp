//*****************************************//
//  midiout.cpp
//  by Gary Scavone, 2003-2004.
//
//  Simple program to test MIDI output.
//
//*****************************************//

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <rtmidi17/rtmidi17.hpp>
#include <thread>

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
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

  std::cout << "\n";
  rtmidi.open_port(i);

  return true;
}

int main(void)
try
{
  using namespace std::literals;
  rtmidi::midi_out midiout;
  std::vector<unsigned char> message;

  // Call function to select port.
  if (chooseMidiPort(midiout) == false)
    return 0;

  // Send out a series of MIDI messages.

  // Program change: 192, 5
  message.push_back(192);
  message.push_back(5);
  midiout.send_message(message);

  std::this_thread::sleep_for(500ms);

  message[0] = 0xF1;
  message[1] = 60;
  midiout.send_message(message);

  // Control Change: 176, 7, 100 (volume)
  message[0] = 176;
  message[1] = 7;
  message.push_back(100);
  midiout.send_message(message);

  // Note On: 144, 64, 90
  message[0] = 144;
  message[1] = 64;
  message[2] = 90;
  midiout.send_message(message);

  std::this_thread::sleep_for(500ms);

  // Note Off: 128, 64, 40
  message[0] = 128;
  message[1] = 64;
  message[2] = 40;
  midiout.send_message(message);

  std::this_thread::sleep_for(500ms);

  // Control Change: 176, 7, 40
  message[0] = 176;
  message[1] = 7;
  message[2] = 40;
  midiout.send_message(message);

  std::this_thread::sleep_for(500ms);

  // Sysex: 240, 67, 4, 3, 2, 247
  message[0] = 240;
  message[1] = 67;
  message[2] = 4;
  message.push_back(3);
  message.push_back(2);
  message.push_back(247);
  midiout.send_message(message);

  return 0;
}
catch (const rtmidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  exit(EXIT_FAILURE);
}
