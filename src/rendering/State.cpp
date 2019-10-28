
#include "State.h"
#include "MIDIScene.h"
#include "../helpers/ResourcesManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Quality names.
const std::map<std::string, Quality::Level> Quality::names = {
	{ "LOW_RES", Quality::LOW_RES },
	{ "LOW", Quality::LOW },
	{ "MEDIUM", Quality::MEDIUM },
	{ "HIGH", Quality::HIGH },
	{ "HIGH_RES", Quality::HIGH_RES }
};

const std::map<Quality::Level, Quality> Quality::availables = {
	{ Quality::LOW_RES, { Quality::LOW_RES, 0.5f, 0.5f, 0.5f}},
	{ Quality::LOW, { Quality::LOW, 0.5f, 0.5f, 1.0f}},
	{ Quality::MEDIUM, { Quality::MEDIUM, 0.5f, 1.0f, 1.0f}},
	{ Quality::HIGH, { Quality::HIGH, 1.0f, 1.0f, 1.0f}},
	{ Quality::HIGH_RES, { Quality::HIGH_RES, 1.0f, 2.0f, 2.0f}}
};

Quality::Quality(const Quality::Level & alevel, const float partRes, const float blurRes, const float finRes){
	for(const auto & kv : names){
		if(kv.second == alevel){
			name = kv.first;
			break;
		}
	}
	particlesResolution = partRes; blurResolution = blurRes; finalResolution = finRes;
}


void State::save(const std::string & path){
	std::string outputPath = path;
	if (path.size() > 4 && path.substr(path.size() - 4, 4) != ".ini") {
		outputPath += ".ini";
	}
	std::ofstream configFile(outputPath);
	if(!configFile.is_open()){
		std::cerr << "Unable to save state to file at path " << outputPath << std::endl;
		return;
	}
	configFile << MIDIVIZ_VERSION_MAJOR << " " << MIDIVIZ_VERSION_MINOR << std::endl;
	
	configFile << baseColor[0] << " " << baseColor[1] << " " << baseColor[2] << std::endl;
	configFile << background.color[0] << " " << background.color[1] << " " << background.color[2] << std::endl;
	configFile << particles.color[0] << " " << particles.color[1] << " " << particles.color[2] << std::endl;
	
	configFile << scale << std::endl;
	configFile << showParticles << std::endl;
	configFile << showFlashes << std::endl;
	configFile << showBlur << std::endl;
	configFile << showBlurNotes << std::endl;
	configFile << lockParticleColor << std::endl;
	
	configFile << background.minorsWidth << std::endl;
	configFile << background.hLines << std::endl;
	configFile << background.vLines << std::endl;
	configFile << background.digits << std::endl;
	configFile << background.keys << std::endl;
	
	configFile << particles.speed << std::endl;
	configFile << particles.expansion << std::endl;
	configFile << particles.count << std::endl;
	
	configFile << Quality::availables.at(quality).name << std::endl;
	configFile << attenuation << std::endl;
	configFile << showNotes << std::endl;
	
	configFile << background.linesColor[0] << " " << background.linesColor[1] << " " << background.linesColor[2] << std::endl;
	configFile << background.textColor[0] << " " << background.textColor[1] << " " << background.textColor[2] << std::endl;
	configFile << background.keysColor[0] << " " << background.keysColor[1] << " " << background.keysColor[2] << std::endl;
	configFile << minorColor[0] << " " << minorColor[1] << " " << minorColor[2] << std::endl;
	configFile << flashColor[0] << " " << flashColor[1] << " " << flashColor[2] << std::endl;
	configFile << flashSize << std::endl;

	configFile << prerollTime << std::endl;
	configFile << showScore << std::endl;

	for (int i = 0; i < layersMap.size(); ++i) {
		configFile << layersMap[i] << (i != (layersMap.size() - 1) ? " " : "");
	}
	configFile << std::endl;

	configFile << background.image << std::endl;
	configFile << background.imageAlpha << std::endl;
	configFile << background.imageBehindKeyboard << std::endl;
	configFile << highlightKeys << std::endl;
	configFile.close();
}

void State::load(const std::string & path){
	std::ifstream configFile(path);
	if(!configFile.is_open()){
		std::cerr << "Unable to load state from file at path " << path << std::endl;
		return;
	}
	int majVersion = 0; int minVersion = 0;
	configFile >> majVersion >> minVersion;
	
	if(majVersion > MIDIVIZ_VERSION_MAJOR || (majVersion == MIDIVIZ_VERSION_MAJOR && minVersion > MIDIVIZ_VERSION_MINOR)){
		std::cout << "The config is more recent, some settings might be ignored." << std::endl;
	}
	if(majVersion < MIDIVIZ_VERSION_MAJOR || (majVersion == MIDIVIZ_VERSION_MAJOR && minVersion < MIDIVIZ_VERSION_MINOR)){
		std::cout << "The config is older, some newer settings will be left as-is." << std::endl;
	}
	
	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 0
	// This part is always valid, as it was present when the saving system was introduced.
	// Note: we don't restore the texture IDs and scale.
	{
		configFile >> baseColor[0] >> baseColor[1] >> baseColor[2] ;
		configFile >> background.color[0] >> background.color[1] >> background.color[2] ;
		configFile >> particles.color[0] >> particles.color[1] >> particles.color[2] ;
	
		configFile >> scale ;
		configFile >> showParticles ;
		configFile >> showFlashes ;
		configFile >> showBlur ;
		configFile >> showBlurNotes ;
		configFile >> lockParticleColor ;
	
		configFile >> background.minorsWidth ;
		configFile >> background.hLines ;
		configFile >> background.vLines ;
		configFile >> background.digits ;
		configFile >> background.keys ;
	
		configFile >> particles.speed ;
		configFile >> particles.expansion ;
		configFile >> particles.count ;
		
	}
	
	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 1
	// Added quality level.
	if(majVersion >= 3  && minVersion >= 1){
		std::string qualityName;;
		configFile >> qualityName;
		if(Quality::names.count(qualityName) > 0){
			quality = Quality::names.at(qualityName);
		}
	}
	
	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 2
	// Added attenuation factor.
	if(majVersion >= 3  && minVersion >= 2){
		configFile >> attenuation ;
	}
	
	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 3
	minorColor = 0.8f*baseColor;
	flashColor = baseColor;
	if(majVersion >= 3  && minVersion >= 3){
		configFile >> showNotes;
		configFile >> background.linesColor[0] >> background.linesColor[1] >> background.linesColor[2] ;
		configFile >> background.textColor[0] >> background.textColor[1] >> background.textColor[2] ;
		configFile >> background.keysColor[0] >> background.keysColor[1] >> background.keysColor[2] ;
		configFile >> minorColor[0] >> minorColor[1] >> minorColor[2] ;
		configFile >> flashColor[0] >> flashColor[1] >> flashColor[2] ;
		configFile >> flashSize;
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 5
	if (majVersion >= 3 && minVersion >= 5) {
		configFile >> prerollTime;
		configFile >> showScore;
		std::string layersList;
		// Get the next line.
		// Skip newline.
		std::getline(configFile, layersList);
		std::getline(configFile, layersList);
		std::stringstream sstr(layersList);
		int id = 0;
		while (!sstr.eof() && id < layersMap.size()) {
			sstr >> layersMap[id];
			++id;
		}
	}

	// MIDIVIZ_VERSION_MAJOR == 4, MIDIVIZ_VERSION_MINOR == 0
	if (majVersion >= 4 && minVersion >= 0) {
		configFile >> background.image;
		configFile >> background.imageAlpha;
		configFile >> background.imageBehindKeyboard;
		configFile >> highlightKeys;
	}
	
	configFile.close();
}

void State::reset(){
	baseColor = 1.35f*glm::vec3(0.57f,0.19f,0.98f);
	minorColor = 0.8f*baseColor;
	flashColor = baseColor;
	background.color = glm::vec3(0.0f, 0.0f, 0.0f) ;
	background.linesColor = glm::vec3(1.0f, 1.0f, 1.0f);
	background.textColor = glm::vec3(1.0f, 1.0f, 1.0f);
	background.keysColor = glm::vec3(0.0f, 0.0f, 0.0f);
	particles.color = baseColor;
	
	scale = 0.5f ;
	attenuation = 0.99f;
	showParticles = true ;
	showFlashes = true ;
	showBlur = true ;
	showBlurNotes = false ;
	lockParticleColor = true ;
	showNotes = true;
	showScore = true;
	flashSize = 1.0f;
	
	background.minorsWidth = 0.8f;
	background.hLines = true;
	background.vLines = true ;
	background.digits = true ;
	background.keys = true ;
	background.image = false;
	background.imageAlpha = 1.0f;
	background.tex = 0;
	background.imageBehindKeyboard = false;

	particles.speed = 0.2f;
	particles.expansion = 1.0f;
	particles.scale = 1.0f;
	particles.count = 256;
	const GLuint blankID = ResourcesManager::getTextureFor("blankarray");
	particles.tex = blankID;
	particles.texCount = 1;
	
	quality = Quality::MEDIUM;
	prerollTime = 1.0f;
	highlightKeys = true;

	for (int i = 0; i < layersMap.size(); ++i) {
		layersMap[i] = i;
	}
}
