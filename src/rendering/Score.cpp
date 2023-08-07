#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "Score.h"

Score::Score(double secondsPerMeasure){
	
	// Load font atlas.
	GLuint textureId = ResourcesManager::getTextureFor("font");

	ScreenQuad::init(textureId, "background_frag", "background_vert");
	
	// Load additional data.
	_program.use();
	_program.uniform("secondsPerMeasure", float(secondsPerMeasure));
	glUseProgram(0);

}

void Score::setScaleAndMinorWidth(const float scale, const float width){
	_program.use();
	_program.uniform("mainSpeed", scale);
	_program.uniform("minorsWidth", width);
	glUseProgram(0);
}

void Score::setDisplay(const bool digits, const bool horiz, const bool vert){
	_program.use();
	_program.uniform("useDigits", digits);
	_program.uniform("useHLines", horiz);
	_program.uniform("useVLines", vert);
	glUseProgram(0);
}

void Score::setColors(const glm::vec3 & linesColor, const glm::vec3 & textColor, const glm::vec3 & keysColor){
	_program.use();
	_program.uniform("linesColor", linesColor);
	_program.uniform("textColor" , textColor);
	glUseProgram(0);
}

void Score::setKeyboardSize(float keyboardHeight){
	_program.use();
	_program.uniform("keyboardHeight", keyboardHeight);
	glUseProgram(0);
}

void Score::setMinMaxKeys(int minKey, int minKeyMajor, int notesCount){
	_program.use();
	_program.uniform("minNoteMajor", minKeyMajor);
	_program.uniform("notesCount", float(notesCount));
	glUseProgram(0);
}

void Score::setPlayDirection(bool reverse){
	_program.use();
	_program.uniform("reverseMode", reverse);
	glUseProgram(0);
}

void Score::setOrientation(bool horizontal){
	_program.use();
	_program.uniform("horizontalMode", horizontal);
	glUseProgram(0);
}

