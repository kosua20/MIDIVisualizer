# RtMidi17

[![Build status](https://github.com/jcelerier/RtMidi17/workflows/Build/badge.svg)](https://github.com/jcelerier/RtMidi17/actions)

RtMidi17 is a header-only cross-platform C++17 library for MIDI input and output.

This is a fork based on two libraries: 

* [RtMidi](https://github.com/theSTK/RtMidi)
* [ModernMIDI](https://github.com/ddiakopoulos/ModernMIDI)

Instead of adding another abstraction layer like ModernMIDI does, 
this directly modernizes the RtMidi codebase with new C++ features and improvements: 

* Callbacks are passed by `std::function` and generally simplified.
* Ability to use `boost::small_vector` to pass midi bytes instead of `std::vector` to reduce allocations.
* Less indirections, virtuals and memory allocations.
* Simplify usage of some functions, use C++ return style everywhere.
* Use of standard C++ `snake_case`.
* Use of standard C++ folder hierarchy ; just add the current folder to your include path.
* Simplification of exceptions.
* The library can be used header-only with the define `RTMIDI17_HEADER_ONLY`.
* Passes clean through clang-tidy, clang analyzer, GCC -Wall -Wextra, etc etc.
* JACK support on Windows.
* JACK support through weakjack to allow runtime loading of JACK.

### To-dos: 
* Work-in-progress support for notification on device connection / disconnection (currently ALSA only)
* Work-in-progress support for UWP MIDI support on Windows

