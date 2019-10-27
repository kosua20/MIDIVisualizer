#ifndef Renderer_h
#define Renderer_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <memory>
#include <array>

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
	

	struct Layer {
		
		enum Type : unsigned int {
			BGCOLOR = 0, BGTEXTURE, BLUR, ANNOTATIONS, KEYBOARD, PARTICLES, NOTES, FLASHES
		};

		Type type = BGCOLOR;
		std::string name = "None";
		void (Renderer::*draw)(const glm::vec2 &) = nullptr;
		bool * toggle = nullptr;

	};

	void blurPrepass();

	void drawBackgroundImage(const glm::vec2 & invSize);

	void drawBlur(const glm::vec2 & invSize);

	void drawParticles(const glm::vec2 & invSize);

	void drawScore(const glm::vec2 & invSize);

	void drawKeyboard(const glm::vec2 & invSize);

	void drawNotes(const glm::vec2 & invSize);

	void drawFlashes(const glm::vec2 & invSize);

	void drawGUI(const float currentTime);
	
	void showLayers();

	void applyAllSettings();
	
	void renderFile(const std::string & outputDirPath, const float frameRate);
	
	void reset();

	State _state;
	std::array<Layer, 8> _layers;

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
	//ScreenQuad _blurryScreen;
	ScreenQuad _passthrough;
	ScreenQuad _backgroundTexture;
	std::shared_ptr<Background> _score;
	bool _showLayers = false;
};

#endif
