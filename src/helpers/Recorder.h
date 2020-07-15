#ifndef Recorder_h
#define Recorder_h


#include <gl3w/gl3w.h>
#include "../rendering/Framebuffer.h"
#include <string>
#include <vector>

class Recorder {

public:

	Recorder();

	~Recorder();

	void record(const std::shared_ptr<Framebuffer> & frame);

	bool drawGUI();

	void start(float preroll, float duration);
	
	bool drawProgress();
	
	bool isRecording() const;

	bool isTransparent() const;
	
	float currentTime() const;

	size_t currentFrame() const;

	size_t framesCount() const;

	const glm::ivec2 & requiredSize() const;

	void setSize(const glm::ivec2 & size);

private:

	float _currentTime = 0.0f;
	size_t _framesCount = 0;
	size_t _currentFrame = 0;

	std::string _exportPath;
	int _exportFramerate = 60;
	bool _exportNoBackground = false;
	std::vector<GLubyte> _buffer;

	// Extra info for display.
	glm::ivec2 _size {0, 0};
	float _sceneDuration = 0.0f;
};

#endif
