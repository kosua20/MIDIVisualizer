#include "MIDIUtils.h"

std::map<MIDIEventType, std::string> MIDIEventTypeName = {
	{ noteOff, "noteOff"},
	{ noteOn, "noteOn"},
	{ afterTouch, "afterTouch"},
	{ controllerChange, "controllerChange"},
	{ programChange, "programChange"},
	{ channelPressure, "channelPressure"},
	{ pitchChange, "pitchChange"}
};

std::map<MetaEventType, std::string> metaEventTypeName = {
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

const char midiKeysString[] = "C-1\0C-1#\0D-1\0D-1#\0E-1\0F-1\0F-1#\0G-1\0G-1#\0A-1\0A-1#\0B-1\0C0\0C0#\0D0\0D0#\0E0\0F0\0F0#\0G0\0G0#\0A0\0A0#\0B0\0C1\0C1#\0D1\0D1#\0E1\0F1\0F1#\0G1\0G1#\0A1\0A1#\0B1\0C2\0C2#\0D2\0D2#\0E2\0F2\0F2#\0G2\0G2#\0A2\0A2#\0B2\0C3\0C3#\0D3\0D3#\0E3\0F3\0F3#\0G3\0G3#\0A3\0A3#\0B3\0C4\0C4#\0D4\0D4#\0E4\0F4\0F4#\0G4\0G4#\0A4\0A4#\0B4\0C5\0C5#\0D5\0D5#\0E5\0F5\0F5#\0G5\0G5#\0A5\0A5#\0B5\0C6\0C6#\0D6\0D6#\0E6\0F6\0F6#\0G6\0G6#\0A6\0A6#\0B6\0C7\0C7#\0D7\0D7#\0E7\0F7\0F7#\0G7\0G7#\0A7\0A7#\0B7\0C8\0C8#\0D8\0D8#\0E8\0F8\0F8#\0G8\0G8#\0A8\0A8#\0B8\0C9\0C9#\0D9\0D9#\0E9\0F9\0F9#\0G9\0";
