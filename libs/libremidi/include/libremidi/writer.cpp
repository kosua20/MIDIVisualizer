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
#  include <libremidi/writer.hpp>
#endif
#include <ostream>
#include <numeric>
#include <string>

namespace libremidi
{
namespace util
{
static LIBREMIDI_INLINE
std::ostream& write_uint16_be(std::ostream& out, uint16_t value)
{
  union
  {
    uint8_t bytes[2];
    uint16_t v;
  } data;
  data.v = value;
  out << data.bytes[1];
  out << data.bytes[0];
  return out;
}

static LIBREMIDI_INLINE
std::ostream& write_int16_be(std::ostream& out, int16_t value)
{
  union
  {
    uint8_t bytes[2];
    int16_t v;
  } data;
  data.v = value;
  out << data.bytes[1];
  out << data.bytes[0];
  return out;
}

static LIBREMIDI_INLINE
std::ostream& write_uint32_be(std::ostream& out, uint32_t value)
{
  union
  {
    uint8_t bytes[4];
    uint32_t v;
  } data;
  data.v = value;
  out << data.bytes[3];
  out << data.bytes[2];
  out << data.bytes[1];
  out << data.bytes[0];
  return out;
}

static LIBREMIDI_INLINE
std::ostream& write_int32_be(std::ostream& out, int32_t value)
{
  union
  {
    uint8_t bytes[4];
    int32_t v;
  } data;
  data.v = value;
  out << data.bytes[3];
  out << data.bytes[2];
  out << data.bytes[1];
  out << data.bytes[0];
  return out;
}

static LIBREMIDI_INLINE
std::ostream& write_float_be(std::ostream& out, float value)
{
  union
  {
    uint8_t bytes[4];
    float v;
  } data;
  data.v = value;
  out << data.bytes[3];
  out << data.bytes[2];
  out << data.bytes[1];
  out << data.bytes[0];
  return out;
}

static LIBREMIDI_INLINE
std::ostream& write_double_be(std::ostream& out, double value)
{
  union
  {
    uint8_t bytes[8];
    double v;
  } data;
  data.v = value;
  out << data.bytes[7];
  out << data.bytes[6];
  out << data.bytes[5];
  out << data.bytes[4];
  out << data.bytes[3];
  out << data.bytes[2];
  out << data.bytes[1];
  out << data.bytes[0];
  return out;
}

// Write a number to the midifile
// as a variable length value which segments a file into 7-bit
// values.  Maximum size of aValue is 0x7fffffff
static LIBREMIDI_INLINE
void write_variable_length(uint32_t aValue, std::vector<uint8_t>& outdata)
{
  uint8_t bytes[5] = {0};

  bytes[0] = (uint8_t)(((uint32_t)aValue >> 28) & 0x7F); // most significant 5 bits
  bytes[1] = (uint8_t)(((uint32_t)aValue >> 21) & 0x7F); // next largest 7 bits
  bytes[2] = (uint8_t)(((uint32_t)aValue >> 14) & 0x7F);
  bytes[3] = (uint8_t)(((uint32_t)aValue >> 7) & 0x7F);
  bytes[4] = (uint8_t)(((uint32_t)aValue) & 0x7F); // least significant 7 bits

  int start = 0;
  while (start < 5 && bytes[start] == 0)
    start++;

  for (int i = start; i < 4; i++)
  {
    bytes[i] = bytes[i] | 0x80;
    outdata.push_back(bytes[i]);
  }
  outdata.push_back(bytes[4]);
}

static LIBREMIDI_INLINE
void add_event_track_count_check(std::vector<midi_track>& tracks, int track)
{
  if(track < 0)
    throw std::out_of_range("Refusing to add an event to track " + std::to_string(track) + ".");
  if(track > 65535)
    throw std::out_of_range("Refusing to add an event to track " + std::to_string(track) + " ; change add_event_track_count_check in libremidi writer.cpp to increase the limit.");

  while(tracks.size() < track + 1)
    tracks.push_back({});
}
}

LIBREMIDI_INLINE
void writer::add_event(int tick, int track, message m)
{
  util::add_event_track_count_check(tracks, track);

  tracks[track].push_back({tick, track, m});
}

LIBREMIDI_INLINE
void writer::add_event(int track, track_event m)
{
  util::add_event_track_count_check(tracks, track);
    
  tracks[track].push_back(m);
}

LIBREMIDI_INLINE
void writer::add_track()
{
  util::add_event_track_count_check(tracks, tracks.size() + 1);
}


LIBREMIDI_INLINE
void writer::write(std::ostream& out)
{
  // MIDI File Header
  out << 'M';
  out << 'T';
  out << 'h';
  out << 'd';
  util::write_uint32_be(out, 6);
  util::write_uint16_be(out, (tracks.size() == 1) ? 0 : 1);
  util::write_uint16_be(out, static_cast<uint16_t>(tracks.size()));
  util::write_uint16_be(out, ticksPerQuarterNote);

  std::vector<uint8_t> trackRawData;
  // Rough estimation of the memory to allocate
  const int num_events = std::accumulate(tracks.begin(), tracks.end(), 0, [] (int lhs, const auto& rhs) { return lhs + rhs.size(); });
  trackRawData.reserve(num_events * 3);

  for (const auto& event_list : tracks)
  {
    for (const auto& event : event_list)
    {
      const auto& msg = event.m;
      if(msg.bytes.empty())
        continue;

      // Suppress end-of-track meta messages (one will be added
      // automatically after all track data has been written).
      if (msg.get_meta_event_type() == meta_event_type::END_OF_TRACK)
        continue;

      util::write_variable_length(event.tick, trackRawData);

      if ((msg.get_message_type() == message_type::SYSTEM_EXCLUSIVE)
          || (event.m.get_message_type() == message_type::EOX))
      {
        // 0xf0 == Complete sysex message (0xf0 is part of the raw MIDI).
        // 0xf7 == Raw byte message (0xf7 not part of the raw MIDI).
        // Print the first byte of the message (0xf0 or 0xf7), then
        // print a VLV length for the rest of the bytes in the message.
        // In other words, when creating a 0xf0 or 0xf7 MIDI message,
        // do not insert the VLV byte length yourself, as this code will
        // do it for you automatically.
        trackRawData.emplace_back(msg.bytes[0]); // 0xf0 or 0xf7;

        util::write_variable_length(static_cast<uint32_t>(msg.size()) - 1, trackRawData);

        for (size_t k = 1; k < msg.size(); k++)
        {
          trackRawData.emplace_back(msg[static_cast<int>(k)]);
        }
      }
      else
      {
        // Non-sysex type of message, so just output the bytes of the message:
        for (size_t k = 0; k < msg.size(); k++)
        {
          trackRawData.emplace_back(msg[static_cast<int>(k)]);
        }
      }
    }
  }

  auto size = trackRawData.size();
  const auto eot = meta_events::end_of_track();

  if ((size < 3) || !((trackRawData[size - 3] == 0xFF) && (trackRawData[size - 2] == 0x2F)))
  {
    trackRawData.emplace_back(0x0); // tick
    trackRawData.emplace_back(eot[0]);
    trackRawData.emplace_back(eot[1]);
    trackRawData.emplace_back(eot[2]);
  }

  // Write the track ID marker "MTrk":
  out << 'M';
  out << 'T';
  out << 'r';
  out << 'k';
  util::write_uint32_be(out, static_cast<uint32_t>(trackRawData.size()));
  out.write((char*)trackRawData.data(), trackRawData.size());
}
}
