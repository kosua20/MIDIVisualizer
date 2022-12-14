# libremidi

[![Build status](https://github.com/jcelerier/libremidi/workflows/Build/badge.svg)](https://github.com/jcelerier/libremidi/actions)

libremidi is a cross-platform C++17 library for real-time and MIDI file input and output.

This is a fork based on two libraries: 

* [RtMidi](https://github.com/theSTK/RtMidi)
* [ModernMIDI](https://github.com/ddiakopoulos/ModernMIDI)

## Design
Instead of adding another abstraction layer like ModernMIDI does, 
this directly modernizes the RtMidi codebase with new C++ features and improvements: 

* The library can be used header-only, [as explained in the docs](docs/header-only.md)

* Callbacks are passed by `std::function` and generally simplified.
* Ability to use `boost::small_vector` to pass midi bytes instead of `std::vector` to reduce allocations.
* Allow to pass `span` when available (C++20) or `(uint8_t* bytes, std::size_t size)` pairs whenever possible to reduce copying.
* Less indirections, virtuals and memory allocations.
* Simplify usage of some functions, use C++ return style everywhere.
* Use of standard C++ `snake_case`.
* Simplification of exceptions.
* Passes clean through clang-tidy, clang analyzer, GCC -Wall -Wextra, ASAN, UBSAN etc etc.

## New & improved backends
* JACK support on Windows.
* JACK support through weakjack to allow runtime loading of JACK.
* UWP MIDI support on Windows
* Emscripten support to run on a web browser with WebMIDI.
* Raw ALSA support in addition to the existing ALSA sequencer support.

## New features & APIs 
* Notification of device connection / disconnection.
* Support chunking of output data (only supported on raw ALSA backend so far).

## Roadmap
* MIDI 2.0 support.
* More tests and compliance checks.
* Work even more towards this library being a zero-cost abstraction on top of native MIDI APIs.
* Rethink some design issues with the original RtMidi, for instance the way port numbers work is not reliable.
* Refactor duplicated code across backends.

# They use this library

* [ossia.io](https://ossia.io): libremidi is used for every MIDI operation.
