#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "MIDIUtils.h"
#include "MIDITrack.h"


class MIDIFile {
	
private:
	
	uint16_t _tracksCount;
	MIDIType format;
	uint16_t unitsPerFrame;
	float framesPerSeconds;
	uint16_t unitsPerQuarterNote;
	
public:
	
	MIDIFile();
	
	MIDIFile(const std::string & filePath);
	
	~MIDIFile();
	
	void printTracks();
	
	void mergeTracks();
	
	void getNotesActive(std::vector<int>& actives, double time, size_t track);
	
	void getNotesActiveFull(std::vector<std::pair<double,double>>& actives, double time, size_t track);
	
	std::vector<MIDITrack> tracks;
};

#endif // MIDI_FILE_H
