#include <tuple>
#include <cmath>
#include <algorithm>
#include "MIDITrack.h"

MIDIEvent MIDITrack::readMIDIEvent(const std::vector<char> & buffer, size_t & position, size_t delta){
	
	uint8_t firstByte = read8(buffer, position);
	uint8_t secondByte = read8(buffer, position+1);
	uint8_t thirdByte = read8(buffer, position+2);
	
	size_t positionOffset = 3;
	
	MIDIEventType type = static_cast<MIDIEventType>((firstByte & 0xF0) >> 4);
	if(MIDIEventTypeName.count(type) == 0){
		thirdByte = secondByte;
		secondByte = firstByte;
		firstByte = _previousEventFirstByte;
		positionOffset = 2;
	}
	
	type = static_cast<MIDIEventType>((firstByte & 0xF0) >> 4);
	short channel = firstByte & 0x0F;
	
	short note = short(secondByte);
	short velocity = short(thirdByte);
	
	std::vector<short> data;
	data.push_back(channel);
	data.push_back(note);
	data.push_back(velocity);
	
	_previousEventFirstByte = firstByte;
	position += positionOffset;
	
	return MIDIEvent(midiEvent, static_cast<uint8_t>(type), delta, data);
}


MIDIEvent MIDITrack::readMetaEvent(const std::vector<char> & buffer, size_t & position, size_t delta){
	position += 1; // We already read FF.
	MetaEventType type = static_cast<MetaEventType>(read8(buffer, position));
	position += 1;
	
	size_t length = readVarLen(buffer,position);
	
	std::vector<short> data;
	for(size_t i = 0; i < length; ++i){
		data.push_back(short(read8(buffer, position + i)));
	}
	
	position = position + length;
	
	return MIDIEvent(metaEvent, static_cast<uint8_t>(type), delta, data);
}


MIDIEvent MIDITrack::readSysexEvent(const std::vector<char> & buffer, size_t & position, size_t delta){
	uint8_t type = read8(buffer, position);
	position += 1;
	
	size_t length = readVarLen(buffer,position);
	
	std::vector<short> data;
	for(size_t i = 0; i < length; ++i){
		data.push_back(short(read8(buffer, position + i)));
	}
	
	position = position + length;
	
	return MIDIEvent(systemEvent, type, delta, data);
}


size_t MIDITrack::readTrack(const std::vector<char>& buffer, size_t pos, uint16_t unitsPerQuarterNote){
	const size_t backupPos = pos;
	
	//Check header
	if( !(buffer[pos] == 'M' && buffer[pos+1] == 'T' && buffer[pos+2] == 'r' && buffer[pos+3] == 'k')){
		std::cerr << "[ERROR]: Missing track." << std::endl;
		return 3;
	}
	pos += 4;
	uint32_t length = read32(buffer, pos);
	pos += 4;
	
	
	if(length == 0){
		std::cerr << "[ERROR]: Empty track." << std::endl;
		return 3;
	}
	size_t count = 0;
	while(pos < backupPos + 8 + length){
		
		size_t delta = readVarLen(buffer,pos);
		
		uint8_t eventMetaType = read8(buffer, pos);
		
		if(eventMetaType == 0xFF){
			events.push_back(readMetaEvent(buffer,pos, delta));
		} else if (eventMetaType == 0xF0 || eventMetaType == 0xF7){
			events.push_back(readSysexEvent(buffer, pos, delta));
		}  else {
			events.push_back(readMIDIEvent(buffer, pos, delta));
		}
		
		count++;
	}
	
	
	// Scan events for track info.
	// Could do it while creating events, but let's separate tasks, shall we?
	int tempo = 0x555555;
	bool minorKey = false;
	short keyShift = 0;
	_timeSignature = 4.0/4.0;
	
	for(auto& event : events){
		if(event.category == metaEvent){
			if(event.type == sequenceName){
				std::string tempName = "";
				for(size_t i = 0; i < event.data.size(); ++i){
					tempName += char(event.data[i]);
				}
				name = tempName;
			} else if(event.type == instrumentName){
				std::string tempName = "";
				for(size_t i = 0; i < event.data.size(); ++i){
					tempName += char(event.data[i]);
				}
				instrument = tempName;
			} else if (event.type == setTempo){
				tempo = (event.data[0] & 0xFF) << 16 | (event.data[1] & 0xFF) << 8 | (event.data[0] & 0xFF);
			} else if (event.type == timeSignature){
				_timeSignature = double(event.data[0]) / double(std::pow(2,event.data[1]));
			} else if (event.type == keySignature){
				keyShift = event.data[0];
				minorKey = (event.data[1] > 0);
			}
		}
	}
	
	// micro s / delta =  (micros/qnote) / (delta/qnote) = tempo / unitsPerQuarterNote
	_unitsPerQuarterNote = double(unitsPerQuarterNote);
	_unitTime = double(tempo) / _unitsPerQuarterNote;
	
	
	secondsPerMeasure = computeMeasureDuration(tempo);
	
	std::cout << "[INFO]: Track " << name << " (length: " << length << ", instrument: " << instrument <<", tempo: " << tempo << ", " << (minorKey ? "minor": "major") << ", measure duration: " << secondsPerMeasure << ")." << std::endl;
	
	return backupPos + 8 + length;
}

double MIDITrack::computeMeasureDuration(int tempo){
	// micro s / measure = (micro s / qnote) * (clock / qnote)^-1 * (clock / beat) * (beat / measure)
	//					 = tempo * 24^-1 * cc * time signature
	return double(tempo) * 4.0 * _timeSignature / 1000000.0;
}

void MIDITrack::extractNotes(short lowerBound, short higherBound, bool normalize){
	notes.clear();
	
	std::map<short, std::tuple<double, short, short>> currentNotes;
	double timeInUnits = 0;
	
	for(auto& event : events){
		
		if(event.category == midiEvent){
			timeInUnits += double(event.delta);
			if(event.type == noteOn && event.data[2] > 0){
				if(event.data[1]>= lowerBound && event.data[1] <= higherBound){
					currentNotes[event.data[1] - (normalize ? lowerBound : 0)] = std::make_tuple(timeInUnits, event.data[2], event.data[0]);
				}
			} else if(event.type == noteOff || (event.type == noteOn && event.data[2] == 0)){
				short noteInd = event.data[1] - (normalize ? lowerBound : 0);
				if(currentNotes.count(noteInd) > 0){
					
					std::tuple<double, short, short> noteTuple = currentNotes[noteInd];
					
					double start = _unitTime*std::get<0>(noteTuple);
					double duration = _unitTime * timeInUnits - start;
					
					MIDINote note(noteInd, start/1000000.0f, duration/1000000.0f, std::get<1>(noteTuple), std::get<2>(noteTuple));
					notes.push_back(note);
					currentNotes.erase(noteInd);
				}
			} else if(event.type == programChange){
				// It seems those should be ignored.
				timeInUnits -= double(event.delta);
			} else {
				//std::cout << "Unknown midi event: " << int(event.type) << "," << double(event.delta) << std::endl;
			}
		} else if(event.category == metaEvent && event.type == setTempo){
			int tempo = (event.data[0] & 0xFF) << 16 | (event.data[1] & 0xFF) << 8 | (event.data[0] & 0xFF);
			_unitTime = double(tempo) / _unitsPerQuarterNote;
			
			secondsPerMeasure = computeMeasureDuration(tempo);
			//std::cout << "[INFO]: Tempo changes detected." << double(event.delta) << std::endl;
		} else if(event.category == metaEvent && event.type == timeSignature){
			secondsPerMeasure /= _timeSignature;
			_timeSignature = double(event.data[0]) / double(std::pow(2,event.data[1]));
			secondsPerMeasure *= _timeSignature;
			//std::cout << "[INFO]: Time signature changes detected." << double(event.delta) << std::endl;
		} else {
			//std::cout << "Unknown event: " << double(event.delta) << std::endl;
		}
	}
	
	// Sort.
	std::sort(notes.begin(), notes.end(), [](const MIDINote & note1, const MIDINote & note2) { return(note1.start < note2.start); } );
	
	std::cout << "[INFO]: " << notes.size() << " notes extracted and sorted." << std::endl;
}

std::vector<MIDINote> MIDITrack::getNotes(NoteType type){
	
	std::vector<MIDINote> selected;
	if(type == minorNotes){
		for(auto& note : notes){
			if(noteIsMinor[note.note]){
				selected.push_back(note);
				selected.back().note = noteShift[note.note];
			}
		}
	} else if (type == majorNotes){
		for(auto& note : notes){
			if(!noteIsMinor[note.note]){
				selected.push_back(note);
				selected.back().note = noteShift[note.note];
			}
		}
	} else {
		for(auto& note : notes){
			selected.push_back(note);
			selected.back().note = noteShift[note.note];
		}
	}
	
	return selected;
}

void MIDITrack::printNotes(){
	for(auto& note : notes){
		std::cout << "Note " << note.note << "(" << note.duration << "s at "<< note.start << "s)" << " on channel " << note.channel << " with velocity " << note.velocity << "." << std::endl;
	}
}

	
void MIDITrack::printEvents(){
	for(auto& event : events){
		if(event.category == systemEvent){
			std::cout << "[INFO]: " << "Sysex event (" << event.delta << "): type is "<< std::hex << std::showbase << event.type << std::dec << ", length is " << event.data.size() << std::endl;
		} else if (event.category == metaEvent){
			std::cout << "[INFO]: " << "Meta event (" << event.delta << "): type is " << metaEventTypeName[static_cast<MetaEventType>(event.type)] << ", length is " << event.data.size() << std::endl;
		} else if (event.category == midiEvent){
			std::cout << "[INFO]: " << "MIDI Event " << (MIDIEventTypeName.count(static_cast<MIDIEventType>(event.type)) ? MIDIEventTypeName[static_cast<MIDIEventType>(event.type)] : "unknown") << " (" << event.delta << ") on channel " << event.data[0] << " with note " << event.data[1] << " and velocity " << event.data[2] << "." << std::endl;
		}
	}
}
