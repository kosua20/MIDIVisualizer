#include <libremidi/reader.hpp>
#include <istream>
#include <fstream>
#include <vector>

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    perror("Usage: ./midifile_dump <midifile.mid>");
    return 1;
  }

  // Read raw from a MIDI file
  std::ifstream file{argv[1], std::ios::binary};
  if(!file.is_open())
  {
    std::cerr << "Could not open " << argv[1] << std::endl;
    return 1;
  }

  std::vector<uint8_t> bytes;
  bytes.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

  // Initialize our reader object
  libremidi::reader r{true};

  // Parse
  libremidi::reader::parse_result result = r.parse(bytes);

  switch (result)
  {
    case libremidi::reader::validated:
      std::cout << "\nParsing validated\n\n";
      // Parsing has succeeded, all the input data is correct MIDI.
      break;

    case libremidi::reader::complete:
      std::cout << "\nParsing complete\n\n";
      // All the input data is parsed but the MIDI file was not necessarily strict SMF
      // (e.g. there are empty tracks or tracks without END OF TRACK)
      break;

    case libremidi::reader::incomplete:
      std::cout << "\nParsing incomplete\n\n";
      // Not all the input could be parsed. For instance a track could not be read.
      break;

    case libremidi::reader::invalid:
      std::cout << "\nParsing invalid\n\n";
      // Nothing could be parsed, this is not MIDI data or we do not support it yet.
      return 1;
  }

  if (result != libremidi::reader::invalid)
  {
    for (const auto& track : r.tracks)
    {
      std::cout << "\nNew track\n\n";
      for (const libremidi::track_event& event : track)
      {
        std::cout << "Event at " << event.tick << " : ";
        if(event.m.is_meta_event())
        {
          std::cout << "Meta event";
        }
        else
        {
          switch(event.m.get_message_type())
          {
            case libremidi::message_type::NOTE_ON:
              std::cout << "Note ON: "
                        << "channel "  << event.m.get_channel() << ' '
                        << "note "     << (int)event.m.bytes[1] << ' '
                        << "velocity " << (int)event.m.bytes[2] << ' ';
              break;
            case libremidi::message_type::NOTE_OFF:
              std::cout << "Note OFF: "
                        << "channel "  << event.m.get_channel() << ' '
                        << "note "     << (int)event.m.bytes[1] << ' '
                        << "velocity " << (int)event.m.bytes[2] << ' ';
              break;
            case libremidi::message_type::CONTROL_CHANGE:
              std::cout << "Control: "
                        << "channel "  << event.m.get_channel() << ' '
                        << "control "  << (int)event.m.bytes[1] << ' '
                        << "value "    << (int)event.m.bytes[2] << ' ';
              break;
            case libremidi::message_type::PROGRAM_CHANGE:
              std::cout << "Program: "
                        << "channel "  << event.m.get_channel() << ' '
                        << "program "  << (int)event.m.bytes[1] << ' ';
              break;
            case libremidi::message_type::AFTERTOUCH:
              std::cout << "Aftertouch: "
                        << "channel "  << event.m.get_channel() << ' '
                        << "value "    << (int)event.m.bytes[1] << ' ';
              break;
            case libremidi::message_type::POLY_PRESSURE:
              std::cout << "Poly pressure: "
                        << "channel "  << event.m.get_channel() << ' '
                        << "note "     << (int)event.m.bytes[1] << ' '
                        << "value "    << (int)event.m.bytes[2] << ' ';
              break;
            case libremidi::message_type::PITCH_BEND:
              std::cout << "Poly pressure: "
                        << "channel "  << event.m.get_channel() << ' '
                        << "bend "     << (int)(event.m.bytes[1] << 7 + event.m.bytes[2]) << ' ';
              break;
            default:
              std::cout << "Unsupported.";
             break;
          }
        }
        std::cout << '\n';
      }
    }
  }
}