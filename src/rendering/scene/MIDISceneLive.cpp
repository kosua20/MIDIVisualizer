#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "../../helpers/ProgramUtilities.h"
#include "../../helpers/ResourcesManager.h"

#include "MIDISceneLive.h"


#ifdef _WIN32
#undef MIN
#undef MAX
#endif

#define MAX_NOTES_IN_FLIGHT 1024

MIDISceneLive::~MIDISceneLive(){
	shared().close_port();
}

MIDISceneLive::MIDISceneLive(int port) : MIDIScene(){
	// For now we use the same MIDI in instance for everything.
	if(shared().is_port_open()){
		shared().close_port();
	}
	shared().open_port(port, "MIDIVisualizer input");

	_activeIds.fill(-1);
	_notes.resize(MAX_NOTES_IN_FLIGHT);
	_secondsPerMeasure = computeMeasureDuration(_tempo, _signature);
}

void MIDISceneLive::updateSets(const SetOptions & options){

}

void MIDISceneLive::updatesActiveNotes(double time, double speed){
	int minUpdated = MAX_NOTES_IN_FLIGHT;
	int maxUpdated = 0;

	// Update the particle systems lifetimes.
	for(auto & particle : _particles){
		// Give a bit of a head start to the animation.
		particle.elapsed = (float(time) - particle.start + 0.25f) / (float(speed) * particle.duration);
		// Disable particles that shouldn't be visible at the current time.
		if(float(time) >= particle.start + particle.duration || float(time) < particle.start){
			particle.note = -1;
			particle.set = -1;
			particle.duration = particle.start = particle.elapsed = 0.0f;
		}
	}

	// Update all active notes, extending their duration.
	for(size_t nid = 0; nid < _actives.size(); ++nid){
		if(_actives[nid] < 0){
			continue;
		}
		const int noteId = _activeIds[nid];
		GPUNote & note = _notes[noteId];
		note.duration = time - note.start;
		// Keep track of which region was modified.
		minUpdated = std::min(minUpdated, noteId);
		maxUpdated = std::max(maxUpdated, noteId);
	}

	while(true){
		auto message = shared().get_message();
		if(message.size() == 0){
			// End of the queue.
			break;
		}

		const auto type = message.get_message_type();

		if(message.is_note_on_or_off()){
			short note = short(message[1]);

			// If the note is currently active, disable it.
			if(_actives[note] >= 0){
				_actives[note] = -1;
				// Keep the note as-is, complete.
				// Duration has already been updated above.
			}

			// Now if this is an on event, we should start a new note.
			if(type == rtmidi::message_type::NOTE_ON && message[2] > 0){
				const int clamped = message.get_channel() % CHANNELS_COUNT;
				const size_t index = _notesCount % MAX_NOTES_IN_FLIGHT;

				// Activate the key.
				_actives[note] = clamped;
				_activeIds[note] = index;

				auto & newNote = _notes[index];
				newNote.start = time;
				newNote.duration = 0.0f;
				newNote.set = clamped;

				const bool isMin = noteIsMinor[note % 12];
				const short shiftId = (note/12) * 7 + noteShift[note % 12];
				newNote.isMinor = isMin ? 1.0f : 0.0f;
				newNote.note = float(shiftId);

				// Keep track of which region was modified.
				minUpdated = std::min(minUpdated, int(index));
				maxUpdated = std::max(maxUpdated, int(index));

				// Find an available particles system and update it with the note parameters.
				for(auto & particle : _particles){
					if(particle.note < 0){
						// Update with new note parameter.
						particle.duration = 10.0f; // Fixed value.
						particle.start = time;
						particle.note = note;
						particle.set = clamped;
						particle.elapsed = 0.0f;
						break;
					}
				}

				++_notesCount;
			}

		} else if(message.is_meta_event()){
			const rtmidi::meta_event_type metaType = message.get_meta_event_type();

			if(metaType == rtmidi::meta_event_type::TIME_SIGNATURE){
				_signature = double(message[3]) / double(std::pow(2, short(message[4])));
				_secondsPerMeasure = computeMeasureDuration(_tempo, _signature);

			} else if(metaType == rtmidi::meta_event_type::TEMPO_CHANGE){
				_tempo = int(((message[3] & 0xFF) << 16) | ((message[4] & 0xFF) << 8) | (message[5] & 0xFF));
				_secondsPerMeasure = computeMeasureDuration(_tempo, _signature);
			}

			// Handle pedal.
		}
	}

	_dataBufferSubsize = std::min(int(_notes.size()), _notesCount);

	// If we have indeed updated a note.
	if(minUpdated <= maxUpdated){
		upload(_notes, minUpdated, maxUpdated);
	}

	_previousTime = time;

	// Update pedal state.
	_pedals.damper = _pedals.sostenuto = _pedals.soft = _pedals.expression = 0.0f;
	//_midiFile.getPedalsActive(_pedals.damper, _pedals.sostenuto, _pedals.soft, _pedals.expression, time, 0);
}

double MIDISceneLive::duration() const {
	return _previousTime;
}

double MIDISceneLive::secondsPerMeasure() const {
	return _secondsPerMeasure;
}

int MIDISceneLive::notesCount() const {
	return _notesCount;
}

void MIDISceneLive::print() const {

}

rtmidi::midi_in * MIDISceneLive::_sharedMIDIIn = nullptr;
std::vector<std::string> MIDISceneLive::_availablePorts;
int MIDISceneLive::_refreshIndex = 0;

rtmidi::midi_in & MIDISceneLive::shared(){
	if(_sharedMIDIIn == nullptr){
		_sharedMIDIIn = new rtmidi::midi_in(rtmidi::API::UNSPECIFIED, "MIDIVisualizer");
	}
	return *_sharedMIDIIn;
}

const std::vector<std::string> & MIDISceneLive::availablePorts(){
	if(_refreshIndex == 0){
		const int portCount = shared().get_port_count();
		_availablePorts.resize(portCount);

		for(int i = 0; i < portCount; ++i){
			_availablePorts[i] = shared().get_port_name(i);
		}
	}
	// Only update once every 15 frames.
	_refreshIndex = (_refreshIndex + 1) % 15;
	return _availablePorts;
}
