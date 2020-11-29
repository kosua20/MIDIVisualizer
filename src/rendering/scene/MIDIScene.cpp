#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "../../helpers/ProgramUtilities.h"
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
	std::vector<unsigned int> indices = {0, 1, 3, 3, 1, 2};
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 128, NULL, GL_DYNAMIC_DRAW);

	_uboKeyboard = 0;
	glGenBuffers(1, &_uboKeyboard);
	glBindBuffer(GL_UNIFORM_BUFFER, _uboKeyboard);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * 128, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// Programs.

	// Notes shaders.
	_programId = createGLProgramFromStrings(ResourcesManager::getStringForShader("notes_vert"), ResourcesManager::getStringForShader("notes_frag"));

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
	_programFlashesId = createGLProgramFromStrings(ResourcesManager::getStringForShader("flashes_vert"), ResourcesManager::getStringForShader("flashes_frag"));

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
	glUseProgram(_programFlashesId);
	glActiveTexture(GL_TEXTURE0);
	GLuint texUniID = glGetUniformLocation(_programFlashesId, "textureFlash");
	glUniform1i(texUniID, 0);
	glUseProgram(0);


	// Particles program.

	_programParticulesId = createGLProgramFromStrings(ResourcesManager::getStringForShader("particles_vert"), ResourcesManager::getStringForShader("particles_frag"));

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
	glUseProgram(_programParticulesId);
	glActiveTexture(GL_TEXTURE0);
	GLuint texUniID1 = glGetUniformLocation(_programParticulesId, "textureParticles");
	glUniform1i(texUniID1, 0);

	glUseProgram(_programParticulesId);
	glActiveTexture(GL_TEXTURE1);
	GLuint texUniID2 = glGetUniformLocation(_programParticulesId, "lookParticles");
	glUniform1i(texUniID2, 1);

	// Pass texture size to shader.
	const glm::vec2 tsize = ResourcesManager::getTextureSizeFor("particles");
	GLuint texSizeID = glGetUniformLocation(_programParticulesId, "inverseTextureSize");
	glUniform2f(texSizeID, 1.0f/float(tsize[0]), 1.0f/float(tsize[1]));
	glUseProgram(0);

	// Keyboard setup.
	_programKeysId = createGLProgramFromStrings(ResourcesManager::getStringForShader("keys_vert"), ResourcesManager::getStringForShader("keys_frag"));
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
	const GLuint uboLoc = glGetUniformBlockIndex(_programKeysId, "ActiveNotes");
	glUniformBlockBinding(_programKeysId, uboLoc, 0);

	// Pedals setup.
	_programPedalsId = createGLProgramFromStrings(ResourcesManager::getStringForShader("pedal_vert"), ResourcesManager::getStringForShader("pedal_frag"));
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
	_programWaveId = createGLProgramFromStrings(ResourcesManager::getStringForShader("wave_vert"), ResourcesManager::getStringForShader("wave_frag"));
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
	glUseProgram(_programId);
	GLuint speedID = glGetUniformLocation(_programId, "mainSpeed");
	glUniform1f(speedID, scale);
	GLuint widthId = glGetUniformLocation(_programId, "minorsWidth");
	glUniform1f(widthId, minorWidth);
	glUseProgram(_programKeysId);
	GLuint widthId1 = glGetUniformLocation(_programKeysId, "minorsWidth");
	glUniform1f(widthId1, minorWidth);
	glUseProgram(0);
}

void MIDIScene::setParticlesParameters(const float speed, const float expansion){
	glUseProgram(_programParticulesId);
	GLuint id0 = glGetUniformLocation(_programParticulesId, "speedScaling");
	glUniform1f(id0, speed);
	GLuint id1 = glGetUniformLocation(_programParticulesId, "expansionFactor");
	glUniform1f(id1, expansion);
	glUseProgram(0);
}

void MIDIScene::setKeyboardSize(float keyboardHeight){
	glUseProgram(_programId);
	glUniform1f(glGetUniformLocation(_programId, "keyboardHeight"), keyboardHeight);
	glUseProgram(_programParticulesId);
	glUniform1f(glGetUniformLocation(_programParticulesId, "keyboardHeight"), keyboardHeight);
	glUseProgram(_programKeysId);
	glUniform1f(glGetUniformLocation(_programKeysId, "keyboardHeight"), keyboardHeight);
	glUseProgram(_programFlashesId);
	glUniform1f(glGetUniformLocation(_programFlashesId, "keyboardHeight"), keyboardHeight);
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
	glUseProgram(_programParticulesId);
	
	// Common uniforms values.
	GLuint screenId = glGetUniformLocation(_programParticulesId, "inverseScreenSize");
	GLuint timeId = glGetUniformLocation(_programParticulesId, "time");
	GLuint durationId = glGetUniformLocation(_programParticulesId, "duration");
	glUniform2fv(screenId,1, &(invScreenSize[0]));
	glUniform1f(timeId,0.0);

	// Variable uniforms.
	GLuint globalShiftId = glGetUniformLocation(_programParticulesId, "globalId");
	GLuint scaleId = glGetUniformLocation(_programParticulesId, "scale");
	GLuint colorId = glGetUniformLocation(_programParticulesId, "baseColor");
	GLuint channelId = glGetUniformLocation(_programParticulesId, "channel");
	
	// Prepass : bigger, darker particles.
	GLuint colorScaleId = glGetUniformLocation(_programParticulesId, "colorScale");
	glUniform1f(colorScaleId, prepass ? 0.6f : 1.6f);
	glUniform1f(scaleId, state.scale * (prepass ? 2.0f : 1.0f));

	glUniform3fv(colorId, GLsizei(state.colors.size()), &state.colors[0][0]);
	
	// Particles trajectories texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texParticles);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, state.tex);
	GLuint texCountId = glGetUniformLocation(_programParticulesId, "texCount");
	glUniform1i(texCountId, state.texCount);

	// Select the geometry.
	glBindVertexArray(_vaoParticles);
	// For each active particles system, draw it with the right parameters.
	for(const auto & particle : _particles){
		if(particle.note >= 0){
			glUniform1i(globalShiftId, particle.note);
			glUniform1f(timeId, particle.elapsed);
			glUniform1f(durationId, particle.duration);
			glUniform1i(channelId, particle.set);
			glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, state.count);
		}
	}
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);

}

void MIDIScene::drawNotes(float time, const glm::vec2 & invScreenSize, const ColorArray & majorColors, const ColorArray & minorColors, bool reverseScroll, bool prepass){
	
	glUseProgram(_programId);
	
	// Uniforms setup.
	GLuint screenId = glGetUniformLocation(_programId, "inverseScreenSize");
	GLuint timeId = glGetUniformLocation(_programId, "time");
	GLuint colorId = glGetUniformLocation(_programId, "baseColor");
	GLuint colorMinId = glGetUniformLocation(_programId, "minorColor");
	GLuint colorScaleId = glGetUniformLocation(_programId, "colorScale");
	GLuint reverseId = glGetUniformLocation(_programId, "reverseMode");
	glUniform2fv(screenId,1, &(invScreenSize[0]));
	glUniform1f(timeId, time);
	glUniform1f(colorScaleId, prepass ? 0.6f: 1.0f);

	glUniform3fv(colorId, GLsizei(majorColors.size()), &(majorColors[0][0]));
	glUniform3fv(colorMinId, GLsizei(minorColors.size()), &(minorColors[0][0]));
	glUniform1i(reverseId, reverseScroll ? 1 : 0);
	
	// Draw the geometry.
	glBindVertexArray(_vao);
	glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, GLsizei(_dataBufferSubsize));

	glBindVertexArray(0);
	glUseProgram(0);
	
}

void MIDIScene::drawFlashes(float time, const glm::vec2 & invScreenSize, const ColorArray & baseColors, float userScale){
	
	// Need alpha blending.
	glEnable(GL_BLEND);

	glBlendFunc(GL_ONE, GL_ONE);
	// Update the flags buffer accordingly.
	glBindBuffer(GL_ARRAY_BUFFER, _flagsBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _actives.size()*sizeof(int) ,&(_actives[0]));
	
	glUseProgram(_programFlashesId);
	
	// Uniforms setup.
	GLuint screenId1 = glGetUniformLocation(_programFlashesId, "inverseScreenSize");
	GLuint timeId1 = glGetUniformLocation(_programFlashesId, "time");
	GLuint colorId = glGetUniformLocation(_programFlashesId, "baseColor");
	GLuint scaleId = glGetUniformLocation(_programFlashesId, "userScale");
	glUniform2fv(screenId1,1, &(invScreenSize[0]));
	glUniform1f(timeId1,time);
	glUniform3fv(colorId, GLsizei(baseColors.size()), &(baseColors[0][0]));
	glUniform1f(scaleId,userScale);
	// Flash texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texFlash);
	
	// Draw the geometry.
	glBindVertexArray(_vaoFlashes);
	glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, 128);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MIDIScene::drawKeyboard(float, const glm::vec2 & invScreenSize, const glm::vec3 & keyColor, const ColorArray & majorColors, const ColorArray & minorColors, bool highlightKeys) {
	// Upload active keys data.
	glBindBuffer(GL_UNIFORM_BUFFER, _uboKeyboard);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, _actives.size() * sizeof(int), &(_actives[0]));
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glUseProgram(_programKeysId);

	// Uniforms setup.
	const GLuint screenId1 = glGetUniformLocation(_programKeysId, "inverseScreenSize");
	const GLuint colorId = glGetUniformLocation(_programKeysId, "keysColor");
	const GLuint majorId = glGetUniformLocation(_programKeysId, "majorColor");
	const GLuint minorId = glGetUniformLocation(_programKeysId, "minorColor");
	const GLuint highId = glGetUniformLocation(_programKeysId, "highlightKeys");
	glUniform2fv(screenId1, 1, &(invScreenSize[0]));
	glUniform3fv(colorId, 1, &(keyColor[0]));
	glUniform3fv(majorId, GLsizei(majorColors.size()), &(majorColors[0][0]));
	glUniform3fv(minorId, GLsizei(minorColors.size()), &(minorColors[0][0]));
	glUniform1i(highId, int(highlightKeys));

	glBindBuffer(GL_UNIFORM_BUFFER, _uboKeyboard);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _uboKeyboard);

	// Draw the geometry.
	glBindVertexArray(_vaoKeyboard);
	glDrawElements(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void MIDIScene::drawPedals(float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight) {

	glEnable(GL_BLEND);
	glUseProgram(_programPedalsId);
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
	if(mode % 2 == 1){
		shift[1] += 2.0f * keyboardHeight;
	}


	// Uniforms setup.
	const GLuint colorId = glGetUniformLocation(_programPedalsId, "pedalColor");
	const GLuint opacityId = glGetUniformLocation(_programPedalsId, "pedalOpacity");
	const GLuint flagsId = glGetUniformLocation(_programPedalsId, "pedalFlags");
	const GLuint scaleId = glGetUniformLocation(_programPedalsId, "scale");
	const GLuint mergeId =  glGetUniformLocation(_programPedalsId, "mergePedals");
	const GLuint shiftId =  glGetUniformLocation(_programPedalsId, "shift");

	glUniform3fv(colorId, 1, &(state.color[0]));
	glUniform2fv(scaleId, 1, &(scale[0]));
	glUniform2fv(shiftId, 1, &(shift[0]));
	glUniform1f(opacityId, state.opacity);
	// sostenuto, damper, soft
	glUniform4f(flagsId, _pedals.sostenuto, _pedals.damper, _pedals.soft, _pedals.expression);
	glUniform1i(mergeId, state.merge ? 1 : 0);

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
	glUseProgram(_programWaveId);
	glBlendFunc(GL_ONE, GL_ONE);
	// Uniforms setup.
	const GLuint colorId = glGetUniformLocation(_programWaveId, "waveColor");
	const GLuint opacityId = glGetUniformLocation(_programWaveId, "waveOpacity");
	const GLuint spreadId = glGetUniformLocation(_programWaveId, "spread");
	const GLuint scaleId = glGetUniformLocation(_programWaveId, "amplitude");
	const GLuint freqId =  glGetUniformLocation(_programWaveId, "freq");
	const GLuint phaseId =  glGetUniformLocation(_programWaveId, "phase");
	const GLuint shiftId =  glGetUniformLocation(_programWaveId, "keyboardSize");

	glUniform3fv(colorId, 1, &(state.color[0]));
	glUniform1f(shiftId, keyboardHeight);
	glUniform1f(opacityId, state.opacity);
	glUniform1f(spreadId, state.spread);

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
		glUniform1f(scaleId, ampl);
		glUniform1f(freqId, freq);
		glUniform1f(phaseId, phase);
		glDrawElements(GL_TRIANGLES, int(_countWave), GL_UNSIGNED_INT, (void*)0);
	}

	glBindVertexArray(0);
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
}

void MIDIScene::setMinMaxKeys(int minKey, int minKeyMajor, int notesCount){
	glUseProgram(_programId);
	glUniform1i(glGetUniformLocation(_programId, "minNoteMajor"), minKeyMajor);
	glUniform1f(glGetUniformLocation(_programId, "notesCount"), float(notesCount));
	glUseProgram(_programFlashesId);
	glUniform1i(glGetUniformLocation(_programFlashesId, "minNote"), minKey);
	glUniform1f(glGetUniformLocation(_programFlashesId, "notesCount"), float(notesCount));
	glUseProgram(_programKeysId);
	glUniform1i(glGetUniformLocation(_programKeysId, "minNoteMajor"), minKeyMajor);
	glUniform1f(glGetUniformLocation(_programKeysId, "notesCount"), float(notesCount));
	glUseProgram(_programParticulesId);
	glUniform1i(glGetUniformLocation(_programParticulesId, "minNote"), minKey);
	glUniform1f(glGetUniformLocation(_programParticulesId, "notesCount"), float(notesCount));
	glUseProgram(0);
}

void MIDIScene::clean(){
	glDeleteVertexArrays(1, &_vao);
	glDeleteVertexArrays(1, &_vaoFlashes);
	glDeleteVertexArrays(1, &_vaoParticles);
	glDeleteProgram(_programId);
	glDeleteProgram(_programFlashesId);
	glDeleteProgram(_programParticulesId);
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

MIDISceneEmpty::~MIDISceneEmpty(){

}
