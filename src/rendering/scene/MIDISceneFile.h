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

	void updateSets(const SetOptions & options) override;

	~MIDISceneFile();

	void updatesActiveNotes(double time, double speed) override;

	double duration() const override;

	double secondsPerMeasure() const override;

	int notesCount() const override;

	void print() const override;
	
	void save(std::ofstream& file) const override;

	const std::string& filePath() const;

private:

	MIDIFile _midiFile;
	std::string _filePath;
	double _previousTime = 0.0;
	
};

#endif
