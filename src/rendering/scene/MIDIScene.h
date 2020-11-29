#ifndef MIDIScene_h
#define MIDIScene_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"
#include "../State.h"

class MIDIScene {

public:

	MIDIScene();

	/// Draw function
	void drawNotes(float time, const glm::vec2 & invScreenSize, const ColorArray & majorColors, const ColorArray & minorColors, bool reverseScroll, bool prepass);
	
	void drawFlashes(float time, const glm::vec2 & invScreenSize, const ColorArray & baseColors, float userScale);
	
	void drawParticles(float time, const glm::vec2 & invScreenSize, const State::ParticlesState & state, bool prepass);
	
	void drawKeyboard(float time, const glm::vec2 & invScreenSize, const glm::vec3 & keyColor, const ColorArray & majorColors, const ColorArray & minorColors, bool highlightKeys);

	void drawPedals(float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight);

	void drawWaves(float time, const glm::vec2 & invScreenSize, const State::WaveState & state, float keyboardHeight);

	/// Clean function
	void clean();
	
	void setScaleAndMinorWidth(const float scale, const float minorWidth);

	void setParticlesParameters(const float speed, const float expansion);

	void setKeyboardSize(float keyboardHeight);

	void setMinMaxKeys(int minKey, int minKeyMajor, int notesCount);

	void resetParticles();

	// Type specific methods.

	virtual void updateSets(const SetOptions & options) = 0;

	virtual void updatesActiveNotes(double time, double speed) = 0;

	virtual double duration() const = 0;

	virtual double secondsPerMeasure() const = 0;

	virtual int notesCount() const = 0;

	virtual void print() const = 0;

	virtual ~MIDIScene();

protected:

	struct Pedals {
		float damper = 0.0f;
		float sostenuto = 0.0f;
		float soft = 0.0f;
		float expression = 0.0f;
	};

	struct Particles {
		int note = -1;
		int set = -1;
		float duration = 0.0f;
		float start = 1000000.0f;
		float elapsed = 0.0f;
	};

	struct GPUNote {
		float note = 0.0f;
		float start = 0.0f;
		float duration = 0.0f;
		float isMinor = 0.0f;
		float set = 0.0f;
	};

	void upload(const std::vector<GPUNote> & data);
	
	void upload(const std::vector<GPUNote> & data, int mini, int maxi);

	std::array<int, 128> _actives;
	std::vector<Particles> _particles;
	Pedals _pedals;
	int _dataBufferSubsize = 0;
	
private:

	void renderSetup();

	GLuint _programId;
	GLuint _programFlashesId;
	GLuint _programParticulesId;
	GLuint _programKeysId;
	GLuint _programPedalsId;
	GLuint _programWaveId;
	
	GLuint _vao;
	GLuint _ebo;
	GLuint _dataBuffer;
	
	GLuint _flagsBufferId;
	GLuint _vaoFlashes;
	GLuint _texFlash;
	
	GLuint _vaoParticles;
	GLuint _texParticles;

	GLuint _vaoKeyboard;
	GLuint _uboKeyboard;

	GLuint _vaoPedals;
	size_t _countPedals;

	GLuint _vaoWave;
	size_t _countWave;
	
	size_t _primitiveCount;

};

class MIDISceneEmpty : public MIDIScene {
public:

	MIDISceneEmpty();
	
	void updateSets(const SetOptions & options);

	void updatesActiveNotes(double time, double speed);

	double duration() const;

	double secondsPerMeasure() const;

	int notesCount() const;

	void print() const;

	~MIDISceneEmpty();

};

#endif
