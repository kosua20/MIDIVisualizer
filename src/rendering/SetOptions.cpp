
#include "SetOptions.h"
#include "../midi/MIDIUtils.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <sstream>

SetOptions::SetOptions(){
	rebuild();
}

void SetOptions::rebuild(){
	// Reset
	_firstNonEmptySet = CHANNELS_COUNT;
	_lastNonEmptySet = -1;
	for(unsigned int sid = 0; sid < CHANNELS_COUNT; ++sid){
		_keysPerSet[sid].clear();
		_keysPerSet[sid].reserve(keys.size());
	}

	// Sort reference keys.
	std::sort(keys.begin(), keys.end());

	for(const KeyFrame& key : keys){
		// Insert in subset, they are already sorted.
		_keysPerSet[key.set].push_back(key);
		// Keep track of bounds.
		_firstNonEmptySet = (std::min)(_firstNonEmptySet, key.set);
		_lastNonEmptySet  = (std::max)( _lastNonEmptySet, key.set);
	}
}

int SetOptions::apply(int note, int channel, int track, double start) const {
	switch(mode){
		case SetMode::CHANNEL:
			return channel % CHANNELS_COUNT;
		case SetMode::TRACK:
			return track % CHANNELS_COUNT;
		case SetMode::SPLIT:
			return note < key ? 0 : 1;
		case SetMode::KEY:
			return noteShift[note % 12] % CHANNELS_COUNT;
		case SetMode::LIST:
		{
			// For each channel, find the corresponding key, and test
			int sid = _firstNonEmptySet;
			for(; sid <= _lastNonEmptySet; ++sid){
				const int keyCount = int(_keysPerSet[sid].size());
				if(keyCount == 0){
					// Ignore this set.
					continue;
				}
				// TODO: binary search instead.
				int kid = 0;
				for( ; kid < keyCount; ++kid){
					if(start < _keysPerSet[sid][kid].time){
						// We found the key after.
						break;
					}
				}
				kid = (std::max)(0, kid - 1);
				if(note < _keysPerSet[sid][kid].key){
					break;
				}

			}
			return glm::clamp(sid, _firstNonEmptySet, _lastNonEmptySet + 1) % CHANNELS_COUNT;
		}
		default:
			assert(false);
			break;
	}
	return 0;
}


std::string SetOptions::toKeysString(const std::string& separator) const {
	std::stringstream str;
	for(const KeyFrame& key : keys){
		str << key.time << "," << key.key << "," << key.set << separator;
	}
	return str.str();
}

std::vector<std::string> split(const std::string & str, const std::string & delimiter, bool skipEmpty){
	std::string subdelimiter = " ";
	if(!delimiter.empty()){
		subdelimiter = delimiter.substr(0,1);
	}
	std::stringstream sstr(str);
	std::string value;
	std::vector<std::string> tokens;
	while(std::getline(sstr, value, subdelimiter[0])) {
		if(!skipEmpty || !value.empty()) {
			tokens.emplace_back(value);
		}
	}
	return tokens;
}

void SetOptions::fromKeysString(const std::string& str){
	// Merge all lines.
	std::string oneLineStr = str;
	for(size_t cid = 0; cid < str.size(); ++cid){
		if(oneLineStr[cid] == '\n' || oneLineStr[cid] == '\r'){
			oneLineStr[cid] = ' ';
		}
	}
	
	std::vector<std::string> tokens = split(oneLineStr, " ", true);
	if(tokens.empty()){
		return;
	}

	keys.clear();
	for(size_t i = 0; i < tokens.size(); ++i){
		std::vector<std::string> values = split(tokens[i], ",", false);
		if(values.size() < 2){
			continue;
		}
		KeyFrame& newKey = keys.emplace_back();
		newKey.time = std::stod(values[0]);
		newKey.key = std::stoi(values[1]);
		if(values.size() > 2){
			newKey.set = std::stoi(values[2]);
		}
	}
	
	rebuild();
}
