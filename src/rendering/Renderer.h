#pragma once
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>

#include "../midi/MIDIFile.h"
#include "State.h"
#include "../helpers/ProgramUtilities.h"
#include "ScreenQuad.h"

#include <fstream>
#include <memory>
#include <vector>

class MIDIScene;
class Framebuffer;

class Renderer {

public:

	void renderSetup();

	void upload(const std::shared_ptr<MIDIScene>& scene);

	void setScaleAndMinorWidth(const float scale, const float minorWidth);

	void setParticlesParameters(const float speed, const float expansion);

	void setKeyboardSizeAndFadeout(float keyboardHeight, float fadeOut);

	void setMinorEdgesAndHeight(bool minorEdges, float minorHeight);

	void setMinMaxKeys(int minKey, int minKeyMajor, int notesCount);

	void setOrientation(bool horizontal);

	/// Draw function
	void drawNotes(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::NotesState & state, bool reverseScroll, bool prepass);
	
	void drawFlashes(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::FlashesState& state);
	
	void drawParticles(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::ParticlesState & state, bool prepass);
	
	void drawKeyboard(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const glm::vec3 & edgeColor, const glm::vec3 & keyColor, const ColorArray & majorColors, const ColorArray & minorColors, bool highlightKeys);

	/// Volumetric keyboard lit by the notes, replaces the flat one (vertical scrolling only).
	void drawKeyboard3D(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::KeyboardState & state, const State::MascotState & mascot, const ColorArray & majorColors, const ColorArray & minorColors);

	/// Load a .mascot file (a model baked into a distance field), empty path to remove it. Returns false on failure.
	bool loadMascot(const std::string & path);

	/// Why the last requested mascot could not be loaded, empty if it was.
	const std::string & mascotError() const { return _mascotError; }

	/// Settings stored with the loaded mascot (its look from the mockups, path left empty), null if none is loaded.
	const State::MascotState * mascotDefaults() const { return _mascotDist ? &_mascotDefaults : nullptr; }

	void drawPedals(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight, bool horizontalMode);

	void drawWaves(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::WaveState & state, float keyboardHeight);

	void drawScore(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::ScoreState & state, float measureScale, float qualityScale,  float keyboardHeight, bool horizontalMode, bool reverseScroll);

	void clean();

private:
	
	ShaderProgram _programNotes;
	ShaderProgram _programFlashes;
	ShaderProgram _programParticules;
	ShaderProgram _programKeyMinors;
	ShaderProgram _programKeyMajors;
	ShaderProgram _programKeys3D;
	/// Travel of each 3D key (0: rest, 1: bottom), animated by a spring so that presses have some weight.
	std::array<float, 128> _keyTravel{};
	std::array<float, 128> _keyVelocity{};
	float _keyAnimTime = 0.0f;
	float _minorWidth = 1.0f;
	float _keyboardHeight = 0.25f;
	/// The 3D keyboard at rest, rendered once and reused for the keys where nothing happens.
	std::shared_ptr<Framebuffer> _keys3DBake;
	/// Everything the baked image depends on; it is rendered again when any of it changes.
	std::vector<float> _keys3DBakeSignature;
	/// Lower resolution render of the 3D keyboard, and the pass scaling it up onto the frame.
	std::shared_ptr<Framebuffer> _keys3DLow;
	ScreenQuad _keys3DUpscale;
	/// Mascot: distance to its surface and its colors (rgb albedo, a: glow), in a box around the model.
	GLuint _mascotDist = 0;
	GLuint _mascotColor = 0;
	glm::vec3 _mascotHalf{1.0f};
	glm::vec3 _mascotCenter{0.0f};
	glm::vec2 _mascotDistRange{0.0f, 1.0f};
	std::string _mascotPath; ///< Last requested file, loaded or not.
	std::string _mascotError;
	State::MascotState _mascotDefaults;
	int _mascotGeneration = 0; ///< Changes with each loaded file, for the bake signature.
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
	GLuint _texNoise;

	// Cached info.
	unsigned int _minKeyMajor{0};
	unsigned int _keyCount{128};
};
