#ifndef MIDIScene_h
#define MIDIScene_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"
#include "State.h"


class MIDIScene {

public:

	~MIDIScene();

	/// Init function
	MIDIScene(const std::string & midiFilePath);
	
	void updatesActiveNotes(double time);
	
	/// Draw function
	void drawNotes(float time, const glm::vec2 & invScreenSize, const glm::vec3 & majorColor, const glm::vec3 & minorColor, bool prepass);
	
	void drawFlashes(float time, const glm::vec2 & invScreenSize, const glm::vec3 & baseColor, float userScale);
	
	void drawParticles(float time, const glm::vec2 & invScreenSize, const State::ParticlesState & state, bool prepass);
	
	void drawKeyboard(float time, const glm::vec2 & invScreenSize, const glm::vec3 & keyColor, const glm::vec3 & majorColor, const glm::vec3 & minorColor, bool highlightKeys);

	/// Clean function
	void clean();

	const MIDIFile& midiFile() { return _midiFile; }
	
	void setScaleAndMinorWidth(const float scale, const float minorWidth);

	void setParticlesParameters(const float speed, const float expansion);
	
	double duration(){ return _duration; };
	
	void resetParticles();

private:
	
	GLuint _programId;
	GLuint _programFlashesId;
	GLuint _programParticulesId;
	GLuint _programKeysId;
	
	GLuint _vao;
	GLuint _ebo;
	
	GLuint _flagsBufferId;
	GLuint _vaoFlashes;
	GLuint _texFlash;
	
	GLuint _vaoParticles;
	GLuint _texParticles;

	GLuint _vaoKeyboard;
	GLuint _uboKeyboard;
	
	size_t _primitiveCount;
	size_t _notesCount;
	double _duration;
	
	std::vector<int> _actives;

	struct Particles {
		int note = -1;
		float duration = 0.0f;
		float start = 1000000.0f;
		float elapsed = 0.0f;
	};
	std::vector<Particles> _particles;
	double _previousTime;

	MIDIFile _midiFile;
	
	
	
};

#endif
