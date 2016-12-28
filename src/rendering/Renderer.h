#ifndef Renderer_h
#define Renderer_h
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

#include "Framebuffer.h"
#include "camera/Camera.h"
#include "MIDIScene.h"
#include "ScreenQuad.h"
#include "Background.h"


class Renderer {

public:

	Renderer();

	~Renderer();
	
	/// Init function
	void init(int width, int height, const std::string& midiFilePath, const glm::vec3& baseColor, const float scale);

	/// Draw function
	void draw();

	/// Clean function
	void clean();

	/// Handle screen resizing
	void resize(int width, int height);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);


private:
	
	float _timer;
	bool _shouldPlay;
	
	Camera _camera;
	
	std::shared_ptr<Framebuffer> _particlesFramebuffer;
	std::shared_ptr<Framebuffer> _blurFramebuffer;

	MIDIScene _scene;
	ScreenQuad _blurringScreen;
	ScreenQuad _blurryScreen;
	Background _background;
	
};

#endif
