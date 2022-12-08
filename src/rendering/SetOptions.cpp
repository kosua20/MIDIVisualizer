
#include "SetOptions.h"
#include "../midi/MIDIUtils.h"

#include <algorithm>
#include <glm/glm.hpp>

SetOptions::SetOptions(){
	rebuild();
}

void SetOptions::rebuild(){
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
		default:
			assert(false);
			break;
	}
	return 0;
}
