#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include "MIDIUtils.h"
#include "MIDIBase.h"
#include "MIDITrack.h"

class MIDIFile {

public:
	
	MIDIFile();
	
	MIDIFile(const std::string & filePath);

	void updateSets(const SetOptions & options);

	void print() const;

	void getNotes(std::vector<MIDINote>& notes, NoteType type, size_t track) const;
	
	void getNotesActive(ActiveNotesArray& actives, double time, size_t track) const;

	void getPedalsActive(float &damper, float &sostenuto, float &soft, float &expression, double time, size_t track) const;

	const double & signature() const { return _signature; }
	
	const double & secondsPerMeasure() const { return _secondsPerMeasure; }

	const double & duration() const { return _duration; }

	const int & notesCount() const { return _count; }

private:

	void populateTemposAndSignature();

	void mergeTracks();

	MIDIType _format = MIDIType::singleTrack;
	uint16_t _unitsPerFrame = 1;
	float _framesPerSeconds = 1;
	uint16_t _unitsPerQuarterNote = 1;
	double _signature = 4.0/4.0;
	double _secondsPerMeasure = 1.0;
	double _duration = 0.0;
	int _count = 0;

	std::vector<MIDITrack> _tracks;
	std::vector<MIDITempo> _tempos;

};

#endif // MIDI_FILE_H
