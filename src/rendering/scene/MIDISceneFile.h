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
	
	void save(std::ofstream& file) const;

	const std::string& filePath() const;

private:

	MIDIFile _midiFile;
	std::string _filePath;
	double _previousTime = 0.0;
	
};

#endif
