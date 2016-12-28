#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "Renderer.h"


Renderer::Renderer(){ }

Renderer::~Renderer(){  }

void Renderer::init(int width, int height, const std::string& midiFilePath, const glm::vec3& baseColor, const float scale){
	
	ResourcesManager::loadResources();
	
	// GL options
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Player.
	_timer = 0.0;
	_shouldPlay = false;
	
	// Setup projection matrix.
	_camera.screen(width, height);
	
	// Setup framebuffers.
	_particlesFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(_camera._screenSize[0],_camera._screenSize[1], GL_RGBA,GL_UNSIGNED_BYTE,GL_LINEAR,GL_CLAMP_TO_EDGE));
	_blurFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(_camera._screenSize[0],_camera._screenSize[1], GL_RGBA,GL_UNSIGNED_BYTE,GL_LINEAR,GL_CLAMP_TO_EDGE));

	// Init objects.
	_scene.init(midiFilePath, baseColor, scale);
	_blurringScreen.init(_particlesFramebuffer->textureId(), ResourcesManager::getStringForShader("particlesblur_frag"));
	_blurryScreen.init(_blurFramebuffer->textureId(), ResourcesManager::getStringForShader("screenquad_frag"));
	_background.init(_scene.midiFile().tracks[0].secondsPerMeasure, scale);
	
	// Check setup errors.
	checkGLError();
}


void Renderer::draw(){
	
	// Compute the time elapsed since last frame, or keep the same value if playback is disabled.
	_timer = _shouldPlay ? glfwGetTime() : _timer;
	
	// Update active notes listing (for particles).
	_scene.updatesActiveNotes(_timer);
	
	// Set viewport.
	glViewport(0,0,_camera._screenSize[0],_camera._screenSize[1]);
	
	// Bind particles buffer.
	_particlesFramebuffer->bind();
	// Draw blurred particles from previous frames.
	_blurryScreen.draw(_timer, 1.0f/ _camera._screenSize);
	// Draw the new particles.
	_scene.drawParticles(_timer, 1.0f/ _camera._screenSize, true);
	_particlesFramebuffer->unbind();
	
	// Bind blur framebuffer.
	_blurFramebuffer->bind();
	// Perform box blur on result from particles pass.
	_blurringScreen.draw(_timer, 1.0f/ _camera._screenSize);
	_blurFramebuffer->unbind();
	
	// Final pass (directly on screen).
	
	glClear(GL_COLOR_BUFFER_BIT);
	// Draw the blurred particles.
	_blurryScreen.draw(_timer, 1.0f/ _camera._screenSize);
	// Draw the particles.
	_scene.drawParticles(_timer, 1.0f/ _camera._screenSize, false);
	// Draw the keys, grid, and measure numbers.
	_background.draw(_timer, 1.0f/ _camera._screenSize);
	// Draw the notes.
	_scene.draw(_timer, 1.0f/ _camera._screenSize);
	// Draw the flashes.
	_scene.drawFlashes(_timer, 1.0f/ _camera._screenSize);
	
}


void Renderer::clean(){
	
	// Clean objects.
	_scene.clean();
	_background.clean();
	_blurringScreen.clean();
	_blurryScreen.clean();
	_particlesFramebuffer->clean();
	_blurFramebuffer->clean();
	
}


void Renderer::resize(int width, int height){
	
	//Update the size of the viewport.
	glViewport(0, 0, width, height);
	// Update the projection matrix.
	_camera.screen(width, height);
	// Resize the framebuffers.
	_particlesFramebuffer->resize(_camera._screenSize);
	_blurFramebuffer->resize(_camera._screenSize);
	
}

void Renderer::keyPressed(int key, int action){
	
	if(action == GLFW_PRESS){
		if(key == GLFW_KEY_P){
			_shouldPlay = !_shouldPlay;
			glfwSetTime(_timer);
		} else if (key == GLFW_KEY_R){
			_timer = 0;
			glfwSetTime(_timer);
		}
	}
	
}


