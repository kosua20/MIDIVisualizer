
#include "SetOptions.h"
#include "../midi/MIDIUtils.h"

#include <algorithm>
#include <glm/glm.hpp>

SetOptions::SetOptions(){
	rebuild();
}

void SetOptions::rebuild(){
	_firstNonEmptySet = keys.size();
	_lastNonEmptySet = -1;

	for(unsigned int i = 0; i < keys.size(); ++i){
		KeyFrames& frames = keys[i];
		std::sort(frames.begin(), frames.end(), [](const KeyFrame& a, const KeyFrame& b){
			return a.time < b.time;
		});
		if(!frames.empty()){
			_firstNonEmptySet = (std::min)(_firstNonEmptySet, int(i));
			_lastNonEmptySet  = (std::max)( _lastNonEmptySet, int(i));
		}
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
				const int keyCount = int(keys[sid].size());
				if(keyCount == 0){
					// Ignore this set.
					continue;
				}
				// TODO: binary search instead.
				int kid = 0;
				for( ; kid < keyCount; ++kid){
					if(start < keys[sid][kid].time){
						// We found the key after.
						break;
					}
				}
				kid = (std::max)(0, kid - 1);
				if(note < keys[sid][kid].key){
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
