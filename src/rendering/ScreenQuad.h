#ifndef ScreenQuad_h
#define ScreenQuad_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>


class ScreenQuad {

public:

	ScreenQuad();

	~ScreenQuad();

	/// Init function
	void init(GLuint textureId, const std::string & fragShader);

	/// Draw function
	void draw(float time, glm::vec2 invScreenSize);

	/// Clean function
	void clean();
	
	GLuint programId(){return _programId; }
	
protected:
	GLuint _programId;
	
private:
	
	GLuint _vao;
	GLuint _ebo;
	GLuint _textureId;
	
	size_t _count;

};

#endif
