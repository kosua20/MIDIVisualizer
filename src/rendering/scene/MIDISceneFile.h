#ifndef MIDISceneFile_h
#define MIDISceneFile_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"
#include "../State.h"
#include "MIDIScene.h"

class MIDISceneFile : public MIDIScene {

public:

	MIDISceneFile(const std::string & midiFilePath, const SetOptions & options);

	void updateSets(const SetOptions & options);

	~MIDISceneFile();

	void updatesActiveNotes(double time, double speed);

	double duration() const;

	double secondsPerMeasure() const;

	int notesCount() const;

	void print() const;

private:

	MIDIFile _midiFile;
	double _previousTime = 0.0;
	
};

#endif
