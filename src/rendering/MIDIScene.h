#ifndef MIDIScene_h
#define MIDIScene_h
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"

class MIDIScene {

public:

	MIDIScene();

	~MIDIScene();

	/// Init function
	void init(const std::string & midiFilePath, const glm::vec3& baseColor, const float scale);
	
	
	void updatesActiveNotes(double time);
	
	/// Draw function
	void draw(float time, glm::vec2 invScreenSize);
	
	void drawFlashes(float time, glm::vec2 invScreenSize);
	
	void drawParticles(float time, glm::vec2 invScreenSize, bool prepass);
	
	/// Clean function
	void clean();

	const MIDIFile& midiFile() { return _midiFile; }
	
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
	
	size_t _primitiveCount;
	size_t _notesCount;
	
	std::vector<std::pair<double,double>> _actives;
	MIDIFile _midiFile;
	
	glm::vec3 _baseColor;
	
};

#endif
