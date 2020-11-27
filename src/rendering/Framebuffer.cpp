#include <stdio.h>
#include <iostream>

#include "../helpers/ProgramUtilities.h"
#include "Framebuffer.h"


Framebuffer::Framebuffer(int width, int height, GLuint format, GLuint type, GLuint filtering, GLuint wrapping) : _width(width),  _height(height){

	// Create a framebuffer.
	glGenFramebuffers(1, &_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	
	// Create the texture to store the result.
	glGenTextures(1, &_idColor);
	glBindTexture(GL_TEXTURE_2D, _idColor);
	glTexImage2D(GL_TEXTURE_2D, 0, format, _width , _height, 0, format, type, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	if(wrapping == GL_CLAMP_TO_BORDER){
		// Setup the border value for the shadow map
		GLfloat border[] = { 1.0, 1.0, 1.0, 1.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	}
	
	// Link the texture to the first color attachment (ie output) of the framebuffer.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 ,GL_TEXTURE_2D, _idColor, 0);
	
	// Create the renderbuffer (depth buffer + color(s) buffer(s)).
	glGenRenderbuffers(1, &_idRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _idRenderbuffer);
	// Setup the depth buffer storage.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, _width, _height);
	// Link the renderbuffer to the framebuffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _idRenderbuffer);
	
	//Register which color attachments to draw to.
	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

Framebuffer::~Framebuffer(){ clean(); }

void Framebuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void Framebuffer::bind(GLenum mode){
	glBindFramebuffer(mode, _id);
}

void Framebuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(int width, int height){
	if(width != _width || height != _height){
		_width = width;
		_height = height;
		// Resize the renderbuffer.
		glBindRenderbuffer(GL_RENDERBUFFER, _idRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// Resize the texture.
		glBindTexture(GL_TEXTURE_2D, _idColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	// Clear everything for safety;
	bind();
	glClear(GL_COLOR_BUFFER_BIT);
	unbind();
}

void Framebuffer::resize(glm::vec2 size){
	resize(int(size[0]),int(size[1]));
}

void Framebuffer::clean(){
	glDeleteRenderbuffers(1, &_idRenderbuffer);
	glDeleteTextures(1, &_idColor);
	glDeleteFramebuffers(1, &_id);
}

