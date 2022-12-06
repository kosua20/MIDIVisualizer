#ifndef MIDISceneLive_h
#define MIDISceneLive_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIBase.h"
#include "../State.h"
#include "MIDIScene.h"

#include <libremidi/libremidi.hpp>
#include <map>

#define VIRTUAL_DEVICE_NAME "VIRTUAL"

class MIDISceneLive : public MIDIScene {

public:

	MIDISceneLive(int port, bool verbose);

	void updateSets(const SetOptions & options);

	~MIDISceneLive();

	void updatesActiveNotes(double time, double speed);

	double duration() const;

	double secondsPerMeasure() const;

	int notesCount() const;

	void print() const;

	void save(std::ofstream& file) const;

	const std::string& deviceName() const;

	static const std::vector<std::string> & availablePorts(bool force = false);
	
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
	std::vector<libremidi::message> _allMessages;

	double _previousTime = 0.0;
	double _maxTime = 0.0;
	double _signatureNum = 4.0;
	double _signatureDenom = 4.0;
	double _secondsPerMeasure = 1.0;
	double _lastMessageTime = 0.0;

	int _notesCount = 0;
	int _tempo = 500000;
	SetOptions _currentSetOption;
	std::string _deviceName;
	bool _verbose = false;

	static libremidi::midi_in & shared();

	static libremidi::midi_in * _sharedMIDIIn;
	static std::vector<std::string> _availablePorts;
	static int _refreshIndex;

};

#endif
