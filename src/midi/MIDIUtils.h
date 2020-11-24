#ifndef MIDI_UTILS_H
#define MIDI_UTILS_H

#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <array>

enum class SetMode : int {
	CHANNEL = 0,
	TRACK = 1,
	KEY = 2
};

struct SetOptions {
	SetMode mode = SetMode::CHANNEL;
	int key = 64;
};

enum MIDIType : uint16_t {
	singleTrack = 0,
	tempoTrack = 1,
	multipleSongs = 2 };

enum MetaEventType : uint8_t {
	sequenceNumber = 0x0,
	textEvent = 0x1,
	copyrightNotice = 0x02,
	sequenceName = 0x03,
	instrumentName = 0x04,
	lyric = 0x05,
	marker = 0x06,
	cuePoint = 0x07,
	channelPrefix = 0x20,
	endOfTrack = 0x2F,
	setTempo = 0x51,
	SMTPEOffset = 0x54,
	timeSignature = 0x58,
	keySignature = 0x59,
	sequencerSpecific = 0x7F
};

enum MIDIEventType : uint8_t {
	noteOff = 0x8,
	noteOn = 0x9,
	afterTouch = 0xA,
	controllerChange = 0xB,
	programChange = 0xC,
	channelPressure = 0xD,
	pitchChange = 0xE,
	ignoreType = 0x0
	
};

enum class EventCategory {
	MIDI, SYSTEM, META
};

enum class NoteType{
	MAJOR, MINOR, ALL
};

enum PedalType : uint8_t {
	EXPRESSION = 11, DAMPER = 64, SOSTENUTO = 66, SOFT = 67
};

// Debug print data.

extern std::map<MetaEventType, std::string> metaEventTypeName;

extern std::map<MIDIEventType, std::string> MIDIEventTypeName;

// Keyboard shifts.

extern const std::array<bool, 12> noteIsMinor;

extern const std::array<short, 12> noteShift;

extern const char midiKeysString[];

// Read data.

inline uint32_t read32(const std::vector<char>& buffer, size_t position){
	return (buffer[position] & 0xFF) << 24 | (buffer[position+1] & 0xFF) << 16 | (buffer[position+2] & 0xFF) << 8 | (buffer[position+3] & 0xFF);
}

inline uint8_t getBit(uint32_t number, short bit){
	return (number & (0x1 << bit)) >> bit;
}

inline uint16_t read16(const std::vector<char>& buffer, size_t position){
	return (buffer[position] & 0xFF) << 8 | (buffer[position+1] & 0xFF);
}

inline uint8_t getBit(uint16_t number, short bit){
	return (number & (0x1 << bit)) >> bit;
}

inline uint8_t read8(const std::vector<char>& buffer, size_t position){
	return buffer[position] & 0xFF;
}

inline uint8_t getBit(uint8_t number, short bit){
	return (number & (0x1 << bit)) >> bit;
}

inline size_t readVarLen(const std::vector<char>& buffer, size_t & position){
	size_t lastIndex = 0;
	size_t accum = 0;
	uint8_t currentByte = read8(buffer, position + lastIndex);
	while(currentByte & 0x80){
		accum = (accum << 7) | (currentByte & 0x7F);
		lastIndex += 1;
		currentByte = read8(buffer, position + lastIndex);
	}
	accum = (accum << 7) | (currentByte & 0x7F);
	position = position + lastIndex + 1;
	return accum;
}

// Time computations.

inline double computeMeasureDuration(int tempo, double signature){
	// micro s / measure = (micro s / qnote) * (clock / qnote)^-1 * (clock / beat) * (beat / measure)
	//					 = tempo * 24^-1 * cc * time signature
	return double(tempo) * 4.0 * signature / 1000000.0;
}

inline double computeUnitsDuration(int tempo, size_t time, uint16_t unitsPerQuarterNote){
	return double(tempo) / double(unitsPerQuarterNote) * double(time);
}


#endif // MIDI_UTILS_H
