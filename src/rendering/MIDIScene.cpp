#include <stdio.h>
#include <iostream>
#include <vector>
#include <lodepng/lodepng.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/MeshUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "MIDIScene.h"


MIDIScene::~MIDIScene(){}

MIDIScene::MIDIScene(const std::string & midiFilePath, const glm::vec3& baseColor, const float scale){
	
	// MIDI processing.
	_midiFile = MIDIFile(midiFilePath);
	
	// Color.
	_baseColor = baseColor;
	
	// Data.
	
	// Load geometry and notes shared data.
	std::vector<float> vertices = {-0.5,-0.5, 0.5, -0.5, 0.5,0.5, -0.5, 0.5};
	std::vector<unsigned int> indices = {0, 1, 3, 3, 1, 2};
	_primitiveCount = indices.size();
	
	std::vector<float> data;
	auto notesM = _midiFile.tracks[0].getNotes(majorNotes);
	for(auto& note : notesM){
		data.push_back(float(note.note));
		data.push_back(note.start);
		data.push_back(note.duration);
		data.push_back(0.0);
	}
	auto notesm = _midiFile.tracks[0].getNotes(minorNotes);
	for(auto& note : notesm){
		data.push_back(float(note.note));
		data.push_back(note.start);
		data.push_back(note.duration);
		data.push_back(1.0);
	}
	_notesCount = notesM.size() + notesm.size();
	
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint) * 88, NULL, GL_DYNAMIC_DRAW);
	
	
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
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(1, 1);
	
	// We load the indices data
	glGenBuffers(1, &_ebo);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);
	checkGLError();
	
	GLuint speedID = glGetUniformLocation(_programId, "mainSpeed");
	glUniform1f(speedID, scale);
	
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
	glVertexAttribPointer(1, 1, GL_INT, GL_FALSE, 0, NULL);
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
	
	// Pass texture size to shader.
	const glm::vec2 tsize = ResourcesManager::getTextureSizeFor("particles");
	GLuint texSizeID = glGetUniformLocation(_programParticulesId, "inverseTextureSize");
	glUniform2f(texSizeID,1.0/float(tsize[0]), 1.0/float(tsize[1]));
	glUseProgram(0);
	
	// Prepare actives notes array.
	_actives = std::vector<int>(88, 0);
	_particles = std::vector<Particles>(256);
}

void MIDIScene::setScale(const float scale){
	glUseProgram(_programId);
	GLuint speedID = glGetUniformLocation(_programId, "mainSpeed");
	glUniform1f(speedID, scale);
	glUseProgram(0);
}

void MIDIScene::updatesActiveNotes(double time, double delta){
	// Update the particle systems lifetimes.
	for(auto & particle : _particles){
		particle.elapsed += delta;
		if(particle.elapsed >= particle.duration){
			particle.note = -1;
			particle.duration = particle.elapsed = 0.0f;
		}
	}
	// Get notes actives.
	_midiFile.getNotesActive(_actives, time, 0);
	for(int i = 0; i < 88; ++i){
		// Check if the note was triggered again.
		if(!_activesLastFrame[i] && _actives[i]){
			// Find an available particles system and update it with the note parameters.
			for(auto & particle : _particles){
				if(particle.note < 0){
					// Update with new note parameter.
					/*auto& note = tracks[track].notes[i];
					double newDuration = std::max(note.duration*2.0, note.duration + 1.2);
					if(note.start - 0.25 <= time && note.start + newDuration >= time){
						actives[note.note] = std::make_pair((time - note.start + 0.25) / newDuration, newDuration);
					}*/
					particle.note = i;
					break;
				}
			}
		}

		// Update status for next frame.
		_activesLastFrame[i] = _actives[i];
	}
}

void MIDIScene::drawParticles(float time, glm::vec2 invScreenSize, bool prepass){

	// Need alpha blending.
	glEnable(GL_BLEND);

	glUseProgram(_programParticulesId);
	
	// Common uniforms values.
	GLuint screenId = glGetUniformLocation(_programParticulesId, "inverseScreenSize");
	GLuint timeId = glGetUniformLocation(_programParticulesId, "time");
	GLuint durationId = glGetUniformLocation(_programParticulesId, "duration");
	glUniform2fv(screenId,1, &(invScreenSize[0]));
	glUniform1f(timeId,time);
	glUniform1f(timeId,0.0);

	// Variable uniforms.
	GLuint globalShiftId = glGetUniformLocation(_programParticulesId, "globalId");
	GLuint scaleId = glGetUniformLocation(_programParticulesId, "scale");
	GLuint colorId = glGetUniformLocation(_programParticulesId, "baseColor");
	
	// Prepass : bigger, darker particles.
	if(prepass){
		glUniform1f(scaleId,2.0);
		glUniform3f(colorId,0.6*_baseColor[0],0.6*_baseColor[1],0.6*_baseColor[2]);
	} else {
		glUniform1f(scaleId,1.0);
		glUniform3f(colorId,1.6*_baseColor[0],1.6*_baseColor[1],1.6*_baseColor[2]);
	}
	
	// Particles trajectories texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texParticles);
	
	// Select the geometry.
	glBindVertexArray(_vaoParticles);
	
	// For each active note, set uniforms and draw particles system.
	for(const auto & particle : _particles){
		if(particle.note >= 0){
			glUniform1i(globalShiftId, particle.note);
			glUniform1f(timeId, particle.elapsed);
			glUniform1f(durationId, particle.duration);
			//glUniform1f(timeId, _actives[i].first);
			//glUniform1f(durationId, _actives[i].second);
			glDrawElementsInstanced(GL_TRIANGLES, _primitiveCount, GL_UNSIGNED_INT, (void*)0, 500);
		}
	}
	
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);

}


void MIDIScene::draw(float time, glm::vec2 invScreenSize){
	
	glUseProgram(_programId);
	
	// Uniforms setup.
	GLuint screenId = glGetUniformLocation(_programId, "inverseScreenSize");
	GLuint timeId = glGetUniformLocation(_programId, "time");
	GLuint colorId = glGetUniformLocation(_programId, "baseColor");
	glUniform2fv(screenId,1, &(invScreenSize[0]));
	glUniform1f(timeId,time);
	glUniform3fv(colorId, 1, &(_baseColor[0]));
	
	// Draw the geometry.
	glBindVertexArray(_vao);
	glDrawElementsInstanced(GL_TRIANGLES, _primitiveCount, GL_UNSIGNED_INT, (void*)0, _notesCount);

	glBindVertexArray(0);
	glUseProgram(0);
	
}

void MIDIScene::drawFlashes(float time, glm::vec2 invScreenSize){
	
	// Need alpha blending.
	glEnable(GL_BLEND);
	
	// Get notes actives at exactly the current time.
	
	
	// Update the flags buffer accordingly.
	glBindBuffer(GL_ARRAY_BUFFER, _flagsBufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, _actives.size()*sizeof(GLint) ,&(_actives[0]));
	
	glUseProgram(_programFlashesId);
	
	// Uniforms setup.
	GLuint screenId1 = glGetUniformLocation(_programFlashesId, "inverseScreenSize");
	GLuint timeId1 = glGetUniformLocation(_programFlashesId, "time");
	GLuint colorId = glGetUniformLocation(_programFlashesId, "baseColor");
	glUniform2fv(screenId1,1, &(invScreenSize[0]));
	glUniform1f(timeId1,time);
	glUniform3fv(colorId, 1, &(_baseColor[0]));
	
	// Flash texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texFlash);
	
	// Draw the geometry.
	glBindVertexArray(_vaoFlashes);
	glDrawElementsInstanced(GL_TRIANGLES, _primitiveCount, GL_UNSIGNED_INT, (void*)0, 88);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
	
}



void MIDIScene::clean(){
	glDeleteVertexArrays(1, &_vao);
	glDeleteVertexArrays(1, &_vaoFlashes);
	glDeleteVertexArrays(1, &_vaoParticles);
	//glDeleteTextures(1, &_texFlash);
	//glDeleteTextures(1, &_texParticles);
	glDeleteProgram(_programId);
	glDeleteProgram(_programFlashesId);
	glDeleteProgram(_programParticulesId);
}


