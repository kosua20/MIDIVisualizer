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
	// TODO: (MV) wrap program in abstraction with reflection, to avoid querying IDs at each frame
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

	// Flash texture loading.
	_texFlash = ResourcesManager::getTextureFor("flash");

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
	glUniform2f(_programParticules["inverseTextureSize"], 1.0f/float(tsize[0]), 1.0f/float(tsize[1]));
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
	glUniform1f(_programNotes["mainSpeed"], scale);
	glUniform1f(_programNotes["minorsWidth"], minorWidth);
	_programKeyMinors.use();
	glUniform1f(_programKeyMinors["minorsWidth"], minorWidth);
	glUseProgram(0);
}

void MIDIScene::setParticlesParameters(const float speed, const float expansion){
	_programParticules.use();
	glUniform1f(_programParticules["speedScaling"], speed);
	glUniform1f(_programParticules["expansionFactor"], expansion);
	glUseProgram(0);
}

void MIDIScene::setKeyboardSizeAndFadeout(float keyboardHeight, float fadeOut){
	const float fadeOutFinal = keyboardHeight + (1.0f - keyboardHeight) * (1.0f - fadeOut);

	_programNotes.use();
	glUniform1f(_programNotes["keyboardHeight"], keyboardHeight);
	glUniform1f(_programNotes["fadeOut"], fadeOutFinal);
	_programParticules.use();
	glUniform1f(_programParticules["keyboardHeight"], keyboardHeight);
	_programKeyMinors.use();
	glUniform1f(_programKeyMinors["keyboardHeight"], keyboardHeight);
	_programKeyMajors.use();
	glUniform1f(_programKeyMajors["keyboardHeight"], keyboardHeight);
	_programFlashes.use();
	glUniform1f(_programFlashes["keyboardHeight"], keyboardHeight);
	glUseProgram(0);
}

void MIDIScene::setMinorEdgesAndHeight(bool minorEdges, float minorHeight){
	_programKeyMinors.use();
	glUniform1i(_programKeyMinors["edgeOnMinors"], int(minorEdges));
	glUniform1f(_programKeyMinors["minorsHeight"], minorHeight);
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
	glUniform2fv(_programParticules["inverseScreenSize"], 1, &(invScreenSize[0]));
	glUniform1f(_programParticules["time"], 0.0f);

	// Prepass : bigger, darker particles.
	glUniform1f(_programParticules["colorScale"], prepass ? 0.6f : 1.6f);
	glUniform1f(_programParticules["scale"], state.scale * (prepass ? 2.0f : 1.0f));

	glUniform3fv(_programParticules["baseColor"], GLsizei(state.colors.size()), &state.colors[0][0]);
	
	// Particles trajectories texture.
	_programParticules.texture("textureParticles", _texParticles, GL_TEXTURE_2D);
	_programParticules.texture("lookParticles", state.tex, GL_TEXTURE_2D_ARRAY);

	glUniform1i(_programParticules["texCount"], state.texCount);

	// Select the geometry.
	glBindVertexArray(_vaoParticles);
	// For each active particles system, draw it with the right parameters.
	for(const auto & particle : _particles){
		if(particle.note >= 0){
			glUniform1i(_programParticules["globalId"], particle.note);
			glUniform1f(_programParticules["time"], particle.elapsed);
			glUniform1f(_programParticules["duration"], particle.duration);
			glUniform1i(_programParticules["channel"], particle.set);
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

	glUniform2fv(_programNotes["inverseScreenSize"],1, &(invScreenSize[0]));
	glUniform1f(_programNotes["time"], time);
	glUniform1f(_programNotes["colorScale"], prepass ? 0.6f: 1.0f);

	glUniform3fv(_programNotes["baseColor"], GLsizei(state.majorColors.size()), &(state.majorColors[0][0]));
	glUniform3fv(_programNotes["minorColor"], GLsizei(state.minorColors.size()), &(state.minorColors[0][0]));
	glUniform1i(_programNotes["reverseMode"], reverseScroll ? 1 : 0);
	glUniform1f(_programNotes["edgeWidth"], state.edgeWidth);
	glUniform1f(_programNotes["edgeBrightness"], state.edgeBrightness);

	glUniform2f(_programNotes["texturesScale"], state.majorTexScale, state.minorTexScale);
	glUniform2f(_programNotes["texturesStrength"], state.majorTexAlpha, state.minorTexAlpha);
	glUniform1f(_programNotes["scrollMajorTexture"], state.majorTexScroll);
	glUniform1f(_programNotes["scrollMinorTexture"], state.minorTexScroll);
	glUniform1f(_programNotes["cornerRadius"], state.cornerRadius * maxCornerRadius);

	// Pass note textures if present, and corresponding flags.
	glUniform1i(_programNotes["useMajorTexture"], int(state.majorTex != 0));
	glUniform1i(_programNotes["useMinorTexture"], int(state.minorTex != 0));
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
	glUniform2fv(_programFlashes["inverseScreenSize"],1, &(invScreenSize[0]));
	glUniform1f(_programFlashes["time"],time);
	glUniform3fv(_programFlashes["baseColor"], GLsizei(state.colors.size()), &(state.colors[0][0]));
	glUniform1f(_programFlashes["userScale"], state.size);
	// Flash texture.
	_programFlashes.texture("textureFlash", _texFlash, GL_TEXTURE_2D);
	
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
		glUniform2fv(_programKeyMajors["inverseScreenSize"], 1, &(invScreenSize[0]));
		glUniform3fv(_programKeyMajors["edgeColor"], 1, &(keyColor[0]));
		glUniform3fv(_programKeyMajors["majorColor"], GLsizei(majorColors.size()), &(majorColors[0][0]));
		glUniform1i(_programKeyMajors["highlightKeys"], int(highlightKeys));
		glUniform1iv(_programKeyMajors["actives"], GLsizei(_actives.size()), &(_actives[0]));

		// Draw the geometry.
		glBindVertexArray(_vaoKeyboard);
		glDrawElements(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0);
	}

	{
		_programKeyMinors.use();
		// Uniforms setup.
		glUniform2fv(_programKeyMinors["inverseScreenSize"], 1, &(invScreenSize[0]));
		glUniform3fv(_programKeyMinors["edgeColor"], 1, &(keyColor[0]));
		glUniform3fv(_programKeyMinors["minorColor"], GLsizei(minorColors.size()), &(minorColors[0][0]));
		glUniform1i(_programKeyMinors["highlightKeys"], int(highlightKeys));
		glUniform1iv(_programKeyMinors["actives"], GLsizei(_actives.size()), &(_actives[0]));

		glBindVertexArray(_vaoKeyboard);
		glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, 53);
	}
	
	glBindVertexArray(0);
	glUseProgram(0);
}

void MIDIScene::drawPedals(float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight, bool horizontalMode) {

	glEnable(GL_BLEND);
	_programPedals.use();
	glDisable(GL_CULL_FACE);

	// Adjust for aspect ratio.
	const float rat = invScreenSize.y/invScreenSize.x;
	const glm::vec2 scale = state.size * (rat < 1.0f ? glm::vec2(1.0f, rat) : glm::vec2(1.0f/rat, 1.0f));
	const float extraHorizFix = state.merge ? 0.4f : 1.0f;

	// Mode: top left, bottom left, top right, bottom right
	const int mode = int(state.location);
	const bool isTop = (mode % 2 == 0);
	const float vertSign = mode % 2 == 0 ? 1.0f : -1.0f;
	const float horizSign = mode < 2 ? -1.0f : 1.0f;

	// Extra vertical shift for the horizontal pedal when at the top.
	const glm::vec2 propShift = glm::vec2(extraHorizFix * 1.25f,
							(isTop && !state.merge) ? 1.2f : 0.8f) * scale;

	glm::vec2 shift = glm::vec2(horizSign, vertSign) * (1.0f - propShift);
	// If at the bottom, shift above the keyboard.
	if(horizontalMode){
		if(mode < 2){
			shift[0] += 2.0f * keyboardHeight;
		}
	} else {
		if(mode % 2 == 1){
			shift[1] += 2.0f * keyboardHeight;
		}
	}

	// Uniforms setup.
	glUniform3fv(_programPedals["pedalColor"], 1, &(state.color[0]));
	glUniform2fv(_programPedals["scale"], 1, &(scale[0]));
	glUniform2fv(_programPedals["shift"], 1, &(shift[0]));
	glUniform1f(_programPedals["pedalOpacity"], state.opacity);
	// sostenuto, damper, soft
	glUniform4f(_programPedals["pedalFlags"], _pedals.sostenuto, _pedals.damper, _pedals.soft, _pedals.expression);
	glUniform1i(_programPedals["mergePedals"], state.merge ? 1 : 0);

	// Draw the geometry.
	glBindVertexArray(_vaoPedals);
	glDrawElements(GL_TRIANGLES, int(_countPedals), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void MIDIScene::drawWaves(float time, const glm::vec2 & invScreenSize, const State::WaveState & state, float keyboardHeight) {

	glEnable(GL_BLEND);
	_programWave.use();
	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

	// Uniforms setup.
	glUniform3fv(_programWave["waveColor"], 1, &(state.color[0]));
	glUniform1f(_programWave["keyboardSize"], keyboardHeight);
	glUniform1f(_programWave["waveOpacity"], state.opacity);
	glUniform1f(_programWave["spread"], state.spread);

	glBindVertexArray(_vaoWave);

	// Fixed initial parameters.
	const float ampls[4] = {-0.023f, -0.011f, 0.017f, 0.009f};
	const float freqs[4] = {10.3f, -8.27f, -4.4f, 13.7f};
	const float phases[4] = {5.2f, 4.7f, 9.3f, -7.1f};
	// Render multiple waves with additive blending.
	for(int i = 0; i < 4; ++i){
		const float ampl = state.amplitude * ampls[i];
		const float freq = state.frequency * freqs[i];
		const float phase = phases[i] * time + float(i+1) * 7.39f;
		glUniform1f(_programWave["amplitude"], ampl);
		glUniform1f(_programWave["freq"], freq);
		glUniform1f(_programWave["phase"], phase);
		glDrawElements(GL_TRIANGLES, int(_countWave), GL_UNSIGNED_INT, (void*)0);
	}

	glBindVertexArray(0);
	glUseProgram(0);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glDisable(GL_BLEND);
}

void MIDIScene::setMinMaxKeys(int minKey, int minKeyMajor, int notesCount){
	_programNotes.use();
	glUniform1i(_programNotes["minNoteMajor"], minKeyMajor);
	 glUniform1f(_programNotes["notesCount"], float(notesCount));
	_programFlashes.use();
	glUniform1i(_programFlashes["minNote"], minKey);
	glUniform1f(_programFlashes["notesCount"], float(notesCount));
	_programKeyMajors.use();
	glUniform1i(_programKeyMajors["minNoteMajor"], minKeyMajor);
	glUniform1f(_programKeyMajors["notesCount"], float(notesCount));
	_programKeyMinors.use();
	glUniform1i(_programKeyMinors["minNoteMajor"], minKeyMajor);
	glUniform1f(_programKeyMinors["notesCount"], float(notesCount));
	_programParticules.use();
	glUniform1i(_programParticules["minNote"], minKey);
	glUniform1f(_programParticules["notesCount"], float(notesCount));
	glUseProgram(0);
}


void MIDIScene::setOrientation(bool horizontal){
	const int val = horizontal ? 1 : 0;
	_programNotes.use();
	glUniform1i(_programNotes["horizontalMode"], val);
	_programFlashes.use();
	glUniform1i(_programFlashes["horizontalMode"], val);
	_programKeyMajors.use();
	glUniform1i(_programKeyMajors["horizontalMode"], val);
	_programKeyMinors.use();
	glUniform1i(_programKeyMinors["horizontalMode"], val);
	_programParticules.use();
	glUniform1i(_programParticules["horizontalMode"], val);
	_programWave.use();
	glUniform1i(_programWave["horizontalMode"], val);
	glUseProgram(0);
}

void MIDIScene::clean(){
	glDeleteVertexArrays(1, &_vao);
	glDeleteVertexArrays(1, &_vaoFlashes);
	glDeleteVertexArrays(1, &_vaoParticles);
	// TODO: (MV) clear programs
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
