#include <stdio.h>
#include <iostream>
#include <vector>

#include "ResourcesManager.h"
#include "ProgramUtilities.h"
// Resources headers.
#include "../resources/data.h"

 unsigned char* ResourcesManager::getDataForImage(const std::string & fileName, unsigned int & imwidth, unsigned int & imheight){
	
	if(imagesLibrary.count(fileName) > 0){
		
		auto size = imagesSize[fileName];
		imwidth = int(size[0]);
		imheight = int(size[1]);
		return imagesLibrary[fileName];
	}
	std::cerr << "[WARNING]: Unable to find ressource for image \"" << fileName << "\"." << std::endl;
	imwidth = 0;
	imheight = 0;
	return NULL;
}

std::string ResourcesManager::getStringForShader(const std::string & shaderName){
	
	if(shadersLibrary.count(shaderName) > 0){
		return shadersLibrary[shaderName];
	}
	
	std::cerr << "[WARNING]: Unable to find ressource for shader \"" << shaderName << "\"." << std::endl;
	return "";
}

void ResourcesManager::loadResources(){
	shadersLibrary.clear();
	imagesLibrary.clear();
	imagesSize.clear();
	
	shadersLibrary = shaders;
	
	imagesLibrary["font"] = font_image;
	imagesLibrary["flash"] = flash_image;
	imagesLibrary["particles"] = particles_image;
	imagesSize["font"] = glm::vec2(256, 128);
	imagesSize["flash"] = glm::vec2(256, 256);
	imagesSize["particles"] = glm::vec2(337, 24);
	
	{
		unsigned int imwidth;
		unsigned int imheight;
		auto fontTexture = ResourcesManager::getDataForImage("font", imwidth, imheight);
		textureLibrary["font"] = loadTexture(fontTexture, imwidth, imheight, 4, false);
	}
	
	{
		unsigned int imwidth;
		unsigned int imheight;
		auto flashTexture = ResourcesManager::getDataForImage("flash", imwidth, imheight);
		textureLibrary["flash"] = loadTexture(flashTexture, imwidth, imheight, 4, false);
	}
	
	{
		unsigned int imwidth1;
		unsigned int imheight1;
		auto particlesTexture = ResourcesManager::getDataForImage("particles", imwidth1, imheight1);
		textureLibrary["particles"] = loadTexture(particlesTexture, imwidth1, imheight1, 4, false);
	}
	
	{
		unsigned int imwidth1 = 4;
		unsigned int imheight1 = 4;
		unsigned char blankTexture[] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
		textureLibrary["blank"] = loadTexture(blankTexture, imwidth1, imheight1, 4, false);
	}
}

GLuint ResourcesManager::getTextureFor(const std::string & fileName){
	if(textureLibrary.count(fileName) > 0){
		return textureLibrary[fileName];
	}
	
	std::cerr << "[WARNING]: Unable to find texture for name \"" << fileName << "\"." << std::endl;
	return 0;
}

glm::vec2 ResourcesManager::getTextureSizeFor(const std::string & fileName){
	if(imagesSize.count(fileName) > 0){
		return imagesSize[fileName];
	}
	std::cerr << "[WARNING]: Unable to find texture size for name \"" << fileName << "\"." << std::endl;
	return glm::vec2(0.0f,0.0f);
}

std::map<std::string, std::string> ResourcesManager::shadersLibrary = {};

std::map<std::string,  unsigned char*> ResourcesManager::imagesLibrary = {};

std::map<std::string, glm::vec2> ResourcesManager::imagesSize = {};

std::map<std::string, GLuint> ResourcesManager::textureLibrary = {};
