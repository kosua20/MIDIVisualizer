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
	glUniform1f(_program["secondsPerMeasure"], float(secondsPerMeasure));
	glUseProgram(0);

}

void Score::setScaleAndMinorWidth(const float scale, const float width){
	_program.use();
	glUniform1f(_program["mainSpeed"], scale);
	glUniform1f(_program["minorsWidth"], width);
	glUseProgram(0);
}

void Score::setDisplay(const bool digits, const bool horiz, const bool vert){
	_program.use();
	glUniform1i(_program["useDigits"], digits);
	glUniform1i(_program["useHLines"], horiz);
	glUniform1i(_program["useVLines"], vert);
	glUseProgram(0);
}

void Score::setColors(const glm::vec3 & linesColor, const glm::vec3 & textColor, const glm::vec3 & keysColor){
	_program.use();
	glUniform3fv(_program["linesColor"], 1, &linesColor[0]);
	glUniform3fv(_program["textColor" ], 1, &textColor[0]);
	//glUniform3fv(_program["keysColor" ], 1, &keysColor[0]);
	glUseProgram(0);
}

void Score::setKeyboardSize(float keyboardHeight){
	_program.use();
	glUniform1f(_program["keyboardHeight"], keyboardHeight);
	glUseProgram(0);
}

void Score::setMinMaxKeys(int minKey, int minKeyMajor, int notesCount){
	_program.use();
	glUniform1i(_program["minNoteMajor"], minKeyMajor);
	glUniform1f(_program["notesCount"], float(notesCount));
	glUseProgram(0);
}

void Score::setPlayDirection(bool reverse){
	_program.use();
	glUniform1i(_program["reverseMode"], reverse ? 1 : 0);
	glUseProgram(0);
}

void Score::setOrientation(bool horizontal){
	_program.use();
	glUniform1i(_program["horizontalMode"], horizontal ? 1 : 0);
	glUseProgram(0);
}

