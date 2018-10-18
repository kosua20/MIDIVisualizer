#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "Background.h"

Background::Background(double secondsPerMeasure, const float scale){
	
	// Load font atlas.
	GLuint textureId = ResourcesManager::getTextureFor("font");

	ScreenQuad::init(textureId, ResourcesManager::getStringForShader("background_frag"));
	
	// Load additional data.
	glUseProgram(_programId);
	GLuint sigID = glGetUniformLocation(_programId, "secondsPerMeasure");
	glUniform1f(sigID, secondsPerMeasure);
	GLuint speedID = glGetUniformLocation(_programId, "mainSpeed");
	glUniform1f(speedID, scale);
	glUseProgram(0);
	
}

void Background::setScale(const float scale){
	glUseProgram(_programId);
	GLuint speedID = glGetUniformLocation(_programId, "mainSpeed");
	glUniform1f(speedID, scale);
	glUseProgram(0);
}

void Background::setMinorWidth(const float width){
	glUseProgram(_programId);
	GLuint widthId = glGetUniformLocation(_programId, "minorsWidth");
	glUniform1f(widthId, width);
	glUseProgram(0);
}

void Background::setDisplay(const bool digits, const bool horiz, const bool vert, const bool keys){
	glUseProgram(_programId);
	GLuint id1 = glGetUniformLocation(_programId, "useDigits"); glUniform1i(id1, digits);
	GLuint id2 = glGetUniformLocation(_programId, "useHLines"); glUniform1i(id2, horiz);
	GLuint id3 = glGetUniformLocation(_programId, "useVLines"); glUniform1i(id3, vert);
	GLuint id4 = glGetUniformLocation(_programId, "useKeys"); glUniform1i(id4, keys);
	glUseProgram(0);
}

