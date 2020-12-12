#ifndef Recorder_h
#define Recorder_h

#include <gl3w/gl3w.h>
#include "../rendering/Framebuffer.h"
#include <string>
#include <vector>
#include <memory>
#include <chrono>

// Forward declare FFmpeg objects in all cases.
struct AVFormatContext;
struct AVCodec;
struct AVCodecContext;
struct AVStream;
struct AVFrame;
struct SwsContext;

class Recorder {

public:

	enum class Format : int {
		PNG = 0, MPEG2 = 1, MPEG4 = 2
	};

	Recorder();

	~Recorder();

	void record(const std::shared_ptr<Framebuffer> & frame);

	bool drawGUI(float scale);

	void prepare(float preroll, float duration, float speed);

	void start(bool verbose);

	void drawProgress();
	
	bool isRecording() const;

	bool isTransparent() const;
	
	float currentTime() const;

	size_t currentFrame() const;

	size_t framesCount() const;

	const glm::ivec2 & requiredSize() const;

	void setSize(const glm::ivec2 & size);

	bool setParameters(const std::string & path, Format format, int framerate, int bitrate, bool skipBackground);

	static bool videoExportSupported();

private:

	bool initVideo(const std::string & path, Format format, bool verbose);

	bool addFrameToVideo(GLubyte * data);
	
	void endVideo();

	bool flush();

	struct CodecOpts {
		std::string name;
		std::string ext;
		Recorder::Format format;
	};
	
	std::vector<CodecOpts> _formats;
	std::vector<GLubyte> _buffer;
	std::string _exportPath;
	glm::ivec2 _size {0, 0};
	size_t _framesCount = 0;
	size_t _currentFrame = 0;
	Format _outFormat = Format::PNG;
	float _sceneDuration = 0.0f;
	float _currentTime = 0.0f;
	int _exportFramerate = 60;
	int _bitRate = 40;
	bool _exportNoBackground = false;

	// Video context ptrs if available.
	AVFormatContext * _formatCtx = nullptr;
	AVCodec * _codec = nullptr;
	AVCodecContext * _codecCtx = nullptr;
	AVStream * _stream = nullptr;
	AVFrame * _frame = nullptr;
	SwsContext * _swsContext = nullptr;

	std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
};

#endif
