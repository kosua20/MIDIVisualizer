#include "Configuration.h"
#include "../rendering/State.h"
#include "../helpers/Recorder.h"
#include "../helpers/System.h"

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

std::string join(const std::vector<std::string>& strs, const std::string& delim){
	std::string res;
	const size_t strCount = strs.size();
	for(size_t i = 0; i < strCount; ++i){
		res += strs[i];
		if(i != strCount - 1){
			res += " ";
		}
	}
	return res;
}

Configuration::Configuration(const std::string& path, const std::vector<std::string>& argv){

	// Attempt to load arguments from file.
	Arguments argsFromFile;
	std::ifstream configFile = System::openInputFile(path);
	if(!configFile.is_open()){
		std::cerr << "[CONFIG]: Could not load internal configuration from " << path << ". Attempting to load default configuration from working directory." << std::endl;
		configFile = System::openInputFile(defaultName());
		if(!configFile.is_open()){
			std::cerr << "[CONFIG]: No default file either, it's probably a first launch." << std::endl;
		}
	}
	if(configFile){
		argsFromFile = parseArguments(configFile);
		configFile.close();
	}

	// Also load arguments from command line.
	Arguments argsFromCommand = parseArguments(argv);

	// Merge both: the command line has priority over the file.
	_args = argsFromFile;
	for(const auto& arg : argsFromCommand){
		_args[arg.first] = arg.second;
	}

	// We can now parse the arguments.
	for(const auto& arg : _args){
		const std::string& name = arg.first;
		const std::vector<std::string>& vals = arg.second;

		// Info
		{
			if(name == "help" || name == "h"){
				showHelp = true;
				continue;
			}
			if(name == "version" || name == "v"){
				showVersion = true;
				continue;
			}
		}
		// Window options
		{
			if(name == "size" && vals.size() >= 2){
				windowSize[0] = Configuration::parseInt(vals[0]);
				windowSize[1] = Configuration::parseInt(vals[1]);
			}
			if(name == "position" && vals.size() >= 2){
				windowPos[0] = Configuration::parseInt(vals[0]);
				windowPos[1] = Configuration::parseInt(vals[1]);
			}
			if(name == "gui-size" && vals.size() >= 1){
				guiScale = Configuration::parseFloat(vals[0]);
			}

			if(name == "fullscreen"){
				fullscreen = vals.empty() || Configuration::parseBool(vals[0]);
			}
			if(name == "hide-window"){
				hideWindow = vals.empty() || Configuration::parseBool(vals[0]);
			}
			if(name == "forbid-transparency"){
				preventTransparency = vals.empty() || Configuration::parseBool(vals[0]);
			}
			if(name == "transparency"){
				useTransparency = vals.empty() || Configuration::parseBool(vals[0]);
			}
		}
		// File options
		{
			if(name == "midi" && vals.size() >= 1){
				lastMidiPath = join(vals, " ");
			}
			if(name == "config" && vals.size() >= 1){
			   lastConfigPath = join(vals, " ");
			}
			if(name == "device" && vals.size() >= 1){
				lastMidiDevice = join(vals, " ");
			}
		}
		// Export options
		{
			if(name == "export" && vals.size() >= 1){
				exporting.path = join(vals, " ");
			}
			if(name == "framerate" && vals.size() >= 1){
				exporting.framerate = Configuration::parseInt(vals[0]);
			}
			if(name == "bitrate" && vals.size() >= 1){
				exporting.bitrate = Configuration::parseInt(vals[0]);
			}
			if(name == "postroll" && vals.size() >= 1){
				exporting.postroll = Configuration::parseFloat(vals[0]);
			}
			if(name == "fix-premultiply"){
				exporting.fixPremultiply = vals.empty() || Configuration::parseBool(vals[0]);
			}
			if(name == "out-alpha" || name == "png-alpha"){
				exporting.alphaBackground = vals.empty() || Configuration::parseBool(vals[0]);
			}
			if(name == "format" && vals.size() >= 1){
				if(vals[0] == "MPEG2"){
					exporting.format = Export::Format::MPEG2;
				} else if(vals[0] == "MPEG4"){
					exporting.format = Export::Format::MPEG4;
				} else if(vals[0] == "PRORES"){
					exporting.format = Export::Format::PRORES;
				}
			}
		}
	}

}

Arguments Configuration::parseArguments(std::istream & configFile){
	// Build arguments list.
	std::unordered_map<std::string, std::vector<std::string>> args;
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

Arguments Configuration::parseArguments(const std::vector<std::string> & argv){
	std::unordered_map<std::string, std::vector<std::string>> args;

	const size_t argc = argv.size();
	for(size_t aid = 1; aid < argc;) {
		// Clean the argument from any -
		const std::string arg = trim(argv[aid], "-:\t");
		if(arg.empty()) {
			continue;
		}
		++aid;

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

void Configuration::save(const std::string& path){
	std::ofstream outFile = System::openOutputFile(path);
	if(!outFile.is_open()){
		std::cerr << "[CONFIG]: Could not save internal configuration to " << path << ", attempting to save in working dir." << std::endl;
		outFile = System::openOutputFile(defaultName());
		if(!outFile.is_open()){
			std::cerr << "[CONFIG]: Unable to save in working dir either, cancelling." << std::endl;
			return;
		}
	}

	// File options
	if(!lastMidiPath.empty()){
		outFile << "midi " << lastMidiPath << "\n";
	}
	if(!lastConfigPath.empty()){
		outFile << "config " << lastConfigPath << "\n";
	}
	if(!lastMidiDevice.empty()){
		outFile << "device " << lastMidiDevice << "\n";
	}

	// Window options
	outFile << "size " << windowSize[0] << " " << windowSize[1] << "\n";
	outFile << "position " << windowPos[0] << " " << windowPos[1] << "\n";
	outFile << "gui-size " << guiScale << "\n";
	outFile << "fullscreen " << fullscreen << "\n";
	outFile << "hide-window " << hideWindow << "\n";
	outFile << "forbid-transparency " << preventTransparency << "\n";
	outFile << "transparency " << useTransparency << "\n";

	outFile.close();
}

void Configuration::printVersion(){
	std::cout << "MIDIVisualizer v" << MIDIVIZ_VERSION_MAJOR << "." << MIDIVIZ_VERSION_MINOR << std::endl;
	std::cout << "* Built on " << __DATE__ << ", at " << __TIME__ << "." << std::endl;
	std::cout << "* Video export with ffmpeg is " << (Recorder::videoExportSupported() ? "enabled" : "disabled") << "." << std::endl;
}

void Configuration::printHelp(){
	std::string configOpts, setsOpts;
	const size_t alignSize = State::helpText(configOpts, setsOpts);

	const std::vector<std::pair<std::string, std::string>> genOpts = {
		{"midi", "path to a MIDI file to load"},
		{"device", "name of a MIDI device to start a live session to (or VIRTUAL to act as a virtual device)"},
		{"config", "path to a configuration INI file"},
		{"size", "dimensions of the window (--size W H)"},
		{"position", "position of the window (--position X Y)"},
		{"fullscreen", "start in fullscreen (1 or 0 to enable/disable)"},
		{"gui-size", "GUI text and button scaling (number, default 1.0)"},
		{"transparency", "enable transparent window background if supported (1 or 0 to enable/disable)"},
		{"forbid-transparency", "prevent transparent window background(1 or 0 to enable/disable)"},
		{"help", "display this help message"},
		{"version", "display the executable version and configuration"},
	};

	const std::vector<std::pair<std::string, std::string>> expOpts = {
		{"export", "path to the output video (or directory for PNG)"},
		{"format", "output format (values: PNG, MPEG2, MPEG4, PRORES)"},
		{"framerate", "number of frames per second to export (integer)"},
		{"bitrate", "target video bitrate in Mb (integer)"},
		{"postroll", "Postroll time after the track, in seconds (number, default 10.0)"},
		{"out-alpha", "use transparent output background, only for PNG and PRORES (1 or 0 to enable/disable)"},
		{"fix-premultiply", "cancel alpha premultiplication, only when out-alpha is enabled (1 or 0 to enable/disable)"},
		{"hide-window", "do not display the window (1 or 0 to enable/disable)"},
	};

	std::cout << "---- Infos ---- MIDIVisualizer v" << MIDIVIZ_VERSION_MAJOR << "." << MIDIVIZ_VERSION_MINOR << " --------" << std::endl
	<< "Visually display a midi file in real time." << std::endl
	<< "Created by Simon Rodriguez (https://github.com/kosua20/MIDIVisualizer)" << std::endl;

	std::cout << std::endl << "* General options: " << std::endl;
	for(const auto & opt : genOpts){
		const std::string pad((std::max)(int(alignSize) - int(opt.first.size()), 0), ' ');
		std::cout << "--" << opt.first << pad << opt.second << std::endl;
	}

	std::cout << std::endl << "* Export options: (--export path is mandatory)" << std::endl;
	for(const auto & opt : expOpts){
		const std::string pad((std::max)(int(alignSize) - int(opt.first.size()), 0), ' ');
		std::cout << "--" << opt.first << pad << opt.second << std::endl;
	}

	std::cout << std::endl << "* Configuration options: (will override config file)" << std::endl
	<< configOpts;

	std::cout << std::endl << "* Note-sets options: (will override config file)" << std::endl
	<< setsOpts << std::endl;

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

std::string Configuration::defaultName(){
	return "midiviz_internal.settings";
}
