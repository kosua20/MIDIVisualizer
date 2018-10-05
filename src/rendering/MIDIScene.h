#ifndef MIDIScene_h
#define MIDIScene_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"

class MIDIScene {

public:

	~MIDIScene();

	/// Init function
	MIDIScene(const std::string & midiFilePath, const glm::vec3& baseColor, const float scale);
	
	
	void updatesActiveNotes(double time);
	
	/// Draw function
	void draw(float time, glm::vec2 invScreenSize);
	
	void drawFlashes(float time, glm::vec2 invScreenSize);
	
	void drawParticles(float time, glm::vec2 invScreenSize, bool prepass);
	
	/// Clean function
	void clean();

	const MIDIFile& midiFile() { return _midiFile; }
	
	void setScale(const float scale);
	
	void setColor(const glm::vec3 & color){ _baseColor = color; };
	
	glm::vec3 getColor(){ return _baseColor; };
	
	glm::vec3 & getColorRef(){ return _baseColor; };
	
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
