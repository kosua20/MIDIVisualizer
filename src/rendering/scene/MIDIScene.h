#ifndef MIDIScene_h
#define MIDIScene_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"
#include "../State.h"
#include "../../helpers/ProgramUtilities.h"

#include <fstream>

class MIDIScene {

public:

	MIDIScene();

	/// Draw function
	void drawNotes(float time, const glm::vec2 & invScreenSize, const State::NotesState & state, bool reverseScroll, bool prepass);
	
	void drawFlashes(float time, const glm::vec2 & invScreenSize, const State::FlashesState& state);
	
	void drawParticles(float time, const glm::vec2 & invScreenSize, const State::ParticlesState & state, bool prepass);
	
	void drawKeyboard(float time, const glm::vec2 & invScreenSize, const glm::vec3 & keyColor, const ColorArray & majorColors, const ColorArray & minorColors, bool highlightKeys);

	void drawPedals(float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight, bool horizontalMode);

	void drawWaves(float time, const glm::vec2 & invScreenSize, const State::WaveState & state, float keyboardHeight);

	void drawScore(float time, const glm::vec2 & invScreenSize, const State::ScoreState & state, float measureScale, float qualityScale,  float keyboardHeight, bool horizontalMode, bool reverseScroll);

	/// Clean function
	void clean();
	
	void setScaleAndMinorWidth(const float scale, const float minorWidth);

	void setParticlesParameters(const float speed, const float expansion);

	void setKeyboardSizeAndFadeout(float keyboardHeight, float fadeOut);

	void setMinorEdgesAndHeight(bool minorEdges, float minorHeight);

	void setMinMaxKeys(int minKey, int minKeyMajor, int notesCount);

	void setOrientation(bool horizontal);

	void resetParticles();

	// Type specific methods.

	virtual void updateSets(const SetOptions & options) = 0;

	virtual void updatesActiveNotes(double time, double speed) = 0;

	virtual double duration() const = 0;

	virtual double secondsPerMeasure() const = 0;

	virtual int notesCount() const = 0;

	virtual void print() const = 0;

	virtual void save(std::ofstream& file) const = 0;

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
	int _notesDataBufferSubsize = 0;
	
private:

	void renderSetup();

	ShaderProgram _programNotes;
	ShaderProgram _programFlashes;
	ShaderProgram _programParticules;
	ShaderProgram _programKeyMinors;
	ShaderProgram _programKeyMajors;
	ShaderProgram _programPedals;
	ShaderProgram _programWave;
	ShaderProgram _programWaveNoise;
	ShaderProgram _programScoreBars;
	ShaderProgram _programScoreLabels;

	GLuint _notesDataBuffer;
	GLuint _keysDataBuffer;
	GLuint _quadVertices;
	GLuint _quadIndices;
	GLuint _waveVertices;
	GLuint _waveIndices;

	GLuint _vaoQuad;
	GLuint _vaoQuadWithNoteData;
	GLuint _vaoQuadWithKeyData;
	size_t _quadPrimitiveCount;

	GLuint _vaoWave;
	size_t _wavePrimitiveCount;

	GLuint _texParticles;
	GLuint _texFont;

	// Cached info.
	unsigned int _minKeyMajor{0};
	unsigned int _keyCount{128};
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

	void save(std::ofstream& file) const;

	~MIDISceneEmpty();

};

#endif
