#include <stdio.h>
#include <iostream>
#include <vector>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "Background.h"

void Background::init(double secondsPerMeasure, const float scale){
	
	// Load font atlas.
	unsigned int imwidth;
	unsigned int imheight;
	auto fontTexture = ResourcesManager::getDataForImage("font", imwidth, imheight);
	GLuint textureId = loadTexture(fontTexture, imwidth, imheight, false);

	ScreenQuad::init(textureId, ResourcesManager::getStringForShader("background_frag"));
	
	// Load additional data.
	glUseProgram(_programId);
	GLuint sigID = glGetUniformLocation(_programId, "secondsPerMeasure");
	glUniform1f(sigID, secondsPerMeasure);
	GLuint speedID = glGetUniformLocation(_programId, "mainSpeed");
	glUniform1f(speedID, scale);
	glUseProgram(0);
	
}

