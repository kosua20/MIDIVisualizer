#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "MIDIScene.h"

#ifdef _WIN32
#undef MIN
#undef MAX
#endif

MIDIScene::~MIDIScene(){}

MIDIScene::MIDIScene(){
	std::vector<float> data(5, 0.0f);
	renderSetup(data);
}

MIDIScene::MIDIScene(const std::string & midiFilePath){
	
	// MIDI processing.
	_midiFile = MIDIFile(midiFilePath);
	
	
	// Load geometry and notes shared data.
	std::vector<float> data;
	std::vector<MIDINote> notesM;
	_midiFile.getNotes(notesM, NoteType::MAJOR, 0);
	for(auto& note : notesM){
		data.push_back(float(note.note));
		data.push_back(float(note.start));
		data.push_back(float(note.duration));
		data.push_back(0.0f);
		data.push_back(float((std::min)(int(note.channel), CHANNELS_COUNT-1)));
	}

	std::vector<MIDINote> notesm;
	_midiFile.getNotes(notesm, NoteType::MINOR, 0);
	for(auto& note : notesm){
		data.push_back(float(note.note));
		data.push_back(float(note.start));
		data.push_back(float(note.duration));
		data.push_back(1.0f);
		data.push_back(float((std::min)(int(note.channel), CHANNELS_COUNT-1)));
	}
	// Upload to the GPU.
	renderSetup(data);
}

void MIDIScene::renderSetup(const std::vector<float> & data){

	std::vector<float> vertices = {-0.5,-0.5, 0.5, -0.5, 0.5,0.5, -0.5, 0.5};
	std::vector<unsigned int> indices = {0, 1, 3, 3, 1, 2};
	_primitiveCount = indices.size();
	// Create an array buffer to host the geometry data.
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Upload the data to the Array buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * 2, &(vertices[0]), GL_STATIC_DRAW);

	// Notes buffer.
	GLuint dataBufferId0 = 0;
	glGenBuffers(1, &dataBufferId0);
	glBindBuffer(GL_ARRAY_BUFFER, dataBufferId0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), &(data[0]), GL_STATIC_DRAW);

	// Enabled notes buffer (empty for now).
	_flagsBufferId = 0;
	glGenBuffers(1, &_flagsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _flagsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 88, NULL, GL_DYNAMIC_DRAW);

	_uboKeyboard = 0;
	glGenBuffers(1, &_uboKeyboard);
	glBindBuffer(GL_UNIFORM_BUFFER, _uboKeyboard);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(int)*88, NULL, GL_DYNAMIC_DRAW);
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
	glBindBuffer(GL_ARRAY_BUFFER, dataBufferId0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glVertexAttribDivisor(1, 1);

	// The second attribute will be the notes data.
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, dataBufferId0);
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

	// Prepare actives notes array.
	_actives = std::vector<int>(88, -1);
	_previousTime = 0.0;
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

void MIDIScene::updatesActiveNotes(double time){
	// Update the particle systems lifetimes.
	for(auto & particle : _particles){
		// Give a bit of a head start to the animation.
		particle.elapsed = (float(time) - particle.start + 0.25f) / particle.duration;
		// Disable old particles.
		if(float(time) >= particle.start + particle.duration){
			particle.note = -1;
			particle.channel = -1;
			particle.duration = particle.start = particle.elapsed = 0.0f;
		}
	}
	// Get notes actives.
	auto actives = std::vector<ActiveNoteInfos>(88);
	_midiFile.getNotesActive(actives, time, 0);
	for(int i = 0; i < 88; ++i){
		const auto & note = actives[i];
		const int clamped = (std::min)(note.channel, CHANNELS_COUNT-1);
		_actives[i] = note.enabled ? clamped : -1;
		// Check if the note was triggered at this frame.
		if(note.start > _previousTime && note.start <= time){
			// Find an available particles system and update it with the note parameters.
			for(auto & particle : _particles){
				if(particle.note < 0){
					// Update with new note parameter.
					particle.duration = (std::max)(note.duration*2.0f, note.duration + 1.2f);
					particle.start = note.start;
					particle.note = i;
					particle.channel = clamped;
					particle.elapsed = 0.0f;
					break;
				}
			}
		}
	}
	_previousTime = time;
}

void MIDIScene::resetParticles() {
	for (auto & particle : _particles) {
		particle.note = -1;
		particle.channel = -1;
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

	glUniform3fv(colorId, state.colors.size(), &state.colors[0][0]);
	
	// Particles trajectories texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texParticles);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, state.tex);
	GLuint texCountId = glGetUniformLocation(_programParticulesId, "texCount");
	glUniform1i(texCountId, state.texCount);

	// Select the geometry.
	glBindVertexArray(_vaoParticles);
	// For each activ particles system, draw it with the right parameters.
	for(const auto & particle : _particles){
		if(particle.note >= 0){
			glUniform1i(globalShiftId, particle.note);
			glUniform1f(timeId, particle.elapsed);
			glUniform1f(durationId, particle.duration);
			glUniform1i(channelId, particle.channel);
			glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, state.count);
		}
	}
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);

}

void MIDIScene::drawNotes(float time, const glm::vec2 & invScreenSize, const ColorArray & majorColors, const ColorArray & minorColors, bool prepass){
	
	glUseProgram(_programId);
	
	// Uniforms setup.
	GLuint screenId = glGetUniformLocation(_programId, "inverseScreenSize");
	GLuint timeId = glGetUniformLocation(_programId, "time");
	GLuint colorId = glGetUniformLocation(_programId, "baseColor");
	GLuint colorMinId = glGetUniformLocation(_programId, "minorColor");
	GLuint colorScaleId = glGetUniformLocation(_programId, "colorScale");
	glUniform2fv(screenId,1, &(invScreenSize[0]));
	glUniform1f(timeId,time);
	glUniform1f(colorScaleId, prepass ? 0.6f: 1.0f);

	glUniform3fv(colorId, majorColors.size(), &(majorColors[0][0]));
	glUniform3fv(colorMinId, minorColors.size(), &(minorColors[0][0]));
	
	
	// Draw the geometry.
	glBindVertexArray(_vao);
	glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, GLsizei(_midiFile.notesCount()));

	glBindVertexArray(0);
	glUseProgram(0);
	
}

void MIDIScene::drawFlashes(float time, const glm::vec2 & invScreenSize, const ColorArray & baseColors, float userScale){
	
	// Need alpha blending.
	glEnable(GL_BLEND);
	
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
	glUniform3fv(colorId, baseColors.size(), &(baseColors[0][0]));
	glUniform1f(scaleId,userScale);
	// Flash texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texFlash);
	
	// Draw the geometry.
	glBindVertexArray(_vaoFlashes);
	glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, 88);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	
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
	glUniform3fv(majorId, majorColors.size(), &(majorColors[0][0]));
	glUniform3fv(minorId, minorColors.size(), &(minorColors[0][0]));
	glUniform1i(highId, int(highlightKeys));

	glBindBuffer(GL_UNIFORM_BUFFER, _uboKeyboard);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _uboKeyboard);

	// Draw the geometry.
	glBindVertexArray(_vaoKeyboard);
	glDrawElements(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
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
