
#include "SetOptions.h"
#include "../midi/MIDIUtils.h"

#include <algorithm>
#include <glm/glm.hpp>

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
