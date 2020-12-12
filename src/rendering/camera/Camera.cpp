#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"


Camera::Camera() : _keyboard(_eye, _center, _up, _right) {
	reset();
}

Camera::~Camera(){}

void Camera::reset(){
	_eye = glm::vec3(0.0,0.0,1.0);
	_center = glm::vec3(0.0,0.0,0.0);
	_up = glm::vec3(0.0,1.0,0.0);
	_right = glm::vec3(1.0,0.0,0.0);
	_view = glm::lookAt(_eye, _center, _up);
	_keyboard.reset();
	_scale = 1.0f;
}

void Camera::update(float elapsedTime){
	
	_keyboard.update(elapsedTime);
	// Update the view matrix.
	_view = glm::lookAt(_eye, _center, _up);
}


void Camera::key(int key, bool flag){
	if (key == GLFW_KEY_W || key == GLFW_KEY_A
		|| key == GLFW_KEY_S || key == GLFW_KEY_D
		|| key == GLFW_KEY_Q || key == GLFW_KEY_E) {
		_keyboard.registerMove(key, flag);
	} else if(flag && key == GLFW_KEY_R) {
		reset();
	} else {
		std::cout << "[INPUT]: Key: " << key << " (" << char(key) << ")." << std::endl;
	}
}

void Camera::mouse(MouseMode mode, float x, float y){
	if (mode == MouseMode::End) {
		_keyboard.endLeftMouse();
	} else {
		// We normalize the x and y values to the [-1, 1] range.
		float xPosition =  fmax(fmin(1.0f,2.0f * x / float(_screenSize[0]) - 1.0f),-1.0f);
		float yPosition =  fmax(fmin(1.0f,2.0f * y / float(_screenSize[1]) - 1.0f),-1.0f);
		
		if(mode == MouseMode::Start) {
			_keyboard.startLeftMouse(xPosition,yPosition);
		} else {
			_keyboard.leftMouseTo(xPosition,yPosition);
		}
	}
}


void Camera::screen(int width, int height, float scaling){
	_screenSize[0] = (width > 0 ? width : 1);
	_screenSize[1] = (height > 0 ? height : 1);
	_scale = scaling;

	_renderSize = glm::ivec2(glm::round(glm::vec2(_screenSize) / _scale));
	// Perspective projection.
	_projection = glm::perspective(45.0f, float(_renderSize[0]) / float(_renderSize[1]), 0.1f, 100.f);
}




