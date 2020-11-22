#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "Score.h"

Score::Score(double secondsPerMeasure){
	
	// Load font atlas.
	GLuint textureId = ResourcesManager::getTextureFor("font");

	ScreenQuad::init(textureId, "background_frag");
	
	// Load additional data.
	glUseProgram(_programId);
	GLuint sigID = glGetUniformLocation(_programId, "secondsPerMeasure");
	glUniform1f(sigID, float(secondsPerMeasure));
	glUseProgram(0);

}

void Score::setScaleAndMinorWidth(const float scale, const float width){
	glUseProgram(_programId);
	GLuint speedID = glGetUniformLocation(_programId, "mainSpeed");
	glUniform1f(speedID, scale);
	GLuint widthId = glGetUniformLocation(_programId, "minorsWidth");
	glUniform1f(widthId, width);
	glUseProgram(0);
}

void Score::setDisplay(const bool digits, const bool horiz, const bool vert){
	glUseProgram(_programId);
	GLuint id1 = glGetUniformLocation(_programId, "useDigits"); glUniform1i(id1, digits);
	GLuint id2 = glGetUniformLocation(_programId, "useHLines"); glUniform1i(id2, horiz);
	GLuint id3 = glGetUniformLocation(_programId, "useVLines"); glUniform1i(id3, vert);
	glUseProgram(0);
}

void Score::setColors(const glm::vec3 & linesColor, const glm::vec3 & textColor, const glm::vec3 & keysColor){
	glUseProgram(_programId);
	GLuint id1 = glGetUniformLocation(_programId, "linesColor"); glUniform3fv(id1, 1, &linesColor[0]);
	GLuint id2 = glGetUniformLocation(_programId, "textColor"); glUniform3fv(id2, 1, &textColor[0]);
	GLuint id3 = glGetUniformLocation(_programId, "keysColor"); glUniform3fv(id3, 1, &keysColor[0]);
	glUseProgram(0);
}

void Score::setKeyboardSize(float keyboardHeight){
	glUseProgram(_programId);
	glUniform1f(glGetUniformLocation(_programId, "keyboardHeight"), keyboardHeight);
	glUseProgram(0);
}

void Score::setMinMaxKeys(int minKey, int minKeyMajor, int notesCount){
	glUseProgram(_programId);
	glUniform1i(glGetUniformLocation(_programId, "minNoteMajor"), minKeyMajor);
	glUniform1f(glGetUniformLocation(_programId, "notesCount"), float(notesCount));
	glUseProgram(0);
}

void Score::setPlayDirection(bool reverse){
	glUseProgram(_programId);
	glUniform1i(glGetUniformLocation(_programId, "reverseMode"), reverse ? 1 : 0);
	glUseProgram(0);
}

