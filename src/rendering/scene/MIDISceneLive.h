#ifndef MIDISceneLive_h
#define MIDISceneLive_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIBase.h"
#include "../State.h"
#include "MIDIScene.h"

#include <rtmidi17/rtmidi17.hpp>
#include <map>

class MIDISceneLive : public MIDIScene {

public:

	MIDISceneLive(int port);

	void updateSets(const SetOptions & options);

	~MIDISceneLive();

	void updatesActiveNotes(double time, double speed);

	double duration() const;

	double secondsPerMeasure() const;

	int notesCount() const;

	void print() const;

	static const std::vector<std::string> & availablePorts();

private:

	struct NoteInfos {
		short note;
		short channel;
	};

	static void updateSet(GPUNote & note, int channel, const SetOptions & options);

	std::vector<GPUNote> _notes;
	std::vector<NoteInfos> _notesInfos;
	std::array<int, 128> _activeIds;
	std::array<bool, 128> _activeRecording;
	std::map<float, Pedals> _pedalInfos;

	double _previousTime = 0.0;
	double _maxTime = 0.0;
	double _signature = 4.0/4.0;
	double _secondsPerMeasure;
	int _notesCount = 0;
	int _tempo = 500000;
	SetOptions _currentSetOption;

	static rtmidi::midi_in & shared();

	static rtmidi::midi_in * _sharedMIDIIn;
	static std::vector<std::string> _availablePorts;
	static int _refreshIndex;

};

#endif
