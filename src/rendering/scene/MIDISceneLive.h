#ifndef MIDISceneLive_h
#define MIDISceneLive_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIBase.h"
#include "../State.h"
#include "MIDIScene.h"

#include <rtmidi17/rtmidi17.hpp>

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

	std::vector<GPUNote> _notes;
	std::array<int, 128> _activeIds;
	double _previousTime = 0.0;
	double _signature = 4.0/4.0;
	double _secondsPerMeasure;
	int _notesCount = 0;
	int _tempo = 500000;

	static rtmidi::midi_in & shared();

	static rtmidi::midi_in * _sharedMIDIIn;
	static std::vector<std::string> _availablePorts;
	static int _refreshIndex;

};

#endif
