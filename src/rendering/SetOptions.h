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
	LIST = 4,
};

struct SetOptions {

	struct KeyFrame {
		double time;
		int key;
	};

	using KeyFrames = std::vector<KeyFrame>;

	SetMode mode = SetMode::CHANNEL;
	std::array<KeyFrames, CHANNELS_COUNT> keys;
	int key = 64;

	SetOptions();
	
	void rebuild();

	int apply(int note, int channel, int track, double start) const;

private:

	int _firstNonEmptySet = CHANNELS_COUNT;
	int _lastNonEmptySet = -1;
};

#endif
