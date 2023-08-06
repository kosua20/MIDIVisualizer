
#include "State.h"
#include "scene/MIDIScene.h"
#include "../helpers/ResourcesManager.h"
#include "../helpers/System.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::unordered_map<std::string, State::OptionInfos> State::_sharedInfos;

// Quality names.
const std::unordered_map<std::string, Quality::Level> Quality::names = {
	{ "LOW_RES", Quality::LOW_RES },
	{ "LOW", Quality::LOW },
	{ "MEDIUM", Quality::MEDIUM },
	{ "HIGH", Quality::HIGH },
	{ "HIGH_RES", Quality::HIGH_RES }
};

const std::unordered_map<Quality::Level, Quality> Quality::availables = {
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

State::OptionInfos::OptionInfos(){
	description = "";
	type = Type::OTHER;
	range = {0.0f, 0.0f};
	values = "";
}

State::OptionInfos::OptionInfos(const std::string & adesc, State::OptionInfos::Type atype, const std::array<float, 2> & arange){
	description = adesc;
	type = atype;
	range = arange;
	// Generate value description based on type and range.
	switch(type){
		case Type::BOOLEAN:
			values = "1 or 0 to enable/disable";
			break;
		case Type::INTEGER:
			values = "integer";
			if(range[0] != range[1]){
				values.append(" in [" + std::to_string(int(range[0])) + "," + std::to_string(int(range[1])) + "]");
			}
			break;
		case Type::FLOAT:
			values = "number";
			if(range[0] != range[1]){
				values.append(" in [" + std::to_string(range[0]) + "," + std::to_string(range[1]) + "]");
			}
			break;
		case Type::COLOR:
			values = "R G B in [0.0, 1.0]";
			break;
		case Type::KEY:
			values = "key index in [0,127]: 0 is C-1, 1 is C-1#, etc.";
			break;
		case Type::PATH:
			values = "absolute path to file on disk";
			break;
		default:
			break;
	}
}

void State::defineOptions(){
	// Integers.
	_sharedInfos["particles-count"] = {"Particles count", OptionInfos::Type::INTEGER, {1.0f, 512.0f}};

	// Booleans.
	_sharedInfos["show-particles"] = {"Should particles be shown", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-flashes"] = {"Should flashes be shown", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-blur"] = {"Should the blur be visible", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-blur-notes"] = {"Should the notes be part of the blur", OptionInfos::Type::BOOLEAN};
	_sharedInfos["lock-colors"] = {"Should the keys and all effects use the same color", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-horiz-lines"] = {"Should horizontal score lines be showed", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-vert-lines"] = {"Should vertical score lines be shown", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-numbers"] = {"Should measure numbers be shown", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-keyboard"] = {"Should the keyboard be shown", OptionInfos::Type::BOOLEAN};
	_sharedInfos["bg-img-behind-keyboard"] = {"Should the background image extend behind the keyboard", OptionInfos::Type::BOOLEAN};
	_sharedInfos["keyboard-highlight"] = {"Should the pressed keys be highlighted", OptionInfos::Type::BOOLEAN};
	_sharedInfos["keyboard-custom-colors"] = {"Override notes color for pressed keys", OptionInfos::Type::BOOLEAN};
	_sharedInfos["keyboard-minor-edges"] = {"Show edges around minor keys", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-score"] = {"Should the score (lines, numbers) be shown", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-bg-img"] = {"Use a background texture", OptionInfos::Type::BOOLEAN};
	_sharedInfos["show-notes"] = {"Should the notes be shown", OptionInfos::Type::BOOLEAN};
	_sharedInfos["notes-major-img-scroll"] = {"Should the major notes texture scroll with the notes", OptionInfos::Type::BOOLEAN};
	_sharedInfos["notes-minor-img-scroll"] = {"Should the minor notes texture scroll with the notes", OptionInfos::Type::BOOLEAN};

	// Floats.
	_sharedInfos["time-scale"] = {"Vertical display scale", OptionInfos::Type::FLOAT};
	_sharedInfos["minor-size"] = {"Relative size of the minor keys and notes", OptionInfos::Type::FLOAT, {0.1f, 1.0f}};
	_sharedInfos["particles-size"] = {"Size of the particles", OptionInfos::Type::FLOAT, {1.0f, 10.0f}};
	_sharedInfos["particles-speed"] = {"Particles speed", OptionInfos::Type::FLOAT};
	_sharedInfos["particles-expansion"] = {"Particles expansion factor", OptionInfos::Type::FLOAT};
	_sharedInfos["blur-attenuation"] = {"Blur attenuation speed", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["flashes-size"] = {"Flash effect size", OptionInfos::Type::FLOAT, {0.1f, 3.0f}};
	_sharedInfos["preroll"] = {"Preroll time in seconds before starting to play", OptionInfos::Type::FLOAT};
	_sharedInfos["scroll-speed"] = {"Playback speed", OptionInfos::Type::FLOAT};
	_sharedInfos["bg-img-opacity"] = {"Background opacity", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["fadeout-notes"] = {"Notes fade-out at the edge of the screen", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["notes-edge-width"] = {"Control the width of the edge around notes", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["notes-edge-intensity"] = {"Control the intensity of the edge around notes", OptionInfos::Type::FLOAT, {0.0f, 100.0f}};

	_sharedInfos["notes-corner-radius"]  = {"Rounding radius of the notes corners", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["notes-major-img-scale"]    = {"Scale of the texture applied to major notes", OptionInfos::Type::FLOAT};
	_sharedInfos["notes-major-img-intensity"] = {"Intensity of the texture applied to major notes", OptionInfos::Type::FLOAT, {0.0, 1.0f}};
	_sharedInfos["notes-minor-img-scale"]    = {"Scale of the texture applied to minor notes", OptionInfos::Type::FLOAT};
	_sharedInfos["notes-minor-img-intensity"] = {"Intensity of the texture applied to minor notes", OptionInfos::Type::FLOAT, {0.0, 1.0f}};



	// Colors.
	_sharedInfos["color-major"] = {"Major notes color", OptionInfos::Type::COLOR};
	_sharedInfos["color-bg"] = {"Background color", OptionInfos::Type::COLOR};
	_sharedInfos["color-particles"] = {"Particles color", OptionInfos::Type::COLOR};
	_sharedInfos["color-keyboard-major"] = {"Custom color for pressed major keys", OptionInfos::Type::COLOR};
	_sharedInfos["color-keyboard-minor"] = {"Custom color for pressed minor keys", OptionInfos::Type::COLOR};
	_sharedInfos["color-lines"] = {"Score lines color", OptionInfos::Type::COLOR};
	_sharedInfos["color-numbers"] = {"Score measure numbers color", OptionInfos::Type::COLOR};
	_sharedInfos["color-keyboard"] = {"Keyboard black keys color", OptionInfos::Type::COLOR};
	_sharedInfos["color-minor"] = {"Minor notes color", OptionInfos::Type::COLOR};
	_sharedInfos["color-flashes"] = {"Flash effect color", OptionInfos::Type::COLOR};

	// Others.
	_sharedInfos["quality"] = {"Rendering quality", OptionInfos::Type::OTHER};
	_sharedInfos["quality"].values = "values: LOW_RES, LOW, MEDIUM, HIGH, HIGH_RES";
	_sharedInfos["layers"] = {"Active layers indices, from background to foreground", OptionInfos::Type::OTHER};
	_sharedInfos["layers"].values = "values: bg-color: 0, bg-texture: 1, blur: 2, score: 3, keyboard: 4, particles: 5, notes: 6, flashes: 7, pedal: 8, wave: 9";
	_sharedInfos["sets-separator-control-points"] = {"Sets of control points for dynamic set asignment", OptionInfos::Type::OTHER};
	_sharedInfos["sets-separator-control-points"].values = "values: space-separated triplets time,key,set";

	// Sets
	for(size_t cid = 1; cid < SETS_COUNT; ++cid){
		const std::string num = std::to_string(cid);
		_sharedInfos["color-major-" + num] = {"Major notes color for set " + num, OptionInfos::Type::COLOR};
		_sharedInfos["color-particles-" + num] = {"Particles color for set " + num, OptionInfos::Type::COLOR};
		_sharedInfos["color-minor-" + num] = {"Minor notes color for set " + num, OptionInfos::Type::COLOR};
		_sharedInfos["color-flashes-" + num] = {"Flash effect color for set " + num, OptionInfos::Type::COLOR};

		_sharedInfos["color-major-" + num].category = OptionInfos::Category::SETS;
		_sharedInfos["color-particles-" + num].category = OptionInfos::Category::SETS;
		_sharedInfos["color-minor-" + num].category = OptionInfos::Category::SETS;
		_sharedInfos["color-flashes-" + num].category = OptionInfos::Category::SETS;

	}
	// Override set 0 parameters to stay consistent.
	_sharedInfos["color-major"].category = OptionInfos::Category::SETS;
	_sharedInfos["color-particles"].category = OptionInfos::Category::SETS;
	_sharedInfos["color-minor"].category = OptionInfos::Category::SETS;
	_sharedInfos["color-flashes"].category = OptionInfos::Category::SETS;
	
	_sharedInfos["colors-per-set"] = {"Should each notes set use its own key/effects colors", OptionInfos::Type::BOOLEAN};
	_sharedInfos["colors-per-set"].category = OptionInfos::Category::SETS;

	_sharedInfos["sets-mode"] = {"How should notes be grouped into sets", OptionInfos::Type::OTHER};
	_sharedInfos["sets-mode"].values = "per-channel: 0, per-track: 1, split based on a key separator: 2, per-key: 3, split based on list: 4, chromatic: 5";
	_sharedInfos["sets-mode"].category = OptionInfos::Category::SETS;

	_sharedInfos["sets-separator-key"] = {"If notes are grouped in two sets, defines the key where the split should happen", OptionInfos::Type::KEY, {0.0f, 127.0f}};
	_sharedInfos["sets-separator-key"].category = OptionInfos::Category::SETS;

	_sharedInfos["keyboard-size"] = {"Vertical size of the keyboard", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["keyboard-minor-height"] = {"Vertical fraction of the keyboard taken by minor keys", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};

	_sharedInfos["min-key"] = {"Lowest key to display", OptionInfos::Type::KEY, {0.0f, 127.0f}};
	_sharedInfos["max-key"] = {"Highest key to display", OptionInfos::Type::KEY, {0.0f, 127.0f}};

	_sharedInfos["show-pedal"] = {"Display the pedals indicator", OptionInfos::Type::BOOLEAN};
	_sharedInfos["pedal-size"] = {"Pedal indicator size", OptionInfos::Type::FLOAT, {0.05f, 0.5f}};
	_sharedInfos["pedal-opacity"] = {"Pedal indicator opacity when not pressed", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["color-pedal"] = {"Pedal color when pressed", OptionInfos::Type::COLOR};
	_sharedInfos["pedal-merge"] = {"Display only one pedal", OptionInfos::Type::BOOLEAN};
	_sharedInfos["pedal-location"] = {"Pedal location on screen", OptionInfos::Type::OTHER, {0.0f, 3.0f}};
	_sharedInfos["pedal-location"].values = "top-left: 0, bottom-left: 1, top-right: 2, bottom-right: 3";

	_sharedInfos["show-wave"] = {"Display the wave effect along the top of the keyboard", OptionInfos::Type::BOOLEAN};
	_sharedInfos["wave-size"] = {"Wave effect size", OptionInfos::Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos["wave-opacity"] = {"Wave effect opacity", OptionInfos::Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["wave-amplitude"] = {"Wave effect amplitude", OptionInfos::Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos["wave-frequency"] = {"Wave effect frequency", OptionInfos::Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos["color-wave"] = {"Wave effect color", OptionInfos::Type::COLOR};

	_sharedInfos["smooth"] = {"Apply anti-aliasing to smooth all lines", OptionInfos::Type::BOOLEAN};
	_sharedInfos["scroll-reverse"] = {"Notes scroll from bottom to top instead of the opposite", OptionInfos::Type::BOOLEAN};
	_sharedInfos["scroll-horizontal"] = {"Notes scroll from right to left when enabled", OptionInfos::Type::BOOLEAN};

	// Paths.
	_sharedInfos["bg-img-path"] = {"Path to an image on disk to use as background", OptionInfos::Type::PATH};
	_sharedInfos["particles-paths"] = {"Set of paths (separated by spaces) to black and white images on disk to use as particles", OptionInfos::Type::PATH};
	_sharedInfos["notes-major-img-path"] = {"Path to an image on disk to use as texture for the major notes", OptionInfos::Type::PATH};
	_sharedInfos["notes-minor-img-path"] = {"Path to an image on disk to use as texture for the minor notes", OptionInfos::Type::PATH};
	
}

size_t State::helpText(std::string & configOpts, std::string & setsOpts){
	if(_sharedInfos.empty()){
		defineOptions();
	}
	size_t maxLength = 0;
	for(const auto & param : _sharedInfos){
		maxLength = (std::max)(maxLength, param.first.size());
	}
	size_t alignSize = maxLength + 4;

	std::vector<std::string> msgDefault;
	std::vector<std::string> msgSets;

	for(const auto & param : _sharedInfos){
		const std::string padString(alignSize - param.first.size(), ' ');
		const std::string line = "--" + param.first + padString + param.second.description + " (" + param.second.values + ")\n";
		if(param.second.category == OptionInfos::Category::SETS){
			msgSets.push_back(line);
		} else {
			msgDefault.push_back(line);
		}
	}
	std::sort(msgDefault.begin(), msgDefault.end());
	std::sort(msgSets.begin(), msgSets.end());

	std::stringstream msgDefaultStr;
	std::stringstream msgSetsStr;

	for(const auto& str : msgDefault){
		msgDefaultStr << str;
	}
	for(const auto& str : msgSets){
		msgSetsStr << str;
	}
	configOpts = msgDefaultStr.str();
	setsOpts = msgSetsStr.str();
	return alignSize;
}

State::State(){
	reset();
	updateOptions();
}

void State::updateOptions(){
	if(_sharedInfos.empty()){
		defineOptions();
	}

	_intInfos["particles-count"] = &particles.count;
	_boolInfos["show-particles"] = &showParticles;
	_boolInfos["show-flashes"] = &showFlashes;
	_boolInfos["show-blur"] = &showBlur;
	_boolInfos["show-blur-notes"] = &showBlurNotes;
	_boolInfos["lock-colors"] = &lockParticleColor;
	_boolInfos["colors-per-set"] = &perSetColors;
	_boolInfos["show-horiz-lines"] = &background.hLines;
	_boolInfos["show-vert-lines"] = &background.vLines;
	_boolInfos["show-numbers"] = &background.digits;
	_boolInfos["show-keyboard"] = &showKeyboard;
	_boolInfos["bg-img-behind-keyboard"] = &background.imageBehindKeyboard;
	_boolInfos["keyboard-highlight"] = &keyboard.highlightKeys;
	_boolInfos["keyboard-custom-colors"] = &keyboard.customKeyColors;
	_boolInfos["keyboard-minor-edges"] = &keyboard.minorEdges;
	_boolInfos["show-score"] = &showScore;
	_boolInfos["show-bg-img"] = &background.image;
	_boolInfos["show-notes"] = &showNotes;
	_boolInfos["notes-major-img-scroll"] = &notes.majorTexScroll;
	_boolInfos["notes-minor-img-scroll"] = &notes.minorTexScroll;

	_floatInfos["time-scale"] = &scale;
	_floatInfos["minor-size"] = &background.minorsWidth;
	_floatInfos["particles-size"] = &particles.scale;
	_floatInfos["particles-speed"] = &particles.speed;
	_floatInfos["particles-expansion"] = &particles.expansion;
	_floatInfos["blur-attenuation"] = &attenuation;
	_floatInfos["flashes-size"] = &flashes.size;
	_floatInfos["preroll"] = &prerollTime;
	_floatInfos["scroll-speed"] = &scrollSpeed;
	_floatInfos["bg-img-opacity"] = &background.imageAlpha;
	_floatInfos["fadeout-notes"] = &notes.fadeOut;
	_floatInfos["notes-edge-width"] = &notes.edgeWidth;
	_floatInfos["notes-edge-intensity"] = &notes.edgeBrightness;
	_floatInfos["notes-corner-radius"] = &notes.cornerRadius;
	_floatInfos["notes-major-img-scale"] = &notes.majorTexScale;
	_floatInfos["notes-major-img-intensity"] = &notes.majorTexAlpha;
	_floatInfos["notes-minor-img-scale"] = &notes.minorTexScale;
	_floatInfos["notes-minor-img-intensity"] = &notes.minorTexAlpha;

	_vecInfos["color-bg"] = &background.color;
	_vecInfos["color-keyboard-major"] = &keyboard.majorColor[0];
	_vecInfos["color-keyboard-minor"] = &keyboard.minorColor[0];
	_vecInfos["color-lines"] = &background.linesColor;
	_vecInfos["color-numbers"] = &background.textColor;
	_vecInfos["color-keyboard"] = &background.keysColor;

	_vecInfos["color-major"] = &notes.majorColors[0];
	_vecInfos["color-particles"] = &particles.colors[0];
	_vecInfos["color-minor"] = &notes.minorColors[0];
	_vecInfos["color-flashes"] = &flashes.colors[0];

	for(size_t cid = 1; cid < notes.majorColors.size(); ++cid){
		const std::string num = std::to_string(cid);
		_vecInfos["color-major-" + num] = &notes.majorColors[cid];
		_vecInfos["color-particles-" + num] = &particles.colors[cid];
		_vecInfos["color-minor-" + num] = &notes.minorColors[cid];
		_vecInfos["color-flashes-" + num] = &flashes.colors[cid];
	}

	_intInfos["sets-mode"] = (int*)&setOptions.mode;
	_intInfos["sets-separator-key"] = &setOptions.key;
	_floatInfos["keyboard-size"] = &keyboard.size;
	_floatInfos["keyboard-minor-height"] = &keyboard.minorHeight;

	_intInfos["min-key"] = &minKey;
	_intInfos["max-key"] = &maxKey;

	_boolInfos["show-pedal"] = &showPedal;
	_floatInfos["pedal-size"] = &pedals.size;
	_floatInfos["pedal-opacity"] = &pedals.opacity;
	_vecInfos["color-pedal"] = &pedals.color;
	_boolInfos["pedal-merge"] = &pedals.merge;
	_intInfos["pedal-location"] = (int*)&pedals.location;

	_boolInfos["show-wave"] = &showWave;
	_floatInfos["wave-size"] = &waves.spread;
	_floatInfos["wave-opacity"] = &waves.opacity;
	_floatInfos["wave-amplitude"] =  &waves.amplitude;
	_floatInfos["wave-frequency"] =  &waves.frequency;
	_vecInfos["color-wave"] = &waves.color;

	_boolInfos["smooth"] = &applyAA;
	_boolInfos["scroll-reverse"] = &reverseScroll;
	_boolInfos["scroll-horizontal"] = &horizontalScroll;

	_pathInfos["bg-img-path"] = &background.imagePath;
	_pathInfos["particles-paths"] = &particles.imagePaths;

	_pathInfos["notes-major-path"] = &notes.majorImagePath;
	_pathInfos["notes-minor-path"] = &notes.minorImagePath;
}


void State::save(const std::string & path){
	std::string outputPath = path;
	if (path.size() > 4 && path.substr(path.size() - 4, 4) != ".ini") {
		outputPath += ".ini";
	}
	std::ofstream configFile = System::openOutputFile(outputPath);
	if(!configFile.is_open()){
		std::cerr << "[CONFIG]: Unable to save state to file at path " << outputPath << std::endl;
		return;
	}
	// Make sure the parameter pointers are up to date.
	updateOptions();

	configFile << "# MIDIVisualizer configuration file." << std::endl;
	configFile << "# Version (do not modify)" << std::endl;
	configFile << MIDIVIZ_VERSION_MAJOR << " " << MIDIVIZ_VERSION_MINOR << std::endl;

	// Write all options.
	for(const auto & param : _floatInfos){
		configFile << std::endl << "# " << _sharedInfos[param.first].description << " (";
		configFile << _sharedInfos[param.first].values << ")" << std::endl;
		configFile << param.first << ": " << *param.second << std::endl;
	}

	for(const auto & param : _intInfos){
		configFile << std::endl << "# " << _sharedInfos[param.first].description << " (";
		configFile << _sharedInfos[param.first].values << ")" << std::endl;
		configFile << param.first << ": " << *param.second << std::endl;
	}

	for(const auto & param : _boolInfos){
		configFile << std::endl << "# " << _sharedInfos[param.first].description << " (";
		configFile << _sharedInfos[param.first].values << ")" << std::endl;
		configFile << param.first << ": " << (*(param.second) ? 1 : 0) << std::endl;
	}

	for(const auto & param : _vecInfos){
		configFile << std::endl << "# " << _sharedInfos[param.first].description << " (";
		configFile << _sharedInfos[param.first].values << ")" << std::endl;
		const glm::vec3 & val = *param.second;
		configFile << param.first << ": " << val[0] << " " << val[1] << " " << val[2] << std::endl;
	}

	for(const auto & param : _pathInfos){
		// Skip empty path collection.
		if(param.second->empty()){
			continue;
		}

		configFile << std::endl << "# " << _sharedInfos[param.first].description << " (";
		configFile << _sharedInfos[param.first].values << ")" << std::endl;
		configFile << param.first << ":";
		for(const std::string& str : *(param.second)){
			configFile << " " << str;
		}
		configFile << std::endl;
	}

	configFile << std::endl << "# " << _sharedInfos["quality"].description << " (";
	configFile << _sharedInfos["quality"].values << ")" << std::endl;
	configFile << "quality: " << Quality::availables.at(quality).name << std::endl;

	configFile << std::endl << "# " << _sharedInfos["layers"].description << " (";
	configFile << _sharedInfos["layers"].values << ")" << std::endl;
	configFile << "layers: ";
	for (int i = 0; i < layersMap.size(); ++i) {
		configFile << layersMap[i] << (i != (layersMap.size() - 1) ? " " : "");
	}
	configFile << std::endl;

	configFile << std::endl << "# " << _sharedInfos["sets-separator-control-points"].description << " (";
	configFile << _sharedInfos["sets-separator-control-points"].values << ")" << std::endl;
	configFile << "sets-separator-control-points: " << setOptions.toKeysString(" ") << std::endl;

	configFile.close();

	_filePath = outputPath;
}

bool State::load(const std::string & path){
	std::ifstream configFileRaw = System::openInputFile(path);
	if(!configFileRaw.is_open()){
		std::cerr << "[CONFIG]: Unable to load state from file at path " << path << std::endl;
		return false;
	}

	// Now that we support comments we need to be able to skip them without large code modifications below.
	// Do a first parse of the file, filtering comments, and build a new stream from it.
	// Not the most efficient thing, but we are talking about a config file of < 100 lines.
	std::stringstream configFile;
	std::string line;
	while(std::getline(configFileRaw, line)){
		line = trim(line, "\r\t ");
		if(line.empty() || line[0] == '#'){
			continue;
		}
		configFile << line << std::endl;
	}
	configFileRaw.close();

	// Check the version number.
	int majVersion = 0; int minVersion = 0;
	configFile >> majVersion >> minVersion;
	
	if(majVersion > MIDIVIZ_VERSION_MAJOR || (majVersion == MIDIVIZ_VERSION_MAJOR && minVersion > MIDIVIZ_VERSION_MINOR)){
		std::cout << "[CONFIG]: The config is more recent, some settings might be ignored." << std::endl;
	}
	if(majVersion < MIDIVIZ_VERSION_MAJOR || (majVersion == MIDIVIZ_VERSION_MAJOR && minVersion < MIDIVIZ_VERSION_MINOR)){
		std::cout << "[CONFIG]: The config is older, some newer settings will be left as-is." << std::endl;
	}

	// Two options: if we are < 5.0, we use the old positional format.
	// Else we use the key-value format, similar to command line.
	if(majVersion < 5){
		load(configFile, majVersion, minVersion);
		// No set options.
	} else {
		// Build arguments list.
		const Arguments args = Configuration::parseArguments(configFile);
		load(args);
	}

	_filePath = path;
	return true;
}

void State::load(const Arguments & configArgs){
	// Make sure the parameter pointers are up to date.
	updateOptions();

	for(const auto & arg : configArgs){
		const auto & key = arg.first;
		if(arg.second.empty()){
			std::cerr << "[CONFIG]: Missing values for key " << key << "." << std::endl;
			continue;
		}

		if(key == "quality"){
			const std::string & qualityName = arg.second[0];
			if(Quality::names.count(qualityName) > 0){
				quality = Quality::names.at(qualityName);
			}
			continue;
		}

		if(key == "layers"){
			const size_t bound = (std::min)(layersMap.size(), arg.second.size());
			for(size_t id = 0; id < bound; ++id){
				layersMap[id] = Configuration::parseInt(arg.second[id]);
			}
			continue;
		}

		if(key == "sets-separator-control-points"){
			std::string str = arg.second[0];
			for(size_t tid = 1; tid < arg.second.size(); ++tid){
				str.append(" ");
				str.append(arg.second[tid]);
			}
			setOptions.fromKeysString(str);
			continue;
		}

		const auto itf = _floatInfos.find(key);
		if(itf != _floatInfos.end()){
			const auto & opt = itf->second;
			*opt = Configuration::parseFloat(arg.second[0]);
			const auto & range = _sharedInfos.at(key).range;
			if(range[0] != range[1]){
				*opt = glm::clamp(*opt, range[0], range[1]);
			}
			continue;
		}

		const auto iti = _intInfos.find(key);
		if(iti != _intInfos.end()){
			const auto & opt = iti->second;
			*opt = Configuration::parseInt(arg.second[0]);
			const auto & range = _sharedInfos.at(key).range;
			if(range[0] != range[1]){
				*opt = glm::clamp(*opt, int(range[0]), int(range[1]));
			}
			continue;
		}

		const auto itb = _boolInfos.find(key);
		if(itb != _boolInfos.end()){
			const auto & opt = itb->second;
			*opt = Configuration::parseBool(arg.second[0]);
			continue;
		}

		const auto itv = _vecInfos.find(key);
		if(itv != _vecInfos.end()){
			const auto & opt = itv->second;
			*opt = Configuration::parseVec3(arg.second);
			continue;
		}

		const auto its = _pathInfos.find(key);
		if(its != _pathInfos.end()){
			const auto & opt = its->second;
			*opt = arg.second;
			continue;
		}
	}

	// Ensure synchronization of the keyboard override colors.
	for(size_t cid = 1; cid < keyboard.majorColor.size(); ++cid){
		keyboard.majorColor[cid] = keyboard.majorColor[0];
		keyboard.minorColor[cid] = keyboard.minorColor[0];
	}

	// Don't erase the file path.
	
	// Rebuild internal data.
	setOptions.rebuild();
}

void State::synchronizeSets(){
	for(size_t cid = 1; cid < SETS_COUNT; ++cid){
		notes.majorColors[cid] = notes.majorColors[0];
		notes.minorColors[cid] = notes.minorColors[0];
		flashes.colors[cid] = flashes.colors[0];
		particles.colors[cid] = particles.colors[0];
		keyboard.majorColor[cid] = keyboard.majorColor[0];
		keyboard.minorColor[cid] = keyboard.minorColor[0];
	}
}

const std::string& State::filePath() const {
	return _filePath;
}

void State::reset(){

	for(size_t cid = 0; cid < SETS_COUNT; ++cid){
		notes.majorColors[cid] = 1.35f * glm::vec3(0.57f,0.19f,0.98f);
		notes.minorColors[cid] = 0.8f * notes.majorColors[0];
		flashes.colors[cid] = notes.majorColors[0];
		particles.colors[cid] = notes.majorColors[0];
		keyboard.majorColor[cid] = notes.majorColors[0];
		keyboard.minorColor[cid] = notes.minorColors[0];
	}

	background.color = glm::vec3(0.0f, 0.0f, 0.0f) ;
	background.linesColor = glm::vec3(1.0f, 1.0f, 1.0f);
	background.textColor = glm::vec3(1.0f, 1.0f, 1.0f);
	background.keysColor = glm::vec3(0.0f, 0.0f, 0.0f);

	scale = 0.5f ;
	attenuation = 0.99f;
	showParticles = true;
	showFlashes = true;
	showBlur = true;
	showBlurNotes = false;
	lockParticleColor = false; // Changed in 7.0
	perSetColors = false;
	showNotes = true;
	showScore = true;
	showKeyboard = true;
	flashes.size = 1.0f;
	
	background.minorsWidth = 0.8f;
	background.hLines = true;
	background.vLines = true ;
	background.digits = true ;
	background.image = false;
	background.imageAlpha = 1.0f;
	background.tex = 0;
	background.imageBehindKeyboard = false;
	background.imagePath.clear();

	particles.speed = 0.2f;
	particles.expansion = 1.0f;
	particles.scale = 1.0f;
	particles.count = 256;
	particles.imagePaths.clear();
	const GLuint blankID = ResourcesManager::getTextureFor("blankarray");
	particles.tex = blankID;
	particles.texCount = 1;
	
	quality = Quality::MEDIUM;
	prerollTime = 1.0f;
	scrollSpeed = 1.0f;
	keyboard.highlightKeys = true;
	keyboard.minorEdges = true;
	keyboard.customKeyColors = false;
	keyboard.size = 0.25f;
	keyboard.minorHeight = 0.6f;

	for (int i = 0; i < layersMap.size(); ++i) {
		layersMap[i] = i;
	}

	setOptions = SetOptions();

	notes.edgeBrightness = 1.05f;
	notes.edgeWidth = 0.1f;
	notes.fadeOut = 0.0f;
	notes.cornerRadius = 0.09f;
	notes.majorTexAlpha = 1.0;
	notes.majorTexScale = 5.0f;
	notes.minorTexAlpha = 1.0;
	notes.minorTexScale = 5.0f;
	notes.majorTexScroll = true;
	notes.minorTexScroll = true;
	notes.majorImagePath.clear();
	notes.minorImagePath.clear();
	notes.minorTex = 0;
	notes.majorTex = 0;
	
	minKey = 21;
	maxKey = 108;
	
	showPedal = true;
	pedals.color = notes.majorColors[0];
	pedals.size = 0.2f;
	pedals.opacity = 0.4f;
	pedals.merge = false;
	pedals.location = PedalsState::BOTTOMRIGHT;

	showWave = true;
	waves.color = notes.majorColors[0];
	waves.amplitude = 1.0f;
	waves.frequency = 1.0f;
	waves.opacity = 1.0f;
	waves.spread = 1.0f;

	applyAA = false;
	reverseScroll = false;
	horizontalScroll = false;

	_filePath = "";
}

void State::load(std::istream & configFile, int majVersion, int minVersion){

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 0
	// This part is always valid, as it was present when the saving system was introduced.
	// Note: we don't restore the texture IDs and scale.
	{
		configFile >> notes.majorColors[0][0] >> notes.majorColors[0][1] >> notes.majorColors[0][2] ;
		configFile >> background.color[0] >> background.color[1] >> background.color[2] ;
		configFile >> particles.colors[0][0] >> particles.colors[0][1] >> particles.colors[0][2] ;

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
		configFile >> showKeyboard ;

		configFile >> particles.speed ;
		configFile >> particles.expansion ;
		configFile >> particles.count ;
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 1
	// Added quality level.
	if(majVersion > 3 || (majVersion == 3 && minVersion >= 1)){
		std::string qualityName;;
		configFile >> qualityName;
		if(Quality::names.count(qualityName) > 0){
			quality = Quality::names.at(qualityName);
		}
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 2
	// Added attenuation factor.
	if(majVersion > 3 || (majVersion == 3 && minVersion >= 2)){
		configFile >> attenuation ;
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 3
	/* minorColor = 0.8f*baseColor;
	flashColor = baseColor; */
	if(majVersion > 3 || (majVersion == 3 && minVersion >= 3)){
		configFile >> showNotes;
		configFile >> background.linesColor[0] >> background.linesColor[1] >> background.linesColor[2] ;
		configFile >> background.textColor[0] >> background.textColor[1] >> background.textColor[2] ;
		configFile >> background.keysColor[0] >> background.keysColor[1] >> background.keysColor[2] ;
		configFile >> notes.minorColors[0][0] >> notes.minorColors[0][1] >> notes.minorColors[0][2] ;
		configFile >> flashes.colors[0][0] >> flashes.colors[0][1] >> flashes.colors[0][2] ;
		configFile >> flashes.size;
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 5
	if (majVersion > 3 || (majVersion == 3 && minVersion >= 5)) {
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
	if (majVersion > 4 || (majVersion == 4 && minVersion >= 0)) {
		configFile >> background.image;
		configFile >> background.imageAlpha;
		configFile >> background.imageBehindKeyboard;
		configFile >> keyboard.highlightKeys;
	}

	// MIDIVIZ_VERSION_MAJOR == 4, MIDIVIZ_VERSION_MINOR == 1
	if (majVersion > 4 || (majVersion == 4 && minVersion >= 1)) {
		configFile >> keyboard.customKeyColors;
		configFile >> keyboard.majorColor[0][0] >> keyboard.majorColor[0][1] >> keyboard.majorColor[0][2];
		configFile >> keyboard.minorColor[0][0] >> keyboard.minorColor[0][1] >> keyboard.minorColor[0][2];
	}

	// Ensure synchronization of all channels colors.
	synchronizeSets();

	pedals.color = notes.majorColors[0];
	waves.color = notes.majorColors[0];
}
