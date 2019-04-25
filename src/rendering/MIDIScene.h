#ifndef MIDIScene_h
#define MIDIScene_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"


#define PARTICLES_TEXTURE_COUNT 4

class MIDIScene {

public:

	~MIDIScene();

	/// Init function
	MIDIScene(const std::string & midiFilePath);
	
	
	void updatesActiveNotes(double time);
	
	/// Draw function
	void draw(float time, glm::vec2 invScreenSize, const glm::vec3 & baseColor, const glm::vec3 & minorColor, bool prepass);
	
	void drawFlashes(float time, glm::vec2 invScreenSize, const glm::vec3 & baseColor, float userScale);
	
	void drawParticles(const float time, const glm::vec2 & invScreenSize, const glm::vec3 & particlesColor, const float particlesScale, const std::vector<GLuint> &lookTextures, const int particlesCount, bool prepass);
	
	/// Clean function
	void clean();

	const MIDIFile& midiFile() { return _midiFile; }
	
	void setScaleAndMinorWidth(const float scale, const float minorWidth);

	void setParticlesParameters(const float speed, const float expansion);
	
	double duration(){ return _duration; };
	
private:
	
	GLuint _programId;
	GLuint _programFlashesId;
	GLuint _programParticulesId;
	
	GLuint _vao;
	GLuint _ebo;
	
	GLuint _flagsBufferId;
	GLuint _vaoFlashes;
	GLuint _texFlash;
	
	GLuint _vaoParticles;
	GLuint _texParticles;
	//GLuint _lookParticles;
	
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
