#ifndef Recorder_h
#define Recorder_h

#include <gl3w/gl3w.h>
#include "../rendering/Framebuffer.h"
#include "../helpers/Configuration.h"
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <mutex>
#include <thread>

// This is highly experimental and untested for now.
// #define FFMPEG_USE_THREADS

// Forward declare FFmpeg objects in all cases.
struct AVFormatContext;
struct AVCodec;
struct AVCodecContext;
struct AVStream;
struct AVFrame;
struct SwsContext;

class Recorder {

public:

	Recorder();

	~Recorder();

	void record(const std::shared_ptr<Framebuffer> & frame);

	bool drawGUI(float scale);

	void prepare(float preroll, float duration, float speed);

	void start(bool verbose);

	bool flush();

	void drawProgress();
	
	bool isRecording() const;

	bool isTransparent() const;
	
	float currentTime() const;

	size_t currentFrame() const;

	size_t framesCount() const;

	const glm::ivec2 & requiredSize() const;

	void setSize(const glm::ivec2 & size);

	bool setParameters(const Export& exporting);

	static bool videoExportSupported();

private:

	bool initVideo(const std::string & path, Export::Format format, bool verbose);

	bool addFrameToVideo(GLubyte * data);
	
	void endVideo();

	struct CodecOpts {
		std::string name;
		std::string ext;
		Export::Format format;
	};
	
	std::vector<CodecOpts> _formats;
	std::vector<std::vector<GLubyte>> _savingBuffers;
	std::vector<std::thread> _savingThreads;

	Export _config;
	glm::ivec2 _size {0, 0};
	size_t _framesCount = 0;
	size_t _currentFrame = 0;
	float _sceneDuration = 0.0f;
	float _currentTime = 0.0f;

	// Video context ptrs if available.
	AVFormatContext * _formatCtx = nullptr;
	const AVCodec * _codec = nullptr;
	AVCodecContext * _codecCtx = nullptr;
	AVStream * _stream = nullptr;
	std::vector<AVFrame *> _frames;
	std::vector<SwsContext *> _swsContexts;
#ifdef FFMPEG_USE_THREADS
	std::mutex _streamMutex;
#endif

	std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
};

#endif
