#include <algorithm>
#include <iterator>
#include <fstream>

#include "MIDIFile.h"

MIDIFile::MIDIFile(){};

MIDIFile::MIDIFile(const std::string & filePath){
	
	std::ifstream input(filePath, std::ios::in|std::ios::binary);
	if(!input.is_open()) {
		std::cerr << "[ERROR]: Couldn't find file at path " << filePath << std::endl;
		throw "BadInput";
	}
	
	std::vector<char> buffer;
	std::copy(std::istreambuf_iterator<char>(input),
			  std::istreambuf_iterator<char>(),
			  std::back_inserter(buffer));
	
	
	// Check midi header
	if(buffer.size() < 5 || !(buffer[0] == 'M' && buffer[1] == 'T' && buffer[2] == 'h' && buffer[3] == 'd') || read32(buffer, 4) != 6){
		std::cerr << "[ERROR]: " << filePath << " is not a midi file." << std::endl;
		throw "BadInput";
	}
	
	format = static_cast<MIDIType>(read16(buffer, 8));
	_tracksCount = read16(buffer, 10);
	
	std::cout << "[INFO]: " << _tracksCount << " tracks ";
	std::cout << "(" << (format==tempoTrack ? "Use tempo (1)" : ( format == singleTrack ? "Single track (0)" : "Multiple songs (2)")) << ")." << std::endl;
	
	// Division mode.
	uint16_t division = read16(buffer, 12);
	bool divisionMode = getBit(division, 15);
	
	
	if(divisionMode){
		unitsPerFrame = division & 0xFF;
		uint16_t framesPerSecondsIndicator = ((division >> 8) & 0b1100000) >> 5;
		framesPerSeconds = (framesPerSecondsIndicator == 0 ? 24.0f : (framesPerSecondsIndicator == 1 ? 25.0f : (framesPerSecondsIndicator == 2 ? 29.97f : 30.0f)));
		std::cout << "[INFO]: " << unitsPerFrame << " units per frame, " << framesPerSeconds << " frames per second." << std::endl;
		
		unitsPerQuarterNote = 0;
		
	} else {
		// Remove 15th bit.
		unitsPerQuarterNote = division ^ ( division & (0x1 << 15));
		std::cout << "[INFO]: " << unitsPerQuarterNote << " units per quarter note ." << std::endl;
		
		unitsPerFrame = 0;
		framesPerSeconds = 0.0f;
	}
	
	if(_tracksCount == 0){
		std::cerr << "[ERROR]: " << "No tracks." << std::endl;
		throw "BadInput";
	}
	if(format==multipleSongs){
		std::cerr << "[ERROR]: " << "Nooop. Lazy dev." << std::endl;
		throw "LazyDev";
	}
	

	
	size_t pos = 14;
	
	for(size_t trackId = 0; trackId < _tracksCount; ++trackId){
		std::cout << "[INFO]: " << "Reading track " << trackId << "." << std::endl;
		tracks.emplace_back();
		pos = tracks.back().readTrack(buffer, pos, unitsPerQuarterNote);
	}
	
	for(auto& track : tracks){
		track.extractNotes(21,108, true);
	}
	mergeTracks();

	// Normalize duration.
	/*const float firstNoteStart = tracks[0].notes[0].start;
	for (size_t i = 0; i < tracks[0].notes.size(); ++i) {
		tracks[0].notes[i].start -= firstNoteStart;
	}*/

	input.close();
	
	
}

MIDIFile::~MIDIFile(){
	
}


void MIDIFile::printTracks(){
	for(auto& track : tracks){
		track.printEvents();
	}
}

void MIDIFile::mergeTracks(){
	
	for(size_t i = 1; i < tracks.size(); ++i){
		for(auto& event : tracks[i].events){
			tracks[0].events.push_back(event);
		}
		
		for(auto& note : tracks[i].notes){
			tracks[0].notes.push_back(note);
		}
		//might want to move it out of the loop
		std::sort(tracks[0].notes.begin(), tracks[0].notes.end(), [](const MIDINote & note1, const MIDINote & note2) { return(note1.start < note2.start); } );
		
	}

	
}

void MIDIFile::getNotesActive(std::vector<int> & actives, double time, size_t track){
	actives.resize(88);
	size_t count = tracks[track].notes.size();
	for(size_t i = 0; i < count;++i){
		auto& note = tracks[track].notes[i];
		if(note.start <= time && note.start+note.duration >= time){
			actives[note.note] = 1;
		}
	}
}

void MIDIFile::getNotesActiveFull(std::vector<std::pair<double,double>> & actives, double time, size_t track){
	for(size_t j = 0; j < 88; ++j){
		actives[j] = std::make_pair(-1000.0,0.0);
	}
	size_t count = tracks[track].notes.size();
	for(size_t i = 0; i < count;++i){
		auto& note = tracks[track].notes[i];
		double newDuration = std::max(note.duration*2.0, note.duration+1.2);
		if(note.start-0.25 <= time && note.start+newDuration >= time){
			actives[note.note] = std::make_pair((time-note.start+0.25)/newDuration, newDuration);
		} 
	}
}





