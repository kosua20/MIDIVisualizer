#pragma once
#include <vector>

namespace libremidi
{
//! MIDI API specifier arguments.
enum class API
{
  UNSPECIFIED, /*!< Search for a working compiled API. */
  MACOSX_CORE, /*!< Macintosh OS-X Core Midi API. */
  LINUX_ALSA, /*!< The Advanced Linux Sound Architecture API. */
  LINUX_ALSA_SEQ = LINUX_ALSA,
  LINUX_ALSA_RAW, /*!< Raw ALSA API. */
  UNIX_JACK, /*!< The JACK Low-Latency MIDI Server API. */
  WINDOWS_MM, /*!< The Microsoft Multimedia MIDI API. */
  WINDOWS_UWP, /*!< The Microsoft WinRT MIDI API. */
  EMSCRIPTEN_WEBMIDI, /*!< Web MIDI API through Emscripten */
  DUMMY       /*!< A compilable but non-functional API. */
};

/**
 * \brief A static function to determine the available compiled MIDI APIs.

  The values returned in the std::vector can be compared against
  the enumerated list values.  Note that there can be more than one
  API compiled for certain operating systems.
*/
std::vector<libremidi::API> available_apis() noexcept;

/**
 * @brief Returns the default backend to use for the target OS.
 *
 * e.g. the one that uses the "recommended" system MIDI API.
 */
inline constexpr libremidi::API default_platform_api() noexcept
{
#if defined(__APPLE__)
  return API::MACOSX_CORE;
#elif defined(_WIN32)
  return API::WINDOWS_MM;
#elif defined(__linux__)
  return API::LINUX_ALSA_SEQ;
#elif defined(__emscripten__)
  return API::EMSCRIPTEN_WEBMIDI;
#else
  return API::DUMMY;
#endif
}
}
