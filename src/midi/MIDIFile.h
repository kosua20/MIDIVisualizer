#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "MIDIUtils.h"
#include "MIDITrack.h"

struct ActiveNoteInfos {
	float start = 1000000.0f;
	float duration = 0.0f;
	bool enabled = false;
};

class MIDIFile {
	
private:
	
	uint16_t _tracksCount = 0;
	MIDIType format = MIDIType::singleTrack;
	uint16_t unitsPerFrame = 1;
	float framesPerSeconds = 1;
	uint16_t unitsPerQuarterNote = 1;
	
public:
	
	MIDIFile();
	
	MIDIFile(const std::string & filePath);
	
	~MIDIFile();
	
	void printTracks() const;
	
	void mergeTracks();
	
	void getNotesActive(std::vector<ActiveNoteInfos>& actives, double time, size_t track);
	
	std::vector<MIDITrack> tracks;
};

#endif // MIDI_FILE_H
