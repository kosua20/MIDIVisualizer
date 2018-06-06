#ifndef Background_h
#define Background_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>

#include "ScreenQuad.h"


class Background : public ScreenQuad {

public:

	/// Init function with measure time.
	Background(double secondsPerMeasure, const float scale);
	
	void setScale(const float scale);
	
	void setDisplay(const bool digits, const bool horiz, const bool vert);
};

#endif
