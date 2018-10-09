#include <algorithm>
#include <iterator>
#include <fstream>

#include "MIDIFile.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>

WCHAR * widen(const std::string & str){
	const int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	WCHAR *arr = new WCHAR[size];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)arr, size);
	// Will leak on Windows.
	/// \todo Stop leaking.
	return arr;
}

std::string narrow(WCHAR * str){
	const int size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	std::string res(size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, &res[0], size, NULL, NULL);
	return res;
}

#else

const char * widen(const std::string & str){
	return str.c_str();
}
std::string narrow(char * str) {
	return std::string(str);
}

#endif

MIDIFile::MIDIFile(){};

MIDIFile::MIDIFile(const std::string & filePath){
	std::ifstream input(widen(filePath), std::ios::in|std::ios::binary);

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
	if (format == singleTrack) {
		for (auto& track : tracks) {
			track.extractNotes(21, 108, true);
		}
	} else if (format == tempoTrack) {
		// There should be at least one track specifying the tempo, and one note track.
		int tid = 0;
		while(tid < _tracksCount && !tracks[tid].hasTempo) {
			++tid;
		}
		int forcedTempo = tracks[tid].getTempo();
		int forcedSignature = tracks[tid].getSignature();
		// The tid track has a tempo !
		for (auto& track : tracks) {
			track.updateMetrics(forcedTempo, forcedSignature);
			track.extractNotes(21, 108, true);
		}
	}
	mergeTracks();

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
	}
	
	std::sort(tracks[0].notes.begin(), tracks[0].notes.end(), [](const MIDINote & note1, const MIDINote & note2) { return(note1.start < note2.start); } );

	
}

void MIDIFile::getNotesActive(std::vector<ActiveNoteInfos> & actives, double time, size_t track){
	// actives.resize(88);
	// Reset all notes.
	for(int i = 0; i < actives.size(); ++i){
		 actives[i].enabled = false;
	}
	size_t count = tracks[track].notes.size();
	for(size_t i = 0; i < count;++i){
		auto& note = tracks[track].notes[i];
		if(note.start <= time && note.start+note.duration >= time){
			actives[note.note].enabled = true;
			actives[note.note].duration =  note.duration;
			actives[note.note].start = note.start;
		}
		
	}
}

// void MIDIFile::getNotesActiveFull(std::vector<std::pair<double,double>> & actives, double time, size_t track){
// 	for(size_t j = 0; j < 88; ++j){
// 		actives[j] = std::make_pair(-1000.0,0.0);
// 	}
// 	size_t count = tracks[track].notes.size();
// 	for(size_t i = 0; i < count;++i){
// 		auto& note = tracks[track].notes[i];
// 		double newDuration = std::max(note.duration*2.0, note.duration+1.2);
// 		if(note.start-0.25 <= time && note.start+newDuration >= time){
// 			actives[note.note] = std::make_pair((time-note.start+0.25)/newDuration, newDuration);
// 		} 
// 	}
// }





