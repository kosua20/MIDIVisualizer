cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(RtMidi17 CXX)

option(RTMIDI17_HEADER_ONLY "Header-only mode" OFF)
option(RTMIDI17_NO_COREMIDI "Disable CoreMidi back-end" OFF)
option(RTMIDI17_NO_WINMM "Disable WinMM back-end" OFF)
option(RTMIDI17_NO_WINUWP "Disable UWP back-end" ON)
option(RTMIDI17_NO_JACK "Disable JACK back-end" OFF)
option(RTMIDI17_NO_ALSA "Disable ALSA back-end" OFF)
option(RTMIDI17_EXAMPLES "Enable examples" ON)

include(CheckSymbolExists)
### Main library ###
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(RTMIDI17_HEADER_ONLY)
  add_library(RtMidi17 INTERFACE)
  set(_public INTERFACE)
  target_compile_definitions(RtMidi17 ${_public} RTMIDI17_HEADER_ONLY)
else()
  add_library(RtMidi17
    include/rtmidi17/rtmidi17.cpp
    include/rtmidi17/reader.cpp
    include/rtmidi17/writer.cpp
  )
  set(_public PUBLIC)
endif()

target_compile_features(RtMidi17 ${_public} cxx_std_17)

find_package(Threads)
target_link_libraries(RtMidi17 ${_public} ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(RtMidi17 ${_public}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(APPLE)
  ## CoreMIDI support ##
  if(NOT RTMIDI17_NO_COREMIDI)
    target_compile_definitions(RtMidi17 ${_public} RTMIDI17_COREAUDIO)

    find_library(COREMIDI_LIBRARY CoreMIDI)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)

    target_link_libraries(RtMidi17
      ${_public}
        ${COREFOUNDATION_LIBRARY}
        ${COREAUDIO_LIBRARY}
        ${COREMIDI_LIBRARY}
     )
  endif()

elseif(WIN32)
  ## WinMM support ##
  if(NOT RTMIDI17_NO_WINMM)
    target_compile_definitions(RtMidi17
      ${_public}
        RTMIDI17_WINMM
        WIN32=1
        UNICODE=1
        _UNICODE=1
    )
    target_link_libraries(RtMidi17 ${_public} winmm)
  endif()

  ## UWP MIDI support ##
  if(NOT RTMIDI17_NO_WINUWP)
    set(WINSDK_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]")
    find_path(CPPWINRT_PATH "winrt/base.h"
        PATHS
            ${WINSDK_PATH}
        PATH_SUFFIXES
            ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/cppwinrt
            Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/cppwinrt)
    if(CPPWINRT_PATH)
      target_include_directories(RtMidi17 ${_public} "${CPPWINRT_PATH}")
      target_compile_definitions(RtMidi17 ${_public} RTMIDI17_WINUWP)
      target_link_libraries(RtMidi17 INTERFACE RuntimeObject)
      # We don't need /ZW option here (support for C++/CX)' as we use C++/WinRT
      target_compile_options(RtMidi17 ${_public} /EHsc /await)
    else()
      message("Failed to find Windows SDK, UWP MIDI backend will not be available")
    endif()
  endif()

elseif(UNIX)
  ## ALSA support ##
  if(NOT RTMIDI17_NO_ALSA)
    find_package(ALSA)
    if(ALSA_FOUND)
      target_compile_definitions(RtMidi17 ${_public} RTMIDI17_ALSA)
      target_link_libraries(RtMidi17 ${_public} ${ALSA_LIBRARIES})
    endif()
  endif()
endif()

## JACK support ##
if(NOT RTMIDI17_NO_JACK)

  find_path(WEAKJACK_PATH weakjack/weak_libjack.h HINTS ${WEAKJACK_FOLDER})
  find_path(JACK_PATH jack/jack.h)
  if(WEAKJACK_PATH AND JACK_PATH)
    message(" -- RtMidi17 : Using WeakJACK")
    set(HAS_JACK 1)
    target_include_directories(RtMidi17 ${_public} $<BUILD_INTERFACE:${WEAKJACK_PATH}> $<BUILD_INTERFACE:${JACK_PATH}>)
  elseif(JACK_PATH)
    message(" -- RtMidi17 : Using linked JACK")
    find_library(JACK_LIBRARIES jack)
    if(JACK_LIBRARIES)
      set(HAS_JACK 1)
      target_link_libraries(RtMidi17 ${_public} ${JACK_LIBRARIES})
      target_include_directories(RtMidi17 ${_public} $<BUILD_INTERFACE:${JACK_PATH}>)
    endif()
  endif()

  if(HAS_JACK)
    target_compile_definitions(RtMidi17 ${_public} RTMIDI17_JACK)

    check_symbol_exists(jack_port_rename jack/jack.h HAS_JACK_PORT_RENAME)
    if(HAS_JACK_PORT_RENAME)
      target_compile_definitions(RtMidi17 ${_public} RTMIDI17_JACK_HAS_PORT_RENAME)
    endif()
  endif()
endif()

### Install  ###
if(NOT RTMIDI17_HEADER_ONLY)
  install(TARGETS RtMidi17
          EXPORT rtmidi17-exports
          ARCHIVE DESTINATION lib/static
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          )
else()
    install(TARGETS RtMidi17
            EXPORT rtmidi17-exports
            )
endif()
install(EXPORT rtmidi17-exports
        DESTINATION lib/cmake/rtmidi17)
export(EXPORT rtmidi17-exports)

### Examples ###
if(RTMIDI17_EXAMPLES)
  add_executable(midiobserve tests/midiobserve.cpp)
  target_link_libraries(midiobserve PRIVATE RtMidi17)

  add_executable(cmidiin tests/cmidiin.cpp)
  target_link_libraries(cmidiin PRIVATE RtMidi17)

  add_executable(midiclock_in tests/midiclock_in.cpp)
  target_link_libraries(midiclock_in PRIVATE RtMidi17)

  add_executable(midiclock_out tests/midiclock_out.cpp)
  target_link_libraries(midiclock_out PRIVATE RtMidi17)

  add_executable(midiout tests/midiout.cpp)
  target_link_libraries(midiout PRIVATE RtMidi17)

  add_executable(midiprobe tests/midiprobe.cpp)
  target_link_libraries(midiprobe PRIVATE RtMidi17)

  add_executable(qmidiin tests/qmidiin.cpp)
  target_link_libraries(qmidiin PRIVATE RtMidi17)

  add_executable(sysextest tests/sysextest.cpp)
  target_link_libraries(sysextest PRIVATE RtMidi17)
endif()
