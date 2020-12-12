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
	input.close();

	// Check midi header
	if(buffer.size() < 5 || !(buffer[0] == 'M' && buffer[1] == 'T' && buffer[2] == 'h' && buffer[3] == 'd') || read32(buffer, 4) != 6){
		std::cerr << "[ERROR]: " << filePath << " is not a midi file." << std::endl;
		throw "BadInput";
	}
	
	_format = static_cast<MIDIType>(read16(buffer, 8));
	const uint16_t tracksCount = read16(buffer, 10);

	const std::vector<std::string> formatNames = { "Single track (0)", "Tempo track (1)", "Multiple songs (2)"};

	std::cout << "[INFO]: " << tracksCount << " tracks ";
	std::cout << "(" << formatNames[int(_format)] << ")." << std::endl;

	if(_format == multipleSongs){
		std::cerr << "[ERROR]: " << "Unsupported MIDI file (type 2)." << std::endl;
		throw "Unsupported MIDI type (2)";
	}

	if(tracksCount == 0){
		std::cerr << "[ERROR]: " << "No tracks." << std::endl;
		throw "BadInput";
	}

	bool shouldMerge = false;
	if(_format == singleTrack && tracksCount > 1){
		std::cerr << "[WARNING]: " << "Too many tracks, will merge all tracks." << std::endl;
		shouldMerge = true;
	}

	// Division mode.
	uint16_t division = read16(buffer, 12);
	bool divisionMode = getBit(division, 15);

	if(divisionMode){
		_unitsPerFrame = division & 0xFF;
		const std::vector<float> fpsValues = {24.0f, 25.0f, 29.97f, 30.0f};

		uint16_t fpsIndicator = ((division >> 8) & 0b1100000) >> 5;
		fpsIndicator = (std::min)(fpsIndicator, uint16_t(int(fpsValues.size()) - 1));
		_framesPerSeconds = fpsValues[fpsIndicator];
		std::cout << "[INFO]: " << _unitsPerFrame << " units per frame, " << _framesPerSeconds << " frames per second." << std::endl;
		std::cout << "[WARN]: " << " Division mode is not well supported." << std::endl;
		_unitsPerQuarterNote = 1;
		
	} else {
		// In that case the 15th bit is 0, nothing to do.
		_unitsPerQuarterNote = division;
		std::cout << "[INFO]: " << _unitsPerQuarterNote << " units per quarter note ." << std::endl;
		_unitsPerFrame = 0;
		_framesPerSeconds = 0.0f;
	}

	// Parse tracks.
	size_t pos = 14;
	for(size_t trackId = 0; trackId < tracksCount; ++trackId){
		std::cout << "[INFO]: " << "Reading track " << trackId << "." << std::endl;
		_tracks.emplace_back();
		pos = _tracks.back().readTrack(buffer, pos);
	}

	// Extract tempos and the signature.
	populateTemposAndSignature();

	// Update seconds per measure.
	_secondsPerMeasure = computeMeasureDuration(_tempos[0].tempo, _signature);

	// Convert each track to real notes.
	for(size_t tid = 0; tid < _tracks.size(); ++tid){
		auto & track = _tracks[tid];
		track.extractNotes(_tempos, _unitsPerQuarterNote, (unsigned int)tid);
	}

	// For now, still merge.
	shouldMerge = true;
	if(shouldMerge){
		mergeTracks();
	}

	// Normalize pedal values.
	for(auto & track : _tracks){
		track.normalizePedalVelocity();
	}

	// Compute duration.
	for(const auto & track : _tracks){
		std::vector<MIDINote> notes;
		track.getNotes(notes, NoteType::ALL);
		for(const auto & note : notes){
			_duration = std::max(_duration, note.start + note.duration);
		}
		_count += int(notes.size());
	}
}

void MIDIFile::print() const {
	for(size_t tid = 0; tid < _tracks.size(); ++tid){
		std::cout << "[INFO]: ---- Track " << tid << std::endl;
		_tracks[tid].print();
	}
}

void MIDIFile::populateTemposAndSignature(){
	std::vector<MIDITempo> mixedTempos;
	const double defaultSign = 4.0/4.0;
	_signature = defaultSign;

	for (auto& track : _tracks) {
		const double trackSign = track.extractTempos(mixedTempos);
		if(trackSign != defaultSign){
			_signature = trackSign;
		}
	}

	// Merge all tempos.
	std::map<size_t, MIDITempo> tempoChanges;
	// Emplace default tempo, will be overwritten as soon as there is an initial tempo event.
	tempoChanges[0] = MIDITempo(0, 500000);
	for(const auto & tempo : mixedTempos){
		tempoChanges[tempo.start] = tempo;
	}
	for(const auto & tempo : tempoChanges){
		_tempos.push_back(tempo.second);
	}
	std::sort(_tempos.begin(), _tempos.end(), [](const MIDITempo& a, const MIDITempo& b){
		return a.start < b.start;
	});

	// Compute the real time stamp of each tempo.
	// We are guaranteed that there is an event at t = 0.
	double currentTime = 0.0;
	_tempos[0].timestamp = 0.0;
	for(size_t tid = 1; tid < _tempos.size(); ++tid){
		const int delta = int(_tempos[tid].start) - int(_tempos[tid-1].start);
		currentTime += computeUnitsDuration(_tempos[tid-1].tempo, delta, _unitsPerQuarterNote);
		_tempos[tid].timestamp = currentTime;
	}
}

void MIDIFile::mergeTracks(){
	
	for(size_t i = 1; i < _tracks.size(); ++i){
		_tracks[0].merge(_tracks[i]);
	}
	_tracks.resize(1);
	
}

void MIDIFile::getNotes(std::vector<MIDINote> & notes, NoteType type, size_t track) const {
	if(track >= _tracks.size()){
		return;
	}
	_tracks[track].getNotes(notes, type);
}

void MIDIFile::getNotesActive(ActiveNotesArray & actives, double time, size_t track) const {
	if(track >= _tracks.size()){
		return;
	}
	_tracks[track].getNotesActive(actives, time);
}

void MIDIFile::getPedalsActive(float & damper, float &sostenuto, float &soft, float &expression, double time, size_t track) const {
	if(track >= _tracks.size()){
		return;
	}
	_tracks[track].getPedalsActive(damper, sostenuto, soft, expression, time);
}

void MIDIFile::updateSets(const SetOptions & options){
	for(auto & track : _tracks){
		track.updateSets(options);
	}
}
