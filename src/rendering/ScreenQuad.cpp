#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "ScreenQuad.h"

ScreenQuad::ScreenQuad(){}

ScreenQuad::~ScreenQuad(){}

void ScreenQuad::init(GLuint textureId, const std::string & fragName, const std::string & vertName){
	init(fragName, vertName);
	// Link the texture of the framebuffer for this program.
	_textureId = textureId;
	checkGLError();
}

void ScreenQuad::init(const std::string & fragName, const std::string & vertName) {

	// Load the shaders
	_programId = createGLProgramFromStrings(ResourcesManager::getStringForShader(vertName), ResourcesManager::getStringForShader(fragName));

	// Load geometry.
	std::vector<float> quadVertices{ -1.0, -1.0,  0.0,
		1.0, -1.0,  0.0,
		-1.0,  1.0,  0.0,
		1.0,  1.0,  0.0
	};

	// Array to store the indices of the vertices to use.
	std::vector<unsigned int> quadIndices{ 0, 1, 2, 2, 1, 3 };

	_count = quadIndices.size();
	// Create an array buffer to host the geometry data.
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Upload the data to the Array buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * quadVertices.size(), &(quadVertices[0]), GL_STATIC_DRAW);

	// Generate a vertex array (useful when we add other attributes to the geometry).
	_vao = 0;
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	// The first attribute will be the vertices positions.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// We load the indices data
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * quadIndices.size(), &(quadIndices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);

	// Link the texture of the framebuffer for this program.
	glUseProgram(_programId);
	GLuint texUniID = glGetUniformLocation(_programId, "screenTexture");
	glUniform1i(texUniID, 0);
	glUseProgram(0);
	checkGLError();
}

void ScreenQuad::draw(float time) {

	draw(_textureId, time);
}

void ScreenQuad::draw(float time, glm::vec2 invScreenSize){
	
	draw(_textureId, time, invScreenSize);
}

void ScreenQuad::draw(GLuint texId, float time) {

	// Select the program (and shaders).
	glUseProgram(_programId);

	GLuint timeID = glGetUniformLocation(_programId, "time");
	glUniform1f(timeID, time);

	// Active screen texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);

	// Select the geometry.
	glBindVertexArray(_vao);
	// Draw!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glDrawElements(GL_TRIANGLES, GLsizei(_count), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void ScreenQuad::draw(GLuint texid, float time, glm::vec2 invScreenSize) {

	// Select the program (and shaders).
	glUseProgram(_programId);

	// Inverse screen size uniform.
	GLuint screenId = glGetUniformLocation(_programId, "inverseScreenSize");
	glUniform2fv(screenId, 1, &(invScreenSize[0]));

	draw(texid, time);
}



void ScreenQuad::clean(){
	glDeleteVertexArrays(1, &_vao);
}


