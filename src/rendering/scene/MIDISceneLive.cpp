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

#define MAX_NOTES_IN_FLIGHT 8192

MIDISceneLive::~MIDISceneLive(){
	shared().close_port();
}

MIDISceneLive::MIDISceneLive(int port) : MIDIScene(){
	// For now we use the same MIDI in instance for everything.
	if(shared().is_port_open()){
		shared().close_port();
	}
	if(port >= 0){
		shared().open_port(port, "MIDIVisualizer input");
	} else {
		shared().open_virtual_port("MIDIVisualizer virtual input");
	}


	_activeIds.fill(-1);
	_activeRecording.fill(false);
	_notes.resize(MAX_NOTES_IN_FLIGHT);
	_notesInfos.resize(MAX_NOTES_IN_FLIGHT);
	_secondsPerMeasure = computeMeasureDuration(_tempo, _signature);
	_pedalInfos[-10000.0f] = Pedals();
	upload(_notes);
}

void MIDISceneLive::updateSet(GPUNote & note, int channel, const SetOptions & options){
	if(options.mode == SetMode::CHANNEL){
		// Restore channel from backup vector.
		note.set = float(int(channel) % CHANNELS_COUNT);
	} else if(options.mode == SetMode::KEY){
		note.set = (note.note < options.key ? 0.0f : 1.0f);
	} else {
		note.set = 0.0f;
	}
}

void MIDISceneLive::updateSets(const SetOptions & options){
	_currentSetOption = options;
	
	for(size_t nid = 0; nid < _notesCount; ++nid){
		auto & note = _notes[nid];
		updateSet(note, _notesInfos[nid].channel, _currentSetOption);
	}
	upload(_notes);
}

void MIDISceneLive::updatesActiveNotes(double time, double speed){
	int minUpdated = MAX_NOTES_IN_FLIGHT;
	int maxUpdated = 0;

	// If we are paused, just empty the queue.
	if(_previousTime == time){
		while(true){
			auto message = shared().get_message();
			if(message.size() == 0){
				// End of the queue.
				break;
			}
		}
		return;
	}

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

	// Restore all active flags.
	for(size_t nid = 0; nid < _actives.size(); ++nid){
		_actives[nid] = -1;
	}

	// Update all currently recording notes, extending their duration.
	for(size_t nid = 0; nid < _actives.size(); ++nid){
		if(!_activeRecording[nid]){
			continue;
		}
		const int noteId = _activeIds[nid];
		GPUNote & note = _notes[noteId];
		note.duration = float(time - double(note.start));
		_actives[nid] = int(note.set);
		// Keep track of which region was modified.
		minUpdated = std::min(minUpdated, noteId);
		maxUpdated = std::max(maxUpdated, noteId);
	}

	// Restore pedals to the last known state.
	_pedals = Pedals();
	auto nextBig = _pedalInfos.upper_bound(float(time));
	if(nextBig != _pedalInfos.begin()){
		_pedals = std::prev(nextBig)->second;
	}

	// Process new events.
	while(true){
		auto message = shared().get_message();
		if(message.size() == 0){
			// End of the queue.
			break;
		}

		const auto type = message.get_message_type();

		// Handle note events.
		if(message.is_note_on_or_off()){
			short note = short(message[1]);

			// If the note is currently recording, disable it.
			if(_activeRecording[note]){
				_activeRecording[note] = false;
				_actives[note] = -1;
				// Duration has already been updated above.
			}

			// If this is an on event with positive veolcity, start a new note.
			if(type == rtmidi::message_type::NOTE_ON && message[2] > 0){

				const int index = _notesCount % MAX_NOTES_IN_FLIGHT;
				// Get new note.
				auto & newNote = _notes[index];
				newNote.start = float(time);
				newNote.duration = 0.0f;
				// Save the original channel.
				_notesInfos[index].channel = message.get_channel();
				// Compute set according to current setting.
				updateSet(newNote, _notesInfos[index].channel, _currentSetOption);
				_actives[note] = int(newNote.set);
				// Activate recording of the key.
				_activeRecording[note] = true;
				_activeIds[note] = index;

				// Compute proper rendering note.
				const bool isMin = noteIsMinor[note % 12];
				const short shiftId = (note/12) * 7 + noteShift[note % 12];
				newNote.isMinor = isMin ? 1.0f : 0.0f;
				newNote.note = float(shiftId);
				// Save original note.
				_notesInfos[index].note = note;

				// Keep track of which region was modified.
				minUpdated = std::min(minUpdated, int(index));
				maxUpdated = std::max(maxUpdated, int(index));

				// Find an available particles system and update it with the note parameters.
				for(auto & particle : _particles){
					if(particle.note < 0){
						// Update with new note parameter.
						particle.duration = 10.0f; // Fixed value.
						particle.start = newNote.start;
						particle.note = note;
						particle.set = int(newNote.set);
						particle.elapsed = 0.0f;
						break;
					}
				}

				++_notesCount;
			}

		} else if(message.is_meta_event()){
			// Handle tempo and signature changes.
			const rtmidi::meta_event_type metaType = message.get_meta_event_type();

			if(metaType == rtmidi::meta_event_type::TIME_SIGNATURE){
				_signature = double(message[3]) / double(std::pow(2, short(message[4])));
				_secondsPerMeasure = computeMeasureDuration(_tempo, _signature);

			} else if(metaType == rtmidi::meta_event_type::TEMPO_CHANGE){
				_tempo = int(((message[3] & 0xFF) << 16) | ((message[4] & 0xFF) << 8) | (message[5] & 0xFF));
				_secondsPerMeasure = computeMeasureDuration(_tempo, _signature);
			}

		} else if(type == rtmidi::message_type::CONTROL_CHANGE){
			// Handle pedal.
			const int rawType = message[1];
			// Skip other CC.
			if(rawType != 64 && rawType != 66 && rawType != 67 && rawType != 11){
				continue;
			}
			const PedalType type = PedalType(rawType);

			float & pedal = (type == DAMPER ? _pedals.damper : (type == SOSTENUTO ? _pedals.sostenuto : (type == SOFT ? _pedals.soft : _pedals.expression)));
			// Stop the current pedal.
			pedal = 0.0f;
			// If non-zero velocity, enable pedal, normalized velocity.
			if(message[2] > 0){
				pedal = float(message[2])/127.0f;
			}
			// Register new pedal event with updated state.
			_pedalInfos[float(time)] = Pedals(_pedals);
		}

	}

	// Update completed notes.
	for(size_t i = 0; i < _dataBufferSubsize; ++i){
		const auto & noteId = _notesInfos[i];
		// If the key is recording, no need to update _actives, skip.
		if(_activeRecording[noteId.note]){
			continue;
		}

		auto& note = _notes[i];
		// Ignore notes that ended at this frame.
		float noteEnd = note.start+note.duration;
		if(noteEnd > _previousTime && noteEnd <= time){
			continue;
		}
		// Update for notes currently playins.
		if(note.start <= time && note.start+note.duration >= time){
			_actives[noteId.note] = int(note.set);
		}
		// Detect notes that started at this frame.
		if(note.start > _previousTime && note.start <= time){
			// Find an available particles system and update it with the note parameters.
			for(auto & particle : _particles){
				if(particle.note < 0){
					// Update with new note parameter.
					particle.duration = (std::max)(note.duration*2.0f, note.duration + 1.2f);
					particle.start = note.start;
					particle.note = noteId.note;
					particle.set = int(note.set);
					particle.elapsed = 0.0f;
					break;
				}
			}
		}
	}

	// If we have indeed updated a note, trigger an upload.
	if(minUpdated <= maxUpdated){
		upload(_notes, minUpdated, maxUpdated);
	}
	// Update range of notes to show.
	_dataBufferSubsize = std::min(MAX_NOTES_IN_FLIGHT, _notesCount);
	// Update timings.
	_previousTime = time;
	_maxTime = std::max(time, _maxTime);
}

double MIDISceneLive::duration() const {
	return _maxTime;
}

double MIDISceneLive::secondsPerMeasure() const {
	return _secondsPerMeasure;
}

int MIDISceneLive::notesCount() const {
	return _notesCount;
}

void MIDISceneLive::print() const {
	std::cout << "[INFO]: Live scene with " << notesCount() << " notes, duration " << duration() << "s." << std::endl;
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
