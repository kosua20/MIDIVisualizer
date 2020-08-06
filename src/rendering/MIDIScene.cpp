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
	renderSetup();
	upload(data);
}

MIDIScene::MIDIScene(const std::string & midiFilePath, const SetOptions & options) {
	
	// MIDI processing.
	_midiFile = MIDIFile(midiFilePath);

	renderSetup();

	updateSets(options);

	std::cout << "[INFO]: Final track duration " << _midiFile.duration() << " sec." << std::endl;
}


void MIDIScene::updateSets(const SetOptions & options){
	// Generate note data for rendering.
	_midiFile.updateSets(options);

	// Load notes shared data.
	std::vector<float> data;
	std::vector<MIDINote> notesM;
	_midiFile.getNotes(notesM, NoteType::MAJOR, 0);
	for(auto& note : notesM){
		data.push_back(float(note.note));
		data.push_back(float(note.start));
		data.push_back(float(note.duration));
		data.push_back(0.0f);
		data.push_back(float(note.set % CHANNELS_COUNT));
	}

	std::vector<MIDINote> notesm;
	_midiFile.getNotes(notesm, NoteType::MINOR, 0);
	for(auto& note : notesm){
		data.push_back(float(note.note));
		data.push_back(float(note.start));
		data.push_back(float(note.duration));
		data.push_back(1.0f);
		data.push_back(float(note.set % CHANNELS_COUNT));
	}
	// Upload to the GPU.
	upload(data);
}

void MIDIScene::upload(const std::vector<float> & data){
	glBindBuffer(GL_ARRAY_BUFFER, _dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), &(data[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

	// Prepare actives notes array.
	_actives.fill(-1);
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

void MIDIScene::updatesActiveNotes(double time){
	// Update the particle systems lifetimes.
	for(auto & particle : _particles){
		// Give a bit of a head start to the animation.
		particle.elapsed = (float(time) - particle.start + 0.25f) / particle.duration;
		// Disable old particles.
		if(float(time) >= particle.start + particle.duration){
			particle.note = -1;
			particle.set = -1;
			particle.duration = particle.start = particle.elapsed = 0.0f;
		}
	}
	// Get notes actives.
	auto actives = ActiveNotesArray();
	_midiFile.getNotesActive(actives, time, 0);
	for(int i = 0; i < 128; ++i){
		const auto & note = actives[i];
		const int clamped = note.set % CHANNELS_COUNT;
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
					particle.set = clamped;
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
			glUniform1i(channelId, particle.set);
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
	glDrawElementsInstanced(GL_TRIANGLES, int(_primitiveCount), GL_UNSIGNED_INT, (void*)0, 128);
	
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

void MIDIScene::drawPedals(float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight) {
	bool damper = false;
	bool sostenuto = false;
	bool soft = false;
	_midiFile.getPedalsActive(damper, sostenuto, soft, time, 0),

	glEnable(GL_BLEND);
	glUseProgram(_programPedalsId);
	glDisable(GL_CULL_FACE);

	// Adjust for aspect ratio.
	const float rat = invScreenSize.y/invScreenSize.x;
	const glm::vec2 scale = state.size * (rat < 1.0f ? glm::vec2(1.0f, rat) : glm::vec2(1.0f/rat, 1.0f));
	const float extraHorizFix = state.merge ? 0.4f : 1.0f;
	const glm::vec2 propShift = glm::vec2(extraHorizFix * 1.25f, 0.785f) * scale;
	// Mode: top left, bottom left, top right, bottom right
	const int mode = int(state.location);
	const float vertSign = mode % 2 == 0 ? 1.0f : -1.0f;
	const float horizSign = mode < 2 ? -1.0f : 1.0f;
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
	glUniform3i(flagsId, sostenuto, damper, soft);
	glUniform1i(mergeId, state.merge ? 1 : 0);

	// Draw the geometry.
	glBindVertexArray(_vaoPedals);
	glDrawElements(GL_TRIANGLES, int(_countPedals), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
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
