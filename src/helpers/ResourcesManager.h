#ifndef ResourcesManager_h
#define ResourcesManager_h

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>


class ResourcesManager {

public:
	
	static std::vector<unsigned char> getDataForImage(const std::string & fileName, unsigned int & imwidth, unsigned int & imheight);
	
	static std::string getStringForShader(const std::string & shaderName);
	
	static void loadResources();
	
private:
	static std::map<std::string, std::string> shadersLibrary;
	
	static std::map<std::string, std::vector<unsigned char>> imagesLibrary;
	
	static std::map<std::string, glm::vec2> imagesSize;
};

#endif
