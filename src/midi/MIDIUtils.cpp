#include "MIDIUtils.h"

std::unordered_map<MIDIEventType, std::string> MIDIEventTypeName = {
	{ noteOff, "noteOff"},
	{ noteOn, "noteOn"},
	{ afterTouch, "afterTouch"},
	{ controllerChange, "controllerChange"},
	{ programChange, "programChange"},
	{ channelPressure, "channelPressure"},
	{ pitchChange, "pitchChange"}
};

std::unordered_map<MetaEventType, std::string> metaEventTypeName = {
	{sequenceNumber, "sequenceNumber"},
	{textEvent, "textEvent"},
	{copyrightNotice, "copyrightNotice"},
	{sequenceName, "sequenceName"},
	{instrumentName, "instrumentName"},
	{lyric, "lyric"},
	{marker, "marker"},
	{cuePoint, "cuePoint"},
	{channelPrefix, "channelPrefix"},
	{endOfTrack, "endOfTrack"},
	{setTempo, "setTempo"},
	{SMTPEOffset, "SMTPEOffset"},
	{timeSignature, "timeSignature"},
	{keySignature, "keySignature"},
	{sequencerSpecific, "sequencerSpecific"}
};

const std::array<bool, 12> noteIsMinor = {false, true, false, true, false, false, true, false, true, false, true, false};

const std::array<short, 12> noteShift = {0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6};

const char* midiKeysStrings[] = { "C-1", "C-1#", "D-1", "D-1#", "E-1", "F-1", "F-1#", "G-1", "G-1#", "A-1", "A-1#", "B-1", "C0", "C0#", "D0", "D0#", "E0", "F0", "F0#", "G0", "G0#", "A0", "A0#", "B0", "C1", "C1#", "D1", "D1#", "E1", "F1", "F1#", "G1", "G1#", "A1", "A1#", "B1", "C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2", "C3", "C3#", "D3", "D3#", "E3", "F3", "F3#", "G3", "G3#", "A3", "A3#", "B3", "C4", "C4#", "D4", "D4#", "E4", "F4", "F4#", "G4", "G4#", "A4", "A4#", "B4", "C5", "C5#", "D5", "D5#", "E5", "F5", "F5#", "G5", "G5#", "A5", "A5#", "B5", "C6", "C6#", "D6", "D6#", "E6", "F6", "F6#", "G6", "G6#", "A6", "A6#", "B6", "C7", "C7#", "D7", "D7#", "E7", "F7", "F7#", "G7", "G7#", "A7", "A7#", "B7", "C8", "C8#", "D8", "D8#", "E8", "F8", "F8#", "G8", "G8#", "A8", "A8#", "B8", "C9", "C9#", "D9", "D9#", "E9", "F9", "F9#", "G9"
};
