#ifndef Renderer_h
#define Renderer_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <memory>

#include "Framebuffer.h"
#include "camera/Camera.h"
#include "MIDIScene.h"
#include "ScreenQuad.h"
#include "Background.h"

#include "State.h"

#define DEBUG_SPEED (1.0f)

class Renderer {

public:

	Renderer();

	~Renderer();
	
	/// Init function
	void init(int width, int height);
	
	void setColorAndScale(const glm::vec3 & baseColor, const float scale);
	
	void loadFile(const std::string & midiFilePath);
	
	/// Draw function
	void draw(const float currentTime);
	
	/// Clean function
	void clean();

	/// Handle screen resizing
	void resize(int width, int height);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);


private:
	
	void drawGUI(const float currentTime);
	
	void applyAllSettings();
	
	void renderFile(const std::string & outputDirPath, const float frameRate);
	
	void reset();

	State _state;
	int _exportFramerate;
	float _timer;
	float _timerStart;
	bool _shouldPlay;
	bool _showGUI;
	bool _showDebug;

	int _performExport;
	std::string _exportPath;
	
	Camera _camera;
	
	std::shared_ptr<Framebuffer> _particlesFramebuffer;
	std::shared_ptr<Framebuffer> _blurFramebuffer;
	std::shared_ptr<Framebuffer> _finalFramebuffer;

	std::shared_ptr<MIDIScene> _scene;
	ScreenQuad _blurringScreen;
	ScreenQuad _blurryScreen;
	ScreenQuad _finalScreen;
	std::shared_ptr<Background> _background;
	
};

#endif
