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

#define MIDIVIZ_VERSION_MAJOR 3
#define MIDIVIZ_VERSION_MINOR 0

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
	
	/// Clean function
	void clean();

	/// Handle screen resizing
	void resize(int width, int height);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);


private:
	
	void drawGUI();
	
	void loadSettings(const std::string & path);
	
	void saveSettings(const std::string & path);
	
	void resetSettings(bool forceApply);
	
	void applyAllSettings();
	
	struct BackgroundState {
		glm::vec3 color;
		float minorsWidth;
		bool hLines;
		bool vLines;
		bool digits;
		bool keys;
	};
	
	
	struct ParticlesState {
		glm::vec3 color ;
		std::vector<GLuint> texs;
		float speed;
		float expansion ;
		float scale;
		int count;
	};
	
	struct AppearanceState {
		BackgroundState background;
		ParticlesState particles;
		glm::vec3 baseColor;
		float scale;
		bool showParticles;
		bool showFlashes;
		bool showBlur;
		bool showBlurNotes;
		bool lockParticleColor;
	} _state;
	
	float _timer;
	float _timerStart;
	bool _shouldPlay;
	bool _showGUI;
	bool _showDebug;

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
