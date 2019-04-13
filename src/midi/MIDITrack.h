#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include "MIDIUtils.h"

struct MIDINote {
	
	MIDINote(short aNote, double aStart, double aDuration, short aVelocity, short aChannel) : start(aStart), duration(aDuration), note(aNote), velocity(aVelocity), channel(aChannel) { }
	
	double start;
	double duration;
	short note;
	short velocity;
	short channel;
	
};

struct MIDIEvent {
	
	MIDIEvent(EventClass aCategory, uint8_t aType, size_t aDelta, std::vector<short> aData) : category(aCategory), type(aType), delta(aDelta), data(aData) { }
	
	EventClass category;
	uint8_t type;
	size_t delta;
	std::vector<short> data;
	
};

enum NoteType{
	majorNotes, minorNotes, both
};


class MIDITrack {

private:
	
	uint8_t _previousEventFirstByte = 0x0;
	double _unitsPerQuarterNote;
	double _unitTime;
	double _timeSignature;
	int _tempo;
	
	MIDIEvent readMIDIEvent(const std::vector<char> & buffer, size_t & position, size_t delta);
	MIDIEvent readMetaEvent(const std::vector<char> & buffer, size_t & position, size_t delta);
	MIDIEvent readSysexEvent(const std::vector<char> & buffer, size_t & position, size_t delta);
	
	double computeMeasureDuration(int tempo);
	
public:
	
	size_t readTrack(const std::vector<char>& buffer, size_t pos, uint16_t unitsPerQuarterNote);
	
	void extractNotes(short lowerBound, short higherBound, bool normalize);
	
	void updateMetrics(const int tempo, const double signature);

	int getTempo() const { return _tempo; }
	double getSignature() const { return _timeSignature;  }

	std::vector<MIDINote> getNotes(NoteType type);
	
	bool containsNotes();

	void printNotes() const;
	void printEvents() const;
	
	std::vector<MIDINote> notes;
	std::vector<MIDIEvent> events;
	std::string name;
	std::string instrument;
	double secondsPerMeasure;
	bool hasTempo;
	
};

#endif // MIDI_TRACK_H
