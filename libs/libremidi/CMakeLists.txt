cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(libremidi CXX)

option(LIBREMIDI_HEADER_ONLY "Header-only mode" OFF)
option(LIBREMIDI_NO_COREMIDI "Disable CoreMidi back-end" OFF)
option(LIBREMIDI_NO_WINMM "Disable WinMM back-end" OFF)
option(LIBREMIDI_NO_WINUWP "Disable UWP back-end" ON)
option(LIBREMIDI_NO_JACK "Disable JACK back-end" OFF)
option(LIBREMIDI_NO_ALSA "Disable ALSA back-end" OFF)
option(LIBREMIDI_EXAMPLES "Enable examples" OFF)

include(CheckSymbolExists)
include(CheckCXXSourceCompiles)

### Main library ###
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(LIBREMIDI_HEADER_ONLY)
  add_library(libremidi INTERFACE)
  set(_public INTERFACE)
  target_compile_definitions(libremidi ${_public} LIBREMIDI_HEADER_ONLY)
else()
  add_library(libremidi
    include/libremidi/detail/alsa.hpp
    include/libremidi/detail/coreaudio.hpp
    include/libremidi/detail/dummy.hpp
    include/libremidi/detail/emscripten.hpp
    include/libremidi/detail/emscripten_api.hpp
    include/libremidi/detail/jack.hpp
    include/libremidi/detail/midi_api.hpp
    include/libremidi/detail/raw_alsa.hpp
    include/libremidi/detail/raw_alsa_helpers.hpp
    include/libremidi/detail/semaphore.hpp
    include/libremidi/detail/winmm.hpp
    include/libremidi/detail/winuwp.hpp

    include/libremidi/message.hpp
    include/libremidi/reader.hpp
    include/libremidi/writer.hpp
    include/libremidi/libremidi.hpp

    include/libremidi/libremidi.cpp
    include/libremidi/reader.cpp
    include/libremidi/writer.cpp
  )
  set(_public PUBLIC)
endif()

target_compile_features(libremidi ${_public} cxx_std_17)

find_package(Threads)
target_link_libraries(libremidi ${_public} ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(libremidi ${_public}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX .html)
  target_compile_definitions(libremidi ${_public} LIBREMIDI_EMSCRIPTEN)
elseif(APPLE)
  ## CoreMIDI support ##
  if(NOT LIBREMIDI_NO_COREMIDI)
    target_compile_definitions(libremidi ${_public} LIBREMIDI_COREAUDIO)

    find_library(COREMIDI_LIBRARY CoreMIDI)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)

    target_link_libraries(libremidi
      ${_public}
        ${COREFOUNDATION_LIBRARY}
        ${COREAUDIO_LIBRARY}
        ${COREMIDI_LIBRARY}
     )
  endif()

elseif(WIN32)
  ## WinMM support ##
  if(NOT LIBREMIDI_NO_WINMM)
    target_compile_definitions(libremidi
      ${_public}
        LIBREMIDI_WINMM
        UNICODE=1
        _UNICODE=1
    )
    target_link_libraries(libremidi ${_public} winmm)
  endif()

  ## UWP MIDI support ##
  if(NOT LIBREMIDI_NO_WINUWP)
    set(WINSDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]")
    find_path(CPPWINRT_PATH "winrt/base.h"
        PATHS
            ${WINSDK_PATH}
        PATH_SUFFIXES
            ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/cppwinrt
            Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/cppwinrt)
    if(CPPWINRT_PATH)
      target_include_directories(libremidi ${_public} "${CPPWINRT_PATH}")
      target_compile_definitions(libremidi ${_public} LIBREMIDI_WINUWP)
      target_link_libraries(libremidi INTERFACE RuntimeObject)
      # We don't need /ZW option here (support for C++/CX)' as we use C++/WinRT
      target_compile_options(libremidi ${_public} /EHsc /await)
    else()
      message(STATUS "libremidi : Failed to find Windows SDK, UWP MIDI backend will not be available")
    endif()
  endif()

elseif(UNIX)
  ## ALSA support ##
  if(NOT LIBREMIDI_NO_ALSA)
    find_package(ALSA)
    if(ALSA_FOUND)
      target_compile_definitions(libremidi ${_public} LIBREMIDI_ALSA)
      target_link_libraries(libremidi ${_public} ${ALSA_LIBRARIES})
    endif()
  endif()
endif()

## JACK support ##
if(NOT LIBREMIDI_NO_JACK)
  find_path(WEAKJACK_PATH weakjack/weak_libjack.h HINTS ${WEAKJACK_FOLDER})
  find_path(JACK_PATH jack/jack.h)
  if(WEAKJACK_PATH AND JACK_PATH)
    message(STATUS "libremidi : Using WeakJACK")
    set(HAS_JACK 1)
    target_include_directories(libremidi ${_public} $<BUILD_INTERFACE:${WEAKJACK_PATH}> $<BUILD_INTERFACE:${JACK_PATH}>)
  elseif(JACK_PATH)
    message(STATUS "libremidi : Using linked JACK")
    find_library(JACK_LIBRARIES jack)
    if(JACK_LIBRARIES)
      set(HAS_JACK 1)
      target_link_libraries(libremidi ${_public} ${JACK_LIBRARIES})
      target_include_directories(libremidi ${_public} $<BUILD_INTERFACE:${JACK_PATH}>)
    endif()
  endif()

  if(HAS_JACK)
    target_compile_definitions(libremidi ${_public} LIBREMIDI_JACK)

    check_symbol_exists(jack_port_rename jack/jack.h HAS_JACK_PORT_RENAME)
    if(HAS_JACK_PORT_RENAME)
      target_compile_definitions(libremidi ${_public} LIBREMIDI_JACK_HAS_PORT_RENAME)
    endif()
  endif()
endif()

### Install  ###
if(NOT LIBREMIDI_HEADER_ONLY)
  install(TARGETS libremidi
          EXPORT libremidi-exports
          ARCHIVE DESTINATION lib/static
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          )
else()
    install(TARGETS libremidi
            EXPORT libremidi-exports
            )
endif()
install(EXPORT libremidi-exports
        DESTINATION lib/cmake/libremidi)
export(EXPORT libremidi-exports)

### Examples ###
if(LIBREMIDI_EXAMPLES)
  add_executable(midiobserve tests/midiobserve.cpp)
  target_link_libraries(midiobserve PRIVATE libremidi)

  add_executable(cmidiin tests/cmidiin.cpp)
  target_link_libraries(cmidiin PRIVATE libremidi)

  add_executable(midiclock_in tests/midiclock_in.cpp)
  target_link_libraries(midiclock_in PRIVATE libremidi)

  add_executable(midiclock_out tests/midiclock_out.cpp)
  target_link_libraries(midiclock_out PRIVATE libremidi)

  add_executable(midiout tests/midiout.cpp)
  target_link_libraries(midiout PRIVATE libremidi)

  if(MSVC)
    set(CMAKE_REQUIRED_FLAGS /std:c++20)
  else()
    set(CMAKE_REQUIRED_FLAGS -std=c++2a)
  endif()
  check_cxx_source_compiles("#include <thread>\nint main() { std::jthread t; }" HAS_STD_JTHREAD)

  if(HAS_STD_JTHREAD)
    add_executable(multithread_midiout tests/multithread_midiout.cpp)
    target_compile_features(multithread_midiout PRIVATE cxx_std_20)
    target_link_libraries(multithread_midiout PRIVATE libremidi)
  endif()

  add_executable(midiprobe tests/midiprobe.cpp)
  target_link_libraries(midiprobe PRIVATE libremidi)

  add_executable(qmidiin tests/qmidiin.cpp)
  target_link_libraries(qmidiin PRIVATE libremidi)

  add_executable(sysextest tests/sysextest.cpp)
  target_link_libraries(sysextest PRIVATE libremidi)

  add_executable(midi2 tests/midi2.cpp)
  target_link_libraries(midi2 PRIVATE libremidi)

  if(EMSCRIPTEN)
    add_executable(emscripten_midiin tests/emscripten_midiin.cpp)
    target_link_libraries(emscripten_midiin PRIVATE libremidi)
  endif()
endif()

find_package(Catch2 QUIET)
if(TARGET Catch2::Catch2 AND Catch2_FOUND)
  message(STATUS "libremidi : compiling tests")
  target_compile_features(libremidi ${_public} cxx_std_20)

  add_executable(midiin_test tests/unit/midi_in.cpp)
  target_link_libraries(midiin_test PRIVATE libremidi Catch2::Catch2)

  add_executable(midiout_test tests/unit/midi_out.cpp)
  target_link_libraries(midiout_test PRIVATE libremidi Catch2::Catch2)

  add_executable(midifile_read_test tests/unit/midifile_read.cpp)
  target_link_libraries(midifile_read_test PRIVATE libremidi Catch2::Catch2)
  target_compile_definitions(midifile_read_test PRIVATE "LIBREMIDI_TEST_CORPUS=\"${CMAKE_CURRENT_SOURCE_DIR}/tests/corpus\"")

  add_executable(midifile_write_test tests/unit/midifile_write.cpp)
  target_link_libraries(midifile_write_test PRIVATE libremidi Catch2::Catch2)
  target_compile_definitions(midifile_write_test PRIVATE "LIBREMIDI_TEST_CORPUS=\"${CMAKE_CURRENT_SOURCE_DIR}/tests/corpus\"")

  include(CTest)
  include(Catch)
  catch_discover_tests(midiin_test)
  catch_discover_tests(midiout_test)
  catch_discover_tests(midifile_read_test)
  catch_discover_tests(midifile_write_test)
else()
  message(STATUS "libremidi : not compiling tests")
endif()
