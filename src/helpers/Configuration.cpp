#include "Configuration.h"

#include <iostream>
#include <stdio.h>
#include <algorithm>

std::string trim(const std::string & str, const std::string & del) {
	const size_t firstNotDel = str.find_first_not_of(del);
	if(firstNotDel == std::string::npos) {
		return "";
	}
	const size_t lastNotDel = str.find_last_not_of(del);
	return str.substr(firstNotDel, lastNotDel - firstNotDel + 1);
}

Arguments Configuration::parseArguments(std::istream & configFile){
	// Build arguments list.
	std::map<std::string, std::vector<std::string>> args;
	std::string line;
	// Again, but this time splitting keys from arguments.
	while(std::getline(configFile, line)){
		// Remove extra spaces and dashes.
		const std::string lineTrim = trim(line, " -\t\r");
		if(lineTrim.empty()) {
			continue;
		}
		// Split at first space.
		const std::string::size_type keySep = lineTrim.find_first_of(" \t");
		if(keySep == std::string::npos){
			std::cerr << "[CONFIG]: Ignoring key " << lineTrim << " without arguments." << std::endl;
			continue;
		}
		std::vector<std::string> values;
		const std::string key = trim(lineTrim.substr(0, keySep), "-: \t");
		// Split the rest of the line based on spaces only.
		std::string::size_type beginPos = keySep + 1;
		std::string::size_type afterEndPos = lineTrim.find_first_of(" \t", beginPos);
		while(afterEndPos != std::string::npos) {
			const std::string value = lineTrim.substr(beginPos, afterEndPos - beginPos);
			values.push_back(value);
			beginPos	= afterEndPos + 1;
			afterEndPos = lineTrim.find_first_of(" \t", beginPos);
		}
		// There is one remaining value, the last one.
		const std::string value = lineTrim.substr(beginPos);
		values.push_back(value);

		if(!key.empty() && !values.empty()) {
			args[key] = values;
		}
	}
	return args;
}

Arguments Configuration::parseArguments(const std::vector<std::string> & argv, bool & showHelp, bool & showVersion){
	std::map<std::string, std::vector<std::string>> args;

	const size_t argc = argv.size();
	for(size_t aid = 1; aid < argc;) {
		// Clean the argument from any -
		const std::string arg = trim(argv[aid], "-:\t");
		if(arg.empty()) {
			continue;
		}
		++aid;
		
		// Note help and skip.
		if(arg == "help" || arg == "h"){
			showHelp = true;
			continue;
		}
		if(arg == "version" || arg == "v"){
			showVersion = true;
			continue;
		}
		std::vector<std::string> values;

		// While we do not encounter a dash, the values are associated to the current argument.
		while(aid < argc) {
			// If the argument begins with a double dash, it's the next argument.
			if(argv[aid].size() >= 2 && argv[aid].substr(0, 2) == "--") {
				break;
			}
			values.emplace_back(argv[aid]);
			++aid;
		}
		if(values.empty()) {
			std::cerr << "[CONFIG]: No values for key " << arg << std::endl;
			continue;
		}
		args[arg] = values;
	}
	return args;
}

bool Configuration::parseBool(const std::string & str){
	return str == "1" || str == "true" || str == "yes" || str == "True" || str == "Yes";
}

int Configuration::parseInt(const std::string & str){
	return std::stoi(str);
}

float Configuration::parseFloat(const std::string & str){
	return std::stof(str);
}

glm::vec3 Configuration::parseVec3(const std::vector<std::string> & strs){
	glm::vec3 vec(0.0f);
	for(size_t i = 0; i < (std::min)(size_t(3), strs.size()); ++i){
		vec[int(i)] = parseFloat(strs[i]);
	}
	return vec;
}
