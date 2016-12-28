#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
#include "Keyboard.h"

enum class MouseMode {
	Start, Move, End
};


class Camera {
	
public:
	
	Camera();

	~Camera();
	
	/// Reset the position of the camera.
	void reset();

	/// Update the view matrix.
	void update(float elapsedTime);

	/// Register a key press/release.
	void key(int key, bool flag);
	
	/// Register a mouse move or click.
	void mouse(MouseMode mode, float x, float y);
	
	/// Update the screen size and projection matrix.
	void screen(int width, int height);
	
	/// The view matrix.
	glm::mat4 _view;
	
	/// The projection matrix.
	glm::mat4 _projection;

	// Screen size
	glm::vec2 _screenSize;
	// Size use for render targets.
	glm::vec2 _renderSize;
	
private:
	
	/// Vectors defining the view frame.
	glm::vec3 _eye;
	glm::vec3 _center;
	glm::vec3 _up;
	glm::vec3 _right;
	
	/// Input methods
	Keyboard _keyboard;
	
	

};

#endif
