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


class Renderer {

public:

	Renderer();

	~Renderer();
	
	/// Init function
	void init(int width, int height);
	
	void setColorAndScale(const glm::vec3 & baseColor, const float scale);
	
	void loadFile(const std::string & midiFilePath);
	
	/// Draw function
	void draw();
	
	void drawGUI();
	
	/// Clean function
	void clean();

	/// Handle screen resizing
	void resize(int width, int height);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);


private:
	
	struct AppearanceState {
		glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 particlesColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 backgroundColor = glm::vec3(0.0f,0.0f,0.0f);
		float scale = 0.5;
		float minorsWidth = 0.8f;
		float particlesSpeed = 0.2f;
		float particlesExpansion = 1.0f;
		float particlesScale = 1.0f;
		int particlesCount = 256;
		std::vector<GLuint> particlesTexs;
		
		bool showParticles = true;
		bool showFlashes = true;
		bool showBlur = true;
		bool showBlurNotes = false;
		bool showHLines = true;
		bool showVLines = true;
		bool showDigits = true;
		bool showKeys = true;
		bool lockParticleColor = true;
	} _state;
	
	float _timer;
	float _timerStart;
	bool _shouldPlay;
	bool _showGUI;
	

	Camera _camera;
	
	std::shared_ptr<Framebuffer> _particlesFramebuffer;
	std::shared_ptr<Framebuffer> _blurFramebuffer;

	std::shared_ptr<MIDIScene> _scene;
	ScreenQuad _blurringScreen;
	ScreenQuad _blurryScreen;
	std::shared_ptr<Background> _background;
	
};

#endif
