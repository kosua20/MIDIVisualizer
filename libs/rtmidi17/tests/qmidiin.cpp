//*****************************************//
//  qmidiin.cpp
//  by Gary Scavone, 2003-2004.
//
//  Simple program to test MIDI input and
//  retrieval from the queue.
//
//*****************************************//

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <rtmidi17/rtmidi17.hpp>
#include <thread>

[[noreturn]] static void usage()
{
  // Error function in case of incorrect command-line
  // argument specifications.
  std::cout << "\nusage: qmidiin <port>\n";
  std::cout << "    where port = the device to use (default = 0).\n\n";
  exit(0);
}

int main(int argc, char** argv)
try
{
  using namespace std::literals;
  // Minimal command-line check.
  if (argc > 2)
    usage();

  rtmidi::midi_in midiin;

  // Check available ports vs. specified.
  auto port = 0U;
  if (argc == 2)
    port = atoi(argv[1]);

  if (port >= midiin.get_port_count())
  {
    std::cout << "Invalid port specifier!\n";
    usage();
  }

  midiin.open_port(port);

  // Don't ignore sysex, timing, or active sensing messages.
  midiin.ignore_types(false, false, false);

  // Install an interrupt handler function.
  static std::atomic_bool done{};
  signal(SIGINT, [](int) { done = true; });

  // Periodically check input queue.
  std::cout << "Reading MIDI from port " << midiin.get_port_name() << " ... quit with Ctrl-C.\n";
  while (!done)
  {
    auto msg = midiin.get_message();
    auto nBytes = msg.size();

    for (auto i = 0U; i < nBytes; i++)
      std::cout << "Byte " << i << " = " << (int)msg[i] << ", ";

    if (nBytes > 0)
      std::cout << "stamp = " << msg.timestamp << std::endl;

    std::this_thread::sleep_for(10ms);
  }

  return 0;
}
catch (const rtmidi::midi_exception& error)
{
  std::cerr << error.what() << std::endl;
  return EXIT_FAILURE;
}
