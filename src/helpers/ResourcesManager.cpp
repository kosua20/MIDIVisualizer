#include <stdio.h>
#include <iostream>
#include <vector>

#include "ResourcesManager.h"

// Ressources headers.
#include "../ressources/shaders.h"
#include "../ressources/font_image.h"
#include "../ressources/flash_image.h"
#include "../ressources/particles_image.h"

std::vector<unsigned char> ResourcesManager::getDataForImage(const std::string & fileName, unsigned int & imwidth, unsigned int & imheight){
	
	if(imagesLibrary.count(fileName) > 0){
		
		auto size = imagesSize[fileName];
		imwidth = int(size[0]);
		imheight = int(size[1]);
		return imagesLibrary[fileName];
	}
	std::cerr << "[WARNING]: Unable to find ressource for image \"" << fileName << "\"." << std::endl;
	imwidth = 0;
	imheight = 0;
	return std::vector<unsigned char>();
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
	
}

std::map<std::string, std::string> ResourcesManager::shadersLibrary = {};

std::map<std::string, std::vector<unsigned char>> ResourcesManager::imagesLibrary = {};

std::map<std::string, glm::vec2> ResourcesManager::imagesSize = {};
