/*
Copyright (c) 2015, Dimitri Diakopoulos All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if !defined(LIBREMIDI_HEADER_ONLY)
#  include <libremidi/reader.hpp>
#endif
#include <algorithm>
#include <iostream>
#include <libremidi/message.hpp>

// File Parsing Validation Todo:
// ==============================
// [] Bad file name
// [] Bad header
// [] Unknown header type
// [] Bad header size
// [] Bad type
// [] Bad tmecode
// [] Header too short
// [] Track too short
// [] Event too short
// ==============================

namespace libremidi
{
namespace util
{
struct no_validator
{
  static inline bool validate_track(const midi_track& track)
  {
    return true;
  }
};

struct validator
{
  static inline bool validate_track(const midi_track& track)
  {
    if(track.empty())
    {
      std::cerr << "libremidi::reader: empty track" << std::endl;
      return false;
    }

    auto& last_event = track.back();
    if(last_event.m.bytes != midi_bytes{0xFF, (unsigned char)meta_event_type::END_OF_TRACK, 0})
    {
      std::cerr << "libremidi::reader: track does not end with END OF TRACK" << std::endl;
      return false;
    }

    return true;
  }
};

// Used when we know that we have enough space
struct read_unchecked
{
// Read a MIDI-style variable-length integer (big-endian value in groups of 7 bits,
// with top bit set to signify that another byte follows).
static inline void ensure_size(const uint8_t* begin, const uint8_t* end, int64_t needed)
{
}

static inline uint32_t read_variable_length(uint8_t const*& data, uint8_t const* end)
{
  uint32_t result = 0;
  while (true)
  {
    uint8_t b = *data++;
    if (b & 0x80)
    {
      result += (b & 0x7F);
      result <<= 7;
    }
    else
    {
      return result + b; // b is the last byte
    }
  }
}

static inline void read_bytes(midi_bytes& buffer, uint8_t const*& data, const uint8_t* end, int num)
{
  buffer.reserve(buffer.size() + num);
  for (int i = 0; i < num; ++i)
    buffer.push_back(uint8_t(*data++));
}

static inline uint16_t read_uint16_be(uint8_t const*& data, const uint8_t* end)
{
  uint16_t result = int(*data++) << 8;
  result += int(*data++);
  return result;
}

static inline uint32_t read_uint24_be(uint8_t const*& data, const uint8_t* end)
{
  uint32_t result = int(*data++) << 16;
  result += int(*data++) << 8;
  result += int(*data++);
  return result;
}

static inline uint32_t read_uint32_be(uint8_t const*& data, const uint8_t* end)
{
  uint32_t result = int(*data++) << 24;
  result += int(*data++) << 16;
  result += int(*data++) << 8;
  result += int(*data++);
  return result;
}
};

// Used when we do not know if we have enough bytes and have to check before reading
struct read_checked
{
// Read a MIDI-style variable-length integer (big-endian value in groups of 7 bits,
// with top bit set to signify that another byte follows).
static inline void ensure_size(const uint8_t* begin, const uint8_t* end, int64_t needed)
{
  if(int64_t available = (end - begin); available < needed)
    throw std::runtime_error("MIDI reader: not enough data to process");
}

static inline uint32_t read_variable_length(uint8_t const*& data, uint8_t const* end)
{
  uint32_t result = 0;
  while (true)
  {
    ensure_size(data, end, 1);
    uint8_t b = *data++;
    if (b & 0x80)
    {
      result += (b & 0x7F);
      result <<= 7;
    }
    else
    {
      return result + b; // b is the last byte
    }
  }
}

static inline void read_bytes(midi_bytes& buffer, uint8_t const*& data, uint8_t const* end, int num)
{
  ensure_size(data, end, num);
  read_unchecked::read_bytes(buffer, data, end, num);
}

static inline uint16_t read_uint16_be(uint8_t const*& data, uint8_t const* end)
{
  ensure_size(data, end, 2);
  return read_unchecked::read_uint16_be(data, end);
}

static inline uint32_t read_uint24_be(uint8_t const*& data, uint8_t const* end)
{
  ensure_size(data, end, 3);
  return read_unchecked::read_uint24_be(data, end);
}

static inline uint32_t read_uint32_be(uint8_t const*& data, uint8_t const* end)
{
  ensure_size(data, end, 4);
  return read_unchecked::read_uint32_be(data, end);
}

};
}

#if defined(LIBREMIDI_UNCHECKED)
using byte_reader = util::read_unchecked;
#else
using byte_reader = util::read_checked;
#endif

#if defined(LIBREMIDI_UNVALIDATED)
using validator = util::no_validator;
#else
using validator = util::validator;
#endif

LIBREMIDI_INLINE
track_event parse_event(int tick, int track, const uint8_t*& dataStart, const uint8_t* dataEnd, message_type lastEventTypeByte)
{
  byte_reader::ensure_size(dataStart, dataEnd, 1);
  message_type type = (message_type)*dataStart++;

  track_event event{tick, track, message{}};

  if (((uint8_t)type & 0xF0) == 0xF0)
  {
    // Meta event
    if ((uint8_t)type == 0xFF)
    {
      byte_reader::ensure_size(dataStart, dataEnd, 1);
      meta_event_type subtype = (meta_event_type)*dataStart++;

      event.m.bytes.reserve(3);
      event.m.bytes.push_back((uint8_t)type);
      event.m.bytes.push_back((uint8_t)subtype);

      uint32_t length = 0;
      // Here we read the meta-event length manually, as this way we can also put it into event.m.bytes
      while (true)
      {
        byte_reader::ensure_size(dataStart, dataEnd, 1);
        uint8_t b = *dataStart++;
        event.m.bytes.push_back(b);
        if (b & 0x80)
        {
          uint8_t byte = (b & 0x7F);

          length += byte;
          length <<= 7;
        }
        else
        {
          length += b; // b is the last byte
          break;
        }
      }

      switch (subtype)
      {
        case meta_event_type::SEQUENCE_NUMBER:
        {
          switch(length)
          {
            case 0:
              return event;
            case 2:
              byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, 2);
              return event;
            default:
              throw std::invalid_argument("Expected length for SEQUENCE_NUMBER event is 0 or 2");
          }
        }
        case meta_event_type::TEXT:
        case meta_event_type::COPYRIGHT:
        case meta_event_type::TRACK_NAME:
        case meta_event_type::INSTRUMENT:
        case meta_event_type::LYRIC:
        case meta_event_type::MARKER:
        case meta_event_type::CUE:
        case meta_event_type::PATCH_NAME:
        case meta_event_type::DEVICE_NAME:
        {
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }

        case meta_event_type::END_OF_TRACK:
        {
          if (length != 0)
            throw std::invalid_argument("Expected length for END_OF_TRACK event is 0");
          return event;
        }
        case meta_event_type::TEMPO_CHANGE:
        {
          if (length != 3)
            throw std::invalid_argument("Expected length for TEMPO_CHANGE event is 3");
          // event.m.bytes[3] = read_uint24_be(dataStart); // @dimitri TOFIX
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
        case meta_event_type::SMPTE_OFFSET:
        {
          if (length != 5)
            throw std::invalid_argument("Expected length for SMPTE_OFFSET event is 5");
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
        case meta_event_type::TIME_SIGNATURE:
        {
          if (length != 4)
            throw std::invalid_argument("Expected length for TIME_SIGNATURE event is 4");
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
        case meta_event_type::KEY_SIGNATURE:
        {
          if (length != 2)
            throw std::invalid_argument("Expected length for KEY_SIGNATURE event is 2");
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
        case meta_event_type::PROPRIETARY:
        {
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
        case meta_event_type::CHANNEL_PREFIX:
        {
          if (length != 1)
            throw std::invalid_argument("Expected length for CHANNEL_PREFIX event is 1");
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
        case meta_event_type::MIDI_PORT:
        {
          if (length != 1)
            throw std::invalid_argument("Expected length for MIDI_PORT event is 1");
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
        case meta_event_type::UNKNOWN:
        default:
        {
          // Unknown events?
          byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
          return event;
        }
      }
    }

    else if (type == message_type::SYSTEM_EXCLUSIVE)
    {
      int length = byte_reader::read_variable_length(dataStart, dataEnd);
      event.m.bytes = { (uint8_t)type };
      byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
      return event;
    }

    else if (type == message_type::EOX)
    {
      int length = byte_reader::read_variable_length(dataStart, dataEnd);
      byte_reader::read_bytes(event.m.bytes, dataStart, dataEnd, length);
      return event;
    }
    else
    {
      throw std::runtime_error("Unrecognised MIDI event type byte");
    }
  }

  // Channel events
  else
  {
    event.m.bytes.clear();

    // Running status...
    if (((uint8_t)type & 0x80) == 0)
    {
      // Reuse lastEventTypeByte as the event type.
      // eventTypeByte is actually the first parameter
      event.m.bytes.push_back((uint8_t)lastEventTypeByte);
      event.m.bytes.push_back((uint8_t)type);
      type = lastEventTypeByte;
    }
    else
    {
      event.m.bytes.push_back((uint8_t)type);

      byte_reader::ensure_size(dataStart, dataEnd, 1);
      event.m.bytes.push_back((uint8_t)*dataStart++);
      lastEventTypeByte = type;
    }

    switch (message_type((uint8_t)type & 0xF0))
    {
      case message_type::NOTE_OFF:
        byte_reader::ensure_size(dataStart, dataEnd, 1);
        event.m.bytes.push_back(*dataStart++);
        return event;
      case message_type::NOTE_ON:
        byte_reader::ensure_size(dataStart, dataEnd, 1);
        event.m.bytes.push_back(*dataStart++);
        return event;
      case message_type::POLY_PRESSURE:
        byte_reader::ensure_size(dataStart, dataEnd, 1);
        event.m.bytes.push_back(*dataStart++);
        return event;
      case message_type::CONTROL_CHANGE:
        byte_reader::ensure_size(dataStart, dataEnd, 1);
        event.m.bytes.push_back(*dataStart++);
        return event;
      case message_type::PROGRAM_CHANGE:
        return event;
      case message_type::AFTERTOUCH:
        return event;
      case message_type::PITCH_BEND:
        byte_reader::ensure_size(dataStart, dataEnd, 1);
        event.m.bytes.push_back(*dataStart++);
        return event;

      case message_type::TIME_CODE:
        throw std::runtime_error("Unsupported MIDI event type TIME_CODE");
      case message_type::SONG_POS_POINTER:
        throw std::runtime_error("Unsupported MIDI event type SONG_POS_POINTER");
      case message_type::SONG_SELECT:
        throw std::runtime_error("Unsupported MIDI event type SONG_SELECT");
      case message_type::RESERVED1:
        throw std::runtime_error("Unsupported MIDI event type RESERVED1");
      case message_type::RESERVED2:
        throw std::runtime_error("Unsupported MIDI event type RESERVED2");
      case message_type::TUNE_REQUEST:
        throw std::runtime_error("Unsupported MIDI event type TUNE_REQUEST");
      case message_type::EOX:
        throw std::runtime_error("Unsupported MIDI event type EOX");
        // System Realtime Messages :
      case message_type::TIME_CLOCK:
        throw std::runtime_error("Unsupported MIDI event type TIME_CLOCK");
      case message_type::RESERVED3:
        throw std::runtime_error("Unsupported MIDI event type RESERVED3");
      case message_type::START:
        throw std::runtime_error("Unsupported MIDI event type START");
      case message_type::CONTINUE:
        throw std::runtime_error("Unsupported MIDI event type CONTINUE");
      case message_type::STOP:
        throw std::runtime_error("Unsupported MIDI event type STOP");
      case message_type::RESERVED4:
        throw std::runtime_error("Unsupported MIDI event type RESERVED4");
      case message_type::ACTIVE_SENSING:
        throw std::runtime_error("Unsupported MIDI event type ACTIVE_SENSING");
      case message_type::SYSTEM_RESET:
        throw std::runtime_error("Unsupported MIDI event type SYSTEM_RESET");
      case message_type::INVALID:
        throw std::runtime_error("Unsupported MIDI event type INVALID");
      case message_type::SYSTEM_EXCLUSIVE:
        throw std::runtime_error("Unsupported MIDI event type SYSTEM_EXCLUSIVE");
      default:
        throw std::runtime_error("Unsupported MIDI event type");
    }
  }
}

LIBREMIDI_INLINE
reader::reader(bool useAbsolute)
    : ticksPerBeat(480), startingTempo(120), useAbsoluteTicks(useAbsolute)
{
}

LIBREMIDI_INLINE
reader::~reader()
{
}

constexpr int str_to_headerid(const char *str)
{
  return str[0] << 24 | str[1] << 16 | str[2] << 8 | str[3];
}

LIBREMIDI_INLINE
auto reader::parse(const uint8_t* dataPtr, std::size_t size) noexcept -> parse_result
try
{
  using namespace libremidi::util;

  tracks.clear();

  if(size == 0)
  {
    std::cerr << "libremidi::reader: empty buffer passed to parse." << std::endl;
    return parse_result::invalid;
  }

  const uint8_t* const dataEnd = dataPtr + size;

  int headerId = read_checked::read_uint32_be(dataPtr, dataEnd);
  int headerLength = read_checked::read_uint32_be(dataPtr, dataEnd);

  if (headerId != str_to_headerid("MThd") || headerLength != 6)
  {
    std::cerr << "libremidi::reader: couldn't parse header" << std::endl;
    return parse_result::invalid;
  }

  format = read_checked::read_uint16_be(dataPtr, dataEnd); //@tofix format type -> save for later eventually

  int trackCount = read_checked::read_uint16_be(dataPtr, dataEnd);
  int timeDivision = read_checked::read_uint16_be(dataPtr, dataEnd);

  // CBB: deal with the SMPTE style time coding
  // timeDivision is described here http://www.sonicspot.com/guide/midifiles.html
  if (timeDivision & 0x8000)
  {
    std::cerr << "libremidi::reader: found SMPTE time frames (unsupported)" << std::endl;
    // int fps = (timeDivision >> 16) & 0x7f;
    // int ticksPerFrame = timeDivision & 0xff;
    // given beats per second, timeDivision should be derivable.
    return parse_result::invalid;
  }

  startingTempo = 120.0f;             // midi default
  ticksPerBeat = float(timeDivision); // ticks per beat (a beat is defined as a quarter note)

  parse_result result = parse_result::validated;

  for (int i = 0; i < trackCount; ++i)
  {
    midi_track track;

    headerId = read_checked::read_uint32_be(dataPtr, dataEnd);
    headerLength = read_checked::read_uint32_be(dataPtr, dataEnd);

    if (headerId != str_to_headerid("MTrk"))
    {
      std::cerr << "libremidi::reader: couldn't find track header" << std::endl;
      return parse_result::incomplete;
    }

    int64_t available = dataEnd - dataPtr;
    if(available < headerLength)
    {
      std::cerr << "libremidi::reader: not enough data available" << std::endl;
      return parse_result::incomplete;
    }

    track.reserve(headerLength / 3);

    const uint8_t* const trackEnd = dataPtr + headerLength;

    message_type runningEvent = message_type::INVALID;

    int tickCount = 0;

    while (dataPtr < trackEnd)
    {
      auto tick = read_checked::read_variable_length(dataPtr, trackEnd);
      if (useAbsoluteTicks)
      {
        tickCount += tick;
      }
      else
      {
        tickCount = tick;
      }

      try
      {
        track_event ev = parse_event(tickCount, i, dataPtr, trackEnd, runningEvent);

        if(!ev.m.bytes.empty())
        {
          if (!ev.m.is_meta_event())
          {
            runningEvent = message_type(ev.m.bytes[0]);
          }
        }
        else
        {
          std::cerr << "libremidi::reader: could not read event" << std::endl;
          dataPtr = trackEnd;
          result = parse_result::incomplete;
          continue;
        }

        track.push_back(std::move(ev));
      }
      catch (const std::exception& e)
      {
        std::cerr << "libremidi::reader: " << e.what() << std::endl;
        dataPtr = trackEnd;
        result = parse_result::incomplete;
        continue;
      }
    }

    if(result == parse_result::validated)
    {
      if(!validator::validate_track(track))
      {
        result = parse_result::complete;
      }
    }
    tracks.push_back(std::move(track));
  }
  return result;
}
catch(const std::exception& e)
{
  std::cerr << "libremidi::reader: " << e.what() << std::endl;
  return parse_result::invalid;
}

// In ticks
LIBREMIDI_INLINE
double reader::get_end_time()
{
  double totalLength = 0;
  for (const auto& t : tracks)
  {
    double localLength = 0;
    for (const auto& e : t)
      localLength += e.tick;

    if (localLength > totalLength)
      totalLength = localLength;
  }
  return totalLength;
}

LIBREMIDI_INLINE
auto reader::parse(const std::vector<uint8_t>& buffer) noexcept -> parse_result
{
  return parse(buffer.data(), buffer.size());
}

#if defined(LIBREMIDI_HAS_SPAN)
LIBREMIDI_INLINE
auto reader::parse(std::span<uint8_t> buffer) noexcept -> parse_result
{
  return parse(buffer.data(), buffer.size());
}
#endif
}
