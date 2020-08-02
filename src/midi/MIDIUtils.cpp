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

std::array<bool, 12> noteIsMinor = {false, true, false, true, false, false, true, false, true, false, true, false};

std::array<short, 12> noteShift = {0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6};

