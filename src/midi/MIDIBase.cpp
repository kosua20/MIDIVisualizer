#include "MIDIBase.h"

MIDINote::MIDINote(short aNote, double aStart, double aDuration, short aVelocity, short aChannel, unsigned int trackId) : start(aStart), duration(aDuration), track(trackId), set(0), note(aNote), velocity(aVelocity), channel(aChannel) {

}

MIDIEvent::MIDIEvent(EventCategory aCategory, uint8_t aType, size_t aDelta, std::vector<short> aData) : category(aCategory), type(aType), delta(aDelta), data(aData) {

}

MIDITempo::MIDITempo(){

}

MIDITempo::MIDITempo(size_t astart, unsigned int atempo) : start(astart), tempo(atempo) {
	
}

MIDIPedal::MIDIPedal(PedalType aType, double aStart, double aDuration, float aVelocity) : start(aStart), duration(aDuration), type(aType), velocity(aVelocity) {

}

void MIDINote::print() const {
	std::cout << "[INFO]: Note " << note << " (" << duration << "s at "<< start << "s), on channel " << channel << " with velocity " << velocity << "." << std::endl;
}

void MIDIEvent::print() const {
	if(category == EventCategory::SYSTEM){
		std::cout << "[INFO]: " << "Sysex event (" << delta << "): type is "<< std::hex << std::showbase << type << std::dec << ", length is " << data.size() << std::endl;
	} else if (category == EventCategory::META){
		std::cout << "[INFO]: " << "Meta event (" << delta << "): type is " << metaEventTypeName[static_cast<MetaEventType>(type)] << ", length is " << data.size() << std::endl;
	} else if (category == EventCategory::MIDI){
		const auto typeName = MIDIEventTypeName.find(static_cast<MIDIEventType>(type));
		if(typeName != MIDIEventTypeName.end()){
			std::cout << "[INFO]: " << "MIDI Event " << typeName->second << " (" << delta << ") on channel " << data[0] << " with note " << data[1] << " and velocity " << data[2] << "." << std::endl;
		} else {
			std::cout << "[INFO]: " << "MIDI Event unknown (" << delta << "), data size " << data.size() << "." << std::endl;
		}
	}
}

void MIDITempo::print() const {
	std::cout << "[INFO]: Tempo " << tempo << " (at "<< start << "u, " << timestamp << "us)." << std::endl;
}

void MIDIPedal::print() const {
	std::cout << "[INFO]: Pedal " << int(type) << " (at "<< start << "s, " << duration << "s) with velocity " << velocity << "." << std::endl;
}

MIDIEvent MIDIEvent::readMIDIEvent(const std::vector<char> & buffer, size_t & position, size_t delta, uint8_t & previousFirstByte){

	uint8_t firstByte = read8(buffer, position);
	size_t positionOffset = 1;

	MIDIEventType type = static_cast<MIDIEventType>((firstByte & 0xF0) >> 4);
	/*if (type == ignoreType) {
		position += positionOffset;
		return MIDIEvent(midiEvent, static_cast<uint8_t>(type), delta, {});
	}*/

	uint8_t secondByte = read8(buffer, position+1);
	uint8_t thirdByte = 0;
	positionOffset += 1;
	if(type != programChange && type != channelPressure){
		thirdByte = read8(buffer, position+2);
		positionOffset += 1;
	}

	if(MIDIEventTypeName.count(type) == 0){
		thirdByte = secondByte;
		secondByte = firstByte;
		firstByte = previousFirstByte;
		positionOffset -= 1;
	}

	type = static_cast<MIDIEventType>((firstByte & 0xF0) >> 4);

	short channel = firstByte & 0x0F;

	short note = short(secondByte);
	short velocity = short(thirdByte);

	std::vector<short> data;
	data.push_back(channel);
	data.push_back(note);
	data.push_back(velocity);

	previousFirstByte = firstByte;
	position += positionOffset;

	return MIDIEvent(EventCategory::MIDI, static_cast<uint8_t>(type), delta, data);
}


MIDIEvent MIDIEvent::readMetaEvent(const std::vector<char> & buffer, size_t & position, size_t delta){
	position += 1; // We already read FF.
	MetaEventType type = static_cast<MetaEventType>(read8(buffer, position));
	position += 1;

	size_t length = readVarLen(buffer, position);

	std::vector<short> data;
	for(size_t i = 0; i < length; ++i){
		data.push_back(short(read8(buffer, position + i)));
	}

	position = position + length;

	return MIDIEvent(EventCategory::META, static_cast<uint8_t>(type), delta, data);
}


MIDIEvent MIDIEvent::readSysexEvent(const std::vector<char> & buffer, size_t & position, size_t delta){
	uint8_t type = read8(buffer, position);
	position += 1;

	size_t length = readVarLen(buffer,position);

	std::vector<short> data;
	for(size_t i = 0; i < length; ++i){
		data.push_back(short(read8(buffer, position + i)));
	}

	position = position + length;

	return MIDIEvent(EventCategory::SYSTEM, type, delta, data);
}

