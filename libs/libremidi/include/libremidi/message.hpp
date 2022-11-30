#pragma once
#if defined(MSC_VER)
#  define NOMINMAX 1
#  define WIN32_LEAN_AND_MEAN
#endif
#include <algorithm>
#include <cinttypes>
#include <memory>
#include <stdexcept>
#include <vector>

#if __has_include(<boost/container/small_vector.hpp>) && !defined(LIBREMIDI_NO_BOOST)
#  include <boost/container/small_vector.hpp>
namespace libremidi
{
using midi_bytes = boost::container::small_vector<unsigned char, 4>;
}
#else
namespace libremidi
{
using midi_bytes = std::vector<unsigned char>;
}
#endif

#if defined(LIBREMIDI_HEADER_ONLY)
#  define LIBREMIDI_INLINE inline
#else
#  define LIBREMIDI_INLINE
#endif

namespace libremidi
{
enum class message_type : uint8_t
{
  INVALID = 0x0,
  // Standard Message
  NOTE_OFF = 0x80,
  NOTE_ON = 0x90,
  POLY_PRESSURE = 0xA0,
  CONTROL_CHANGE = 0xB0,
  PROGRAM_CHANGE = 0xC0,
  AFTERTOUCH = 0xD0,
  PITCH_BEND = 0xE0,

  // System Common Messages
  SYSTEM_EXCLUSIVE = 0xF0,
  TIME_CODE = 0xF1,
  SONG_POS_POINTER = 0xF2,
  SONG_SELECT = 0xF3,
  RESERVED1 = 0xF4,
  RESERVED2 = 0xF5,
  TUNE_REQUEST = 0xF6,
  EOX = 0xF7,

  // System Realtime Messages
  TIME_CLOCK = 0xF8,
  RESERVED3 = 0xF9,
  START = 0xFA,
  CONTINUE = 0xFB,
  STOP = 0xFC,
  RESERVED4 = 0xFD,
  ACTIVE_SENSING = 0xFE,
  SYSTEM_RESET = 0xFF
};

enum class meta_event_type : uint8_t
{
  SEQUENCE_NUMBER = 0x00,
  TEXT = 0x01,
  COPYRIGHT = 0x02,
  TRACK_NAME = 0x03,
  INSTRUMENT = 0x04,
  LYRIC = 0x05,
  MARKER = 0x06,
  CUE = 0x07,
  PATCH_NAME = 0x08,
  DEVICE_NAME = 0x09,
  CHANNEL_PREFIX = 0x20,
  MIDI_PORT = 0x21,
  END_OF_TRACK = 0x2F,
  TEMPO_CHANGE = 0x51,
  SMPTE_OFFSET = 0x54,
  TIME_SIGNATURE = 0x58,
  KEY_SIGNATURE = 0x59,
  PROPRIETARY = 0x7F,
  UNKNOWN = 0xFF
};

constexpr inline uint8_t clamp(uint8_t val, uint8_t min, uint8_t max)
{
  return std::max(std::min(val, max), min);
}

struct message
{
  midi_bytes bytes;
  double timestamp{};

  message() noexcept = default;
  message(const midi_bytes& src_bytes, double src_timestamp)
      : bytes(src_bytes), timestamp(src_timestamp)
  {
  }
  message(std::initializer_list<unsigned char> args) noexcept : bytes{args}
  {
  }
  static uint8_t make_command(const message_type type, const int channel) noexcept
  {
    return (uint8_t)((uint8_t)type | clamp(channel, 0, channel - 1));
  }

  static message note_on(uint8_t channel, uint8_t note, uint8_t velocity) noexcept
  {
    return {make_command(message_type::NOTE_ON, channel), note, velocity};
  }

  static message note_off(uint8_t channel, uint8_t note, uint8_t velocity) noexcept
  {
    return {make_command(message_type::NOTE_OFF, channel), note, velocity};
  }

  static message control_change(uint8_t channel, uint8_t control, uint8_t value) noexcept
  {
    return {make_command(message_type::CONTROL_CHANGE, channel), control, value};
  }

  static message program_change(uint8_t channel, uint8_t value) noexcept
  {
    return {make_command(message_type::PROGRAM_CHANGE, channel), value};
  }

  static message pitch_bend(uint8_t channel, int value) noexcept
  {
    return {
        make_command(message_type::PITCH_BEND, channel), (unsigned char)(value & 0x7F),
        (uint8_t)((value >> 7) & 0x7F)};
  }

  static message pitch_bend(uint8_t channel, uint8_t lsb, uint8_t msb) noexcept
  {
    return {make_command(message_type::PITCH_BEND, channel), lsb, msb};
  }

  static message poly_pressure(uint8_t channel, uint8_t note, uint8_t value) noexcept
  {
    return {make_command(message_type::POLY_PRESSURE, channel), note, value};
  }

  static message aftertouch(uint8_t channel, uint8_t value) noexcept
  {
    return {make_command(message_type::AFTERTOUCH, channel), value};
  }

  bool uses_channel(int channel) const
  {
    if (channel <= 0 || channel > 16)
      throw std::range_error("message::uses_channel: out of range");
    return ((bytes[0] & 0xF) == channel - 1) && ((bytes[0] & 0xF0) != 0xF0);
  }

  int get_channel() const
  {
    if ((bytes[0] & 0xF0) != 0xF0)
      return (bytes[0] & 0xF) + 1;
    return 0;
  }

  bool is_meta_event() const
  {
    return bytes[0] == 0xFF;
  }

  meta_event_type get_meta_event_type() const
  {
    if (!is_meta_event())
      return meta_event_type::UNKNOWN;
    return (meta_event_type)bytes[1];
  }

  message_type get_message_type() const
  {
    if (bytes[0] >= uint8_t(message_type::SYSTEM_EXCLUSIVE))
    {
      return (message_type)(bytes[0] & 0xFF);
    }
    else
    {
      return (message_type)(bytes[0] & 0xF0);
    }
  }

  bool is_note_on_or_off() const
  {
    const auto status = get_message_type();
    return (status == message_type::NOTE_ON) || (status == message_type::NOTE_OFF);
  }

  auto size() const
  {
    return bytes.size();
  }

  auto& front() const
  {
    return bytes.front();
  }
  auto& back() const
  {
    return bytes.back();
  }
  auto& operator[](int i) const
  {
    return bytes[i];
  }
  auto& front()
  {
    return bytes.front();
  }
  auto& back()
  {
    return bytes.back();
  }
  auto& operator[](int i)
  {
    return bytes[i];
  }

  template <typename... Args>
  auto assign(Args&&... args)
  {
    return bytes.assign(std::forward<Args>(args)...);
  }
  template <typename... Args>
  auto insert(Args&&... args)
  {
    return bytes.insert(std::forward<Args>(args)...);
  }
  auto clear()
  {
    bytes.clear();
  }

  auto begin() const
  {
    return bytes.begin();
  }
  auto end() const
  {
    return bytes.end();
  }
  auto begin()
  {
    return bytes.begin();
  }
  auto end()
  {
    return bytes.end();
  }
  auto cbegin() const
  {
    return bytes.cbegin();
  }
  auto cend() const
  {
    return bytes.cend();
  }
  auto cbegin()
  {
    return bytes.cbegin();
  }
  auto cend()
  {
    return bytes.cend();
  }
  auto rbegin() const
  {
    return bytes.rbegin();
  }
  auto rend() const
  {
    return bytes.rend();
  }
  auto rbegin()
  {
    return bytes.rbegin();
  }
  auto rend()
  {
    return bytes.rend();
  }
};

struct meta_events
{
  static message end_of_track()
  {
    return {0xFF, 0x2F, 0};
  }

  static message channel(int channel)
  {
    return {0xff, 0x20, 0x01, clamp(0, 0xff, channel - 1)};
  }

  static message tempo(int mpqn)
  {
    return {0xff, 81, 3, (uint8_t)(mpqn >> 16), (uint8_t)(mpqn >> 8), (uint8_t)mpqn};
  }

  static message time_signature(int numerator, int denominator)
  {
    int n = 1;
    int powTwo = 0;

    while (n < denominator)
    {
      n <<= 1;
      ++powTwo;
    }

    return {0xff, 0x58, 0x04, (uint8_t)numerator, (uint8_t)powTwo, 1, 96};
  }

  // Where key index goes from -7 (7 flats, C♭ Major) to +7 (7 sharps, C♯
  // Major)
  static message key_signature(int keyIndex, bool isMinor)
  {
    if (keyIndex < -7 || keyIndex > 7)
      throw std::range_error("meta_events::key_signature: out of range");
    return {0xff, 0x59, 0x02, (uint8_t)keyIndex, isMinor ? (uint8_t)1 : (uint8_t)0};
  }

  static message song_position(int positionInBeats) noexcept
  {
    return {0xf2, (uint8_t)(positionInBeats & 127), (uint8_t)((positionInBeats >> 7) & 127)};
  }
};

struct track_event
{
  int tick = 0;
  int track = 0;
  message m;
};

typedef std::vector<track_event> midi_track;
}
