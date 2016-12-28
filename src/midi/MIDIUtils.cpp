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

std::vector<bool> noteIsMinor = { false, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false, true, false, true, false, false, true, false, true, false, true, false, false };

std::vector<short> noteShift = { 0, 0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 9, 9, 10, 10, 11, 12, 12, 13, 13, 14, 14, 15, 16, 16, 17, 17, 18, 19, 19, 20, 20, 21, 21, 22, 23, 23, 24, 24, 25, 26, 26, 27, 27, 28, 28, 29, 30, 30, 31, 31, 32, 33, 33, 34, 34, 35, 35, 36, 37, 37, 38, 38, 39, 40, 40, 41, 41, 42, 42, 43, 44, 44, 45, 45, 46, 47, 47, 48, 48, 49, 49, 50, 51 };
