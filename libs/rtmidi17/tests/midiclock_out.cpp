//*****************************************//
//  midiclock.cpp
//
//  Simple program to test MIDI clock sync.  Run midiclock_in in one
//  console and midiclock_out in the other, make sure to choose
//  options that connect the clocks between programs on your platform.
//
//  (C)2016 Refer to README.md in this archive for copyright.
//
//*****************************************//

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <rtmidi17/rtmidi17.hpp>
#include <thread>

// These functions should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
template <typename RT>
bool choosePort(RT& rtmidi, std::string dir)
{
  std::string portName;
  auto port = 0U;
  auto nPorts = rtmidi.get_port_count();

  if (nPorts == 0)
  {
    std::cout << "No " << dir << " ports available!" << std::endl;
    return false;
  }
  else if (nPorts == 1)
  {
    std::cout << "\nOpening " << rtmidi.get_port_name() << std::endl;
  }
  else
  {
    for (auto i = 0U; i < nPorts; i++)
    {
      portName = rtmidi.get_port_name(i);
      std::cout << "  " << dir << " port #" << i << ": " << portName << '\n';
    }

    do
    {
      std::cout << "\nChoose a port number: ";
      std::cin >> port;
    } while (port >= nPorts);
  }

  std::cout << "\n";

  rtmidi.open_port(port);

  return true;
}

int main(int, const char* argv[])
try
{
  using namespace std::literals;
  rtmidi::midi_out midiout;

  // Call function to select port.
  if (choosePort(midiout, "output") == false)
    return 0;

  // Period in ms = 100 BPM
  // 100*24 ticks / 1 minute, so (60*1000) / (100*24) = 25 ms / tick
  int sleep_ms = 25;
  std::cout << "Generating clock at " << (60.0 / 24.0 / sleep_ms * 1000.0) << " BPM." << std::endl;

  // Send out a series of MIDI clock messages.
  // MIDI start
  std::vector<unsigned char> message;
  message.push_back(0xFA);
  midiout.send_message(message);
  std::cout << "MIDI start" << std::endl;

  for (int j = 0; j < 8; j++)
  {
    if (j > 0)
    {
      // MIDI continue
      message.clear();
      message.push_back(0xFB);
      midiout.send_message(message);
      std::cout << "MIDI continue" << std::endl;
    }

    for (int k = 0; k < 96; k++)
    {
      // MIDI clock
      message.clear();
      message.push_back(0xF8);
      midiout.send_message(message);
      if (k % 24 == 0)
        std::cout << "MIDI clock (one beat)" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }

    // MIDI stop
    message.clear();
    message.push_back(0xFC);
    midiout.send_message(message);
    std::cout << "MIDI stop" << std::endl;
    std::this_thread::sleep_for(500ms);
  }

  // MIDI stop
  message.clear();
  message.push_back(0xFC);
  midiout.send_message(message);
  std::cout << "MIDI stop" << std::endl;

  std::this_thread::sleep_for(500ms);

  std::cout << "Done!" << std::endl;

  return 0;
}
catch (const rtmidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  exit(EXIT_FAILURE);
}
