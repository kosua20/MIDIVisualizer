#ifndef MIDI_TRACK_H
#define MIDI_TRACK_H

#include "MIDIBase.h"

class MIDITrack {
public:
	
	size_t readTrack(const std::vector<char>& buffer, size_t pos);
	
	double extractTempos(std::vector<MIDITempo> & tempos) const;

	void extractNotes(const std::vector<MIDITempo> & tempos, uint16_t unitsPerQuarterNote, short minId, short maxId, unsigned int trackId);

	void print() const;

	void getNotes(std::vector<MIDINote> & notes, NoteType type) const;

	void getNotesActive(std::vector<ActiveNoteInfos>& actives, double time) const;

	void merge(MIDITrack & other);

private:

	std::pair<double, double> computeNoteTimings(const std::vector<MIDITempo> & tempos, size_t start,size_t end, uint16_t upqn) const;

	std::vector<MIDIEvent> _events;
	std::vector<MIDINote> _notes;
	std::string _name;
	std::string _instrument;
	uint8_t _previousEventFirstByte = 0x0;

};

#endif // MIDI_TRACK_H
