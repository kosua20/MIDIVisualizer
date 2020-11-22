#ifndef Score_h
#define Score_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>

#include "ScreenQuad.h"


class Score : public ScreenQuad {

public:

	/// Init function with measure time.
	Score(double secondsPerMeasure);
	
	void setScaleAndMinorWidth(const float scale, const float width);
	
	void setDisplay(const bool digits, const bool horiz, const bool vert);
	
	void setColors(const glm::vec3 & linesColor, const glm::vec3 & textColor, const glm::vec3 & keysColor);

	void setKeyboardSize(float keyboardHeight);

	void setMinMaxKeys(int minKey, int minKeyMajor, int notesCount);

	void setPlayDirection(bool reverse);

};

#endif
