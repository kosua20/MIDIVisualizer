#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "../../helpers/ResourcesManager.h"

#include "MIDIScene.h"

#include <imgui/imgui.h>

#ifdef _WIN32
#undef MIN
#undef MAX
#endif

MIDIScene::~MIDIScene(){}

MIDIScene::MIDIScene(){
	renderSetup();
}

void MIDIScene::renderSetup(){

	std::vector<float> vertices = {-0.5,-0.5, 0.5, -0.5, 0.5,0.5, -0.5, 0.5};
	std::vector<unsigned int> indices = {1, 2, 0, 2, 3, 0};
	_primitiveCount = indices.size();
	// Create an array buffer to host the geometry data.
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Upload the data to the Array buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

	// Notes buffer.
	_dataBuffer = 0;
	glGenBuffers(1, &_dataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 1000, nullptr, GL_STATIC_DRAW);

	// Enabled notes buffer (empty for now).
	_flagsBufferId = 0;
	glGenBuffers(1, &_flagsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _flagsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 128, nullptr, GL_DYNAMIC_DRAW);

	// Programs.

	// Notes shaders.
	_programNotes.init("notes_vert", "notes_frag");

	// Generate a vertex array (useful when we add other attributes to the geometry).
	_vao = 0;
	glGenVertexArrays (1, &_vao);
	glBindVertexArray(_vao);
	// The first attribute will be the vertices positions.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);

	// The second attribute will be the notes data.
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _dataBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glVertexAttribDivisor(1, 1);

	// The second attribute will be the notes data.
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _dataBuffer);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
	glVertexAttribDivisor(2, 1);

	// We load the indices data
	glGenBuffers(1, &_ebo);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);

	checkGLError();

	// Flashes shaders.
	_programFlashes.init("flashes_vert", "flashes_frag");

	glGenVertexArrays (1, &_vaoFlashes);
	glBindVertexArray(_vaoFlashes);
	// The first attribute will be the vertices positions.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);
	// The second attribute will be the flags buffer.
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _flagsBufferId);
	glVertexAttribIPointer(1, 1, GL_INT, 0, NULL);
	glVertexAttribDivisor(1, 1);
	// We load the indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

	// Particles program.
	_programParticules.init("particles_vert", "particles_frag");

	glGenVertexArrays (1, &_vaoParticles);
	glBindVertexArray(_vaoParticles);
	// The first attribute will be the vertices positions.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

	// Particles trajectories texture loading.
	_texParticles = ResourcesManager::getTextureFor("particles");

	// Pass texture size to shader.
	const glm::vec2 tsize = ResourcesManager::getTextureSizeFor("particles");
	 _programParticules.use();
	_programParticules.uniform("inverseTextureSize", 1.0f / tsize);
	glUseProgram(0);

	// Keyboard setup.
	_programKeyMajors.init("majorKeys_vert", "majorKeys_frag");
	_programKeyMinors.init("minorKeys_vert", "minorKeys_frag");

	glGenVertexArrays(1, &_vaoKeyboard);
	glBindVertexArray(_vaoKeyboard);
	// The first attribute will be the vertices positions.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);
	// We load the indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBindVertexArray(0);

	// Pedals setup.
	_programPedals.init("pedal_vert", "pedal_frag");
	// Create an array buffer to host the geometry data.
	GLuint vboPdl = 0;
	glGenBuffers(1, &vboPdl);
	glBindBuffer(GL_ARRAY_BUFFER, vboPdl);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pedalsVertices.size(), &(pedalsVertices[0]), GL_STATIC_DRAW);
	glGenVertexArrays (1, &_vaoPedals);
	glBindVertexArray(_vaoPedals);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboPdl);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	// We load the indices data
	GLuint eboPdl = 0;
	glGenBuffers(1, &eboPdl);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboPdl);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * pedalsIndices.size(), &(pedalsIndices[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);
	_countPedals = pedalsIndices.size();

	// Wave setup.
	_programWave.init("wave_vert", "wave_frag");
	_programWaveNoise.init("wave_noise_vert", "wave_noise_frag");
	// Create an array buffer to host the geometry data.
	const int numSegments = 512;
	std::vector<glm::vec2> waveVerts((numSegments+1)*2);
	for(int vid = 0; vid < numSegments+1; ++vid){
		const float x = 2.0f * float(vid) / float(numSegments) - 1.0f;
		waveVerts[2*vid] = glm::vec2(x, 1.0f);
		waveVerts[2*vid+1] = glm::vec2(x, -1.0f);
	}
	std::vector<unsigned int> waveInds(3 * 2 * numSegments);
	for(int sid = 0; sid < numSegments; ++sid){
		const int bid = 6 * sid;
		const int vid = 2 * sid;
		waveInds[bid  ] = vid;
		waveInds[bid+1] = vid + 1;
		waveInds[bid+2] = vid + 2;
		waveInds[bid+3] = vid + 2;
		waveInds[bid+4] = vid + 1;
		waveInds[bid+5] = vid + 3;
	}
	GLuint vboWav = 0;
	glGenBuffers(1, &vboWav);
	glBindBuffer(GL_ARRAY_BUFFER, vboWav);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * waveVerts.size(), &(waveVerts[0][0]), GL_STATIC_DRAW);
	glGenVertexArrays (1, &_vaoWave);
	glBindVertexArray(_vaoWave);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboWav);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	// We load the indices data
	GLuint eboWav = 0;
	glGenBuffers(1, &eboWav);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboWav);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * waveInds.size(), &(waveInds[0]), GL_STATIC_DRAW);
	glBindVertexArray(0);
	_countWave = waveInds.size();

	// Prepare actives notes array.
	_actives.fill(-1);
	// Particle systems pool.
	_particles = std::vector<Particles>(256);
}

void MIDIScene::setScaleAndMinorWidth(const float scale, const float minorWidth){
	_programNotes.use();
	_programNotes.uniform("mainSpeed", scale);
	_programNotes.uniform("minorsWidth", minorWidth);
	_programKeyMinors.use();
	_programKeyMinors.uniform("minorsWidth", minorWidth);
	glUseProgram(0);
}

void MIDIScene::setParticlesParameters(const float speed, const float expansion){
	_programParticules.use();
	_programParticules.uniform("speedScaling", speed);
	_programParticules.uniform("expansionFactor", expansion);
	glUseProgram(0);
}

void MIDIScene::setKeyboardSizeAndFadeout(float keyboardHeight, float fadeOut){
	const float fadeOutFinal = keyboardHeight + (1.0f - keyboardHeight) * (1.0f - fadeOut);

	_programNotes.use();
	_programNotes.uniform("keyboardHeight", keyboardHeight);
	_programNotes.uniform("fadeOut", fadeOutFinal);
	_programParticules.use();
	_programParticules.uniform("keyboardHeight", keyboardHeight);
	_programKeyMinors.use();
	_programKeyMinors.uniform("keyboardHeight", keyboardHeight);
	_programKeyMajors.use();
	_programKeyMajors.uniform("keyboardHeight", keyboardHeight);
	_programFlashes.use();
	_programFlashes.uniform("keyboardHeight", keyboardHeight);
	glUseProgram(0);
}

void MIDIScene::setMinorEdgesAndHeight(bool minorEdges, float minorHeight){
	_programKeyMinors.use();
	_programKeyMinors.uniform("edgeOnMinors", minorEdges);
	_programKeyMinors.uniform("minorsHeight", minorHeight);
	glUseProgram(0);
}

void MIDIScene::resetParticles() {
	for (auto & particle : _particles) {
		particle.note = -1;
		particle.set = -1;
		particle.duration = particle.start = particle.elapsed = 0.0f;
	}
}

void MIDIScene::drawParticles(float time, const glm::vec2 & invScreenSize, const State::ParticlesState & state, bool prepass){

	glEnable(GL_BLEND);
	_programParticules.use();
	
	// Common uniforms values.
	_programParticules.uniform("inverseScreenSize", invScreenSize);
	_programParticules.uniform("time", 0.0f);

	// Prepass : bigger, darker particles.
	_programParticules.uniform("colorScale", prepass ? 0.6f : 1.6f);
	_programParticules.uniform("scale", state.scale * (prepass ? 2.0f : 1.0f));

	_programParticules.uniforms("baseColor", state.colors.size(), state.colors.data());
	
	// Particles trajectories texture.
	_programParticules.texture("textureParticles", _texParticles, GL_TEXTURE_2D);
	_programParticules.texture("lookParticles", state.tex, GL_TEXTURE_2D_ARRAY);

	_programParticules.uniform("texCount", state.texCount);

	// Select the geometry.
	glBindVertexArray(_vaoParticles);
	// For each active particles system, draw it with the right parameters.
	for(const auto & particle : _particles){
		if(particle.note >= 0){
			_programParticules.uniform("globalId", particle.note);
			_programParticules.uniform("time", particle.elapsed);
			_programParticules.uniform("duration", particle.duration);
			_programParticules.uniform("channel", particle.set);
			glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, state.count);
		}
	}
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);

}

void MIDIScene::drawNotes(float time, const glm::vec2 & invScreenSize, const State::NotesState & state, bool reverseScroll, bool prepass){
	
	_programNotes.use();
	
	// Uniforms setup.
	const float maxCornerRadius = 0.12f; // Internal tweaking to avoid notes whose edges are not timing-aligned.

	_programNotes.uniform("inverseScreenSize", invScreenSize);
	_programNotes.uniform("time", time);
	_programNotes.uniform("colorScale", prepass ? 0.6f: 1.0f);

	_programNotes.uniforms("baseColor" , state.majorColors.size(), state.majorColors.data());
	_programNotes.uniforms("minorColor", state.minorColors.size(), state.minorColors.data());
	_programNotes.uniform("reverseMode", reverseScroll);
	_programNotes.uniform("edgeWidth", state.edgeWidth);
	_programNotes.uniform("edgeBrightness", state.edgeBrightness);

	_programNotes.uniform("texturesScale", glm::vec2{state.majorTexScale, state.minorTexScale});
	_programNotes.uniform("texturesStrength", glm::vec2{state.majorTexAlpha, state.minorTexAlpha});
	_programNotes.uniform("scrollMajorTexture", state.majorTexScroll);
	_programNotes.uniform("scrollMinorTexture", state.minorTexScroll);
	_programNotes.uniform("cornerRadius", state.cornerRadius * maxCornerRadius);

	// Pass note textures if present, and corresponding flags.
	_programNotes.uniform("useMajorTexture", state.majorTex != 0);
	_programNotes.uniform("useMinorTexture", state.minorTex != 0);
	// IDs below can be 0
	_programNotes.texture("majorTexture", state.majorTex, GL_TEXTURE_2D);
	_programNotes.texture("minorTexture", state.minorTex, GL_TEXTURE_2D);
	
	// Draw the geometry.
	glBindVertexArray(_vao);
	glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, GLsizei(_dataBufferSubsize));

	glBindVertexArray(0);
	glUseProgram(0);
	
}

void MIDIScene::drawFlashes(float time, const glm::vec2 & invScreenSize, const State::FlashesState& state){
	
	// Need alpha blending.
	glEnable(GL_BLEND);

	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
	// Update the flags buffer accordingly.
	glBindBuffer(GL_ARRAY_BUFFER, _flagsBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _actives.size()*sizeof(int) ,&(_actives[0]));
	
	_programFlashes.use();
	
	// Uniforms setup.
	_programFlashes.uniform("inverseScreenSize", invScreenSize);
	_programFlashes.uniform("time",time);
	_programFlashes.uniforms("baseColor", state.colors.size(), state.colors.data());
	_programFlashes.uniform("userScale", state.size);
	_programFlashes.uniform("haloInnerRadius", state.haloInnerRadius);
	_programFlashes.uniform("haloOuterRadius", state.haloOuterRadius);
	_programFlashes.uniform("haloIntensity", state.haloIntensity);
	_programFlashes.uniform("texRowCount", state.texRowCount);
	_programFlashes.uniform("texColCount", state.texColCount);
	// Flash texture.
	_programFlashes.texture("textureFlash", state.tex, GL_TEXTURE_2D);
	
	// Draw the geometry.
	glBindVertexArray(_vaoFlashes);
	glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, 128);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

void MIDIScene::drawKeyboard(float, const glm::vec2 & invScreenSize, const glm::vec3 & keyColor, const ColorArray & majorColors, const ColorArray & minorColors, bool highlightKeys) {

	{
		_programKeyMajors.use();
		// Uniforms setup.
		_programKeyMajors.uniform("inverseScreenSize", invScreenSize);
		_programKeyMajors.uniform("edgeColor", keyColor);
		_programKeyMajors.uniforms("majorColor", majorColors.size(), majorColors.data());
		_programKeyMajors.uniform("highlightKeys", highlightKeys);
		_programKeyMajors.uniforms("actives", _actives.size(), _actives.data());

		// Draw the geometry.
		glBindVertexArray(_vaoKeyboard);
		glDrawElements(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0);
	}

	{
		_programKeyMinors.use();
		// Uniforms setup.
		_programKeyMinors.uniform("inverseScreenSize", invScreenSize);
		_programKeyMinors.uniform("edgeColor", keyColor);
		_programKeyMinors.uniforms("minorColor", minorColors.size(), minorColors.data());
		_programKeyMinors.uniform("highlightKeys", highlightKeys);
		_programKeyMinors.uniforms("actives", _actives.size(), _actives.data());

		glBindVertexArray(_vaoKeyboard);
		glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, 53);
	}
	
	glBindVertexArray(0);
	glUseProgram(0);
}

void MIDIScene::drawPedals(float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight, bool horizontalMode) {

	glEnable(GL_BLEND);
	_programPedals.use();

	// Adjust for aspect ratio.
	const float rat = invScreenSize.y/invScreenSize.x;
	const glm::vec2 ratioScale = (rat < 1.0f) ? glm::vec2(1.0f, rat) : glm::vec2(1.0f/rat, 1.0f);
	const glm::vec2 globalScale = 2.0f /* quad is -0.5,0.5 */ * state.size * ratioScale * glm::vec2(1.2f, 1.0f)  /* existing pedal ratio correction */;
	const State::PedalsState::Location mode = state.location;
	const float vertSign  = (mode == State::PedalsState::TOPLEFT || mode == State::PedalsState::TOPRIGHT) ? 1.0f : -1.0f;
	const float horizSign = (mode == State::PedalsState::TOPLEFT || mode == State::PedalsState::BOTTOMLEFT) ? -1.0f : 1.0f;
	const float safetyMargin = 0.02f;
	glm::vec2 globalShift = glm::vec2(horizSign, vertSign) * (1.0f - 0.5f * globalScale - safetyMargin * ratioScale);

	// If at the bottom, shift above the keyboard.
	if(horizontalMode){
		if(mode == State::PedalsState::TOPLEFT || mode == State::PedalsState::BOTTOMLEFT){
			globalShift[0] += 2.0f * keyboardHeight;
		}
	} else {
		if(mode == State::PedalsState::BOTTOMLEFT || mode == State::PedalsState::BOTTOMRIGHT){
			globalShift[1] += 2.0f * keyboardHeight;
		}
	}

	const float tightenShiftX = state.margin.x;
	const float tightenShiftY = state.margin.y;
	const GLuint textures[4] = { state.texSides[0], state.texCenter, state.texSides[1], state.texTop };


	// (1)soft, (2)sostenuto, (3)damper, (4)expression
	//   *-----------------^-----*
	//   |          (4)    | 20% |
	//   |-----------------v-----|
	//   |  (1)  |  (2)  |  (3)  |
	//   |       |   x   |       |
	//   |       |       |       |
	//   |<-35%->|<-30%->|<-35%->|
	//   |       |       |       |
	//   *-----------------------*

	const float expressionHeight = 0.20f;
	const float expressionWidth = 1.0f;
	const float sidesWidth = 0.35f;
	const float sidesHeight = 1.0f - expressionHeight;
	const float centralWidth = 1.0f - 2.0f * sidesWidth;
	const float centralHeight = sidesHeight;
	const float expressionShiftX = 0.0f;
	const float expressionShiftY = 0.5f * (1.0f - expressionHeight ) - tightenShiftY;
	const float sidesShiftX = 0.5f * (sidesWidth + centralWidth) - tightenShiftX;
	const float sidesShiftY = 0.5f * (sidesHeight-1.0f);
	const float centralShiftX = 0.0f;
	const float centralShiftY = 0.5f * (sidesHeight-1.0f);

	const glm::vec2 localScales[4] = {
		glm::vec2(sidesWidth, sidesHeight), glm::vec2(centralWidth, 	centralHeight),
		glm::vec2(sidesWidth, sidesHeight), glm::vec2(expressionWidth,	expressionHeight)
	};
	// By default all quads are centered at 0.5,0.5 in local space, we need to shift them post-scaling.
	const glm::vec2 localShifts[4] = {
		glm::vec2(-sidesShiftX, sidesShiftY), glm::vec2(centralShiftX,		centralShiftY),
		glm::vec2( sidesShiftX, sidesShiftY), glm::vec2(expressionShiftX,	expressionShiftY)
	};
	const float actives[4] = { _pedals.soft, _pedals.sostenuto, _pedals.damper, _pedals.expression };
	const glm::vec3* colors[4] = { &state.leftColor, &state.centerColor, &state.rightColor, &state.topColor};
	// Uniforms setup.
	_programPedals.uniform("pedalOpacity", state.opacity);

	glBindVertexArray(_vao);
	if(state.merge){
		const float active = (glm::max)((glm::max)(actives[0], actives[1]), (glm::max)(actives[2], actives[3]));
		// We want to move the central pedal to the side
		const glm::vec2 localShift = glm::vec2(horizSign, vertSign) * glm::vec2(sidesShiftX, 0.5f * expressionHeight);

		_programPedals.uniform("scale", globalScale * localScales[1]);
		_programPedals.uniform("shift", globalShift + globalScale * localShift);
		_programPedals.uniform("pedalFlag", active);
		_programPedals.uniform("mirror", false);
		_programPedals.texture("pedalTexture", textures[1], GL_TEXTURE_2D);
		_programPedals.uniform("pedalColor", *colors[1]);

		glDrawElements(GL_TRIANGLES, int(_vaoFlashes), GL_UNSIGNED_INT, (void*)0);
	} else {
		for(unsigned int i = 0; i < 4; ++i){
			_programPedals.uniform("scale", globalScale * localScales[i]);
			_programPedals.uniform("shift", globalShift + globalScale * localShifts[i]);
			_programPedals.uniform("pedalFlag", actives[i]);
			_programPedals.texture("pedalTexture", textures[i], GL_TEXTURE_2D);
			_programPedals.uniform("mirror", state.mirror && (i==2));
			_programPedals.uniform("pedalColor", *colors[i]);
			glDrawElements(GL_TRIANGLES, int(_vaoFlashes), GL_UNSIGNED_INT, (void*)0);
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
}

void MIDIScene::drawWaves(float time, const glm::vec2 & invScreenSize, const State::WaveState & state, float keyboardHeight) {

	_programWave.use();
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

	// Uniforms setup.
	_programWave.uniform("waveColor", state.color);
	_programWave.uniform("keyboardSize", keyboardHeight);
	_programWave.uniform("waveOpacity", state.opacity);
	_programWave.uniform("spread", state.spread);
	glBindVertexArray(_vaoWave);

	// Fixed initial parameters.
	const float ampls[4] = {-0.023f, -0.011f, 0.017f, 0.009f};
	const float freqs[4] = {10.3f, -8.27f, -4.4f, 13.7f};
	const float phases[4] = {5.2f, 4.7f, 9.3f, -7.1f};
	// Render multiple waves with additive blending.
	for(int i = 0; i < 4; ++i){
		const float ampl = state.amplitude * ampls[i];
		const float freq = state.frequency * freqs[i];
		const float phase = phases[i] * time * state.speed + float(i+1) * 7.39f;
		_programWave.uniform("amplitude", ampl);
		_programWave.uniform("freq", freq);
		_programWave.uniform("phase", phase);
		glDrawElements(GL_TRIANGLES, int(_countWave), GL_UNSIGNED_INT, (void*)0);
	}

	glBindVertexArray(0);
	glUseProgram(0);

	_programWaveNoise.use();
	_programWaveNoise.uniform("keyboardSize", keyboardHeight);
	_programWaveNoise.uniform("scale", state.noiseSize * 0.5f);

	_programWaveNoise.texture("textureNoise", ResourcesManager::getTextureFor("noise"), GL_TEXTURE_2D);
	_programWaveNoise.uniform("offset", time * state.speed * 0.05f);
	_programWaveNoise.uniform("waveColor", state.color);
	_programWaveNoise.uniform("noiseScale", state.frequency);
	_programWaveNoise.uniform("waveOpacity", state.noiseIntensity);
	_programWaveNoise.uniform("inverseScreenSize", invScreenSize);

	glBindVertexArray(_vaoFlashes);
	glDrawElements(GL_TRIANGLES, int(_vaoFlashes), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glDisable(GL_BLEND);
}

void MIDIScene::setMinMaxKeys(int minKey, int minKeyMajor, int notesCount){
	_programNotes.use();
	_programNotes.uniform("minNoteMajor", minKeyMajor);
	_programNotes.uniform("notesCount", float(notesCount));
	_programFlashes.use();
	_programFlashes.uniform("minNote", minKey);
	_programFlashes.uniform("notesCount", float(notesCount));
	_programKeyMajors.use();
	_programKeyMajors.uniform("minNoteMajor", minKeyMajor);
	_programKeyMajors.uniform("notesCount", float(notesCount));
	_programKeyMinors.use();
	_programKeyMinors.uniform("minNoteMajor", minKeyMajor);
	_programKeyMinors.uniform("notesCount", float(notesCount));
	_programParticules.use();
	_programParticules.uniform("minNote", minKey);
	_programParticules.uniform("notesCount", float(notesCount));
	glUseProgram(0);
}


void MIDIScene::setOrientation(bool horizontal){
	_programNotes.use();
	_programNotes.uniform("horizontalMode", horizontal);
	_programFlashes.use();
	_programFlashes.uniform("horizontalMode", horizontal);
	_programKeyMajors.use();
	_programKeyMajors.uniform("horizontalMode", horizontal);
	_programKeyMinors.use();
	_programKeyMinors.uniform("horizontalMode", horizontal);
	_programParticules.use();
	_programParticules.uniform("horizontalMode", horizontal);
	_programWave.use();
	_programWave.uniform("horizontalMode", horizontal);
	_programWaveNoise.use();
	_programWaveNoise.uniform("horizontalMode", horizontal);
	glUseProgram(0);
}

void MIDIScene::clean(){
	glDeleteVertexArrays(1, &_vao);
	glDeleteVertexArrays(1, &_vaoFlashes);
	glDeleteVertexArrays(1, &_vaoParticles);

	_programNotes.clean();
	_programFlashes.clean();
	_programParticules.clean();
	_programKeyMinors.clean();
	_programKeyMajors.clean();
	_programPedals.clean();
	_programWave.clean();
	_programWaveNoise.clean();
}

void MIDIScene::upload(const std::vector<GPUNote> & data){
	glBindBuffer(GL_ARRAY_BUFFER, _dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GPUNote) * data.size(), &(data[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MIDIScene::upload(const std::vector<GPUNote> & data, int mini, int maxi){
	const int size = maxi - mini + 1;
	glBindBuffer(GL_ARRAY_BUFFER, _dataBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, mini * sizeof(GPUNote), size * sizeof(GPUNote), &(data[mini]));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

MIDISceneEmpty::MIDISceneEmpty(){
	// Upload one dummy note.
	std::vector<GPUNote> data = { GPUNote() };
	upload(data);
	_dataBufferSubsize = 0;
}

void MIDISceneEmpty::updateSets(const SetOptions & options){

}

void MIDISceneEmpty::updatesActiveNotes(double time, double speed){

}

double MIDISceneEmpty::duration() const {
	return 0.0;
}

double MIDISceneEmpty::secondsPerMeasure() const {
	return 1.0;
}

int MIDISceneEmpty::notesCount() const {
	return 0;
}

void MIDISceneEmpty::print() const {

}

void MIDISceneEmpty::save(std::ofstream& file) const {

}

MIDISceneEmpty::~MIDISceneEmpty(){

}
