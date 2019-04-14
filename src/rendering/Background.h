#ifndef Background_h
#define Background_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>

#include "ScreenQuad.h"


class Background : public ScreenQuad {

public:

	/// Init function with measure time.
	Background(double secondsPerMeasure);
	
	void setScaleAndMinorWidth(const float scale, const float width);
	
	void setDisplay(const bool digits, const bool horiz, const bool vert, const bool keys);
	
	void setColors(const glm::vec3 & linesColor, const glm::vec3 & textColor, const glm::vec3 & keysColor);
};

#endif
