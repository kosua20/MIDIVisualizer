#ifndef SetOptions_h
#define SetOptions_h

#include <vector>
#include <array>
#include <string>

#define SETS_COUNT 12

enum class SetMode : int {
	CHANNEL = 0,
	TRACK = 1,
	SPLIT = 2,
	KEY = 3,
	LIST = 4,
};

struct SetOptions {

	struct KeyFrame {
		double time = 0.0;
		int set = 0;
		int key = 64;


	};

	using KeyFrames = std::vector<KeyFrame>;

	SetMode mode = SetMode::CHANNEL;
	std::vector<KeyFrame> keys;
	int key = 64;

	SetOptions();
	
	void rebuild();

	int apply(int note, int channel, int track, double start) const;

	std::string toKeysString(const std::string& separator) const;

	void fromKeysString(const std::string& str);

private:

	std::array<KeyFrames, SETS_COUNT> _keysPerSet;
	int _firstNonEmptySet = SETS_COUNT;
	int _lastNonEmptySet = -1;
};

inline bool operator <(const SetOptions::KeyFrame& a, const SetOptions::KeyFrame& b){
	return a.time < b.time;
}

#endif
