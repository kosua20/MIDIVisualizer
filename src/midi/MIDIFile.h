#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "MIDIUtils.h"
#include "MIDITrack.h"

#ifdef _WIN32
typedef std::wstring pathstring;
#else
typedef std::string pathstring;
#endif

class MIDIFile {
	
private:
	
	uint16_t _tracksCount;
	MIDIType format;
	uint16_t unitsPerFrame;
	float framesPerSeconds;
	uint16_t unitsPerQuarterNote;
	
public:
	
	MIDIFile();
	
	MIDIFile(const pathstring & filePath);
	
	~MIDIFile();
	
	void printTracks();
	
	void mergeTracks();
	
	void getNotesActive(std::vector<int>& actives, double time, size_t track);
	
	void getNotesActiveFull(std::vector<std::pair<double,double>>& actives, double time, size_t track);
	
	std::vector<MIDITrack> tracks;
};

#endif // MIDI_FILE_H
