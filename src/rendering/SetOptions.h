#ifndef SetOptions_h
#define SetOptions_h

#include <vector>
#include <array>

#define CHANNELS_COUNT 8

enum class SetMode : int {
	CHANNEL = 0,
	TRACK = 1,
	SPLIT = 2,
	KEY = 3,
};

struct SetOptions {

	SetMode mode = SetMode::CHANNEL;
	int key = 64;

	SetOptions();
	
	void rebuild();

	int apply(int note, int channel, int track, double start) const;

};

#endif
