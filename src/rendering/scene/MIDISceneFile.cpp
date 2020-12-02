#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "../../helpers/ProgramUtilities.h"
#include "../../helpers/ResourcesManager.h"

#include "MIDISceneFile.h"

#ifdef _WIN32
#undef MIN
#undef MAX
#endif

MIDISceneFile::~MIDISceneFile(){}

MIDISceneFile::MIDISceneFile(const std::string & midiFilePath, const SetOptions & options) : MIDIScene() {

	// MIDI processing.
	_midiFile = MIDIFile(midiFilePath);

	updateSets(options);

	std::cout << "[INFO]: Final track duration " << _midiFile.duration() << " sec." << std::endl;
}


void MIDISceneFile::updateSets(const SetOptions & options){
	// Generate note data for rendering.
	_midiFile.updateSets(options);

	// Load notes shared data.
	std::vector<GPUNote> data;
	std::vector<MIDINote> notesM;
	_midiFile.getNotes(notesM, NoteType::MAJOR, 0);
	for(auto& note : notesM){
		data.emplace_back();
		data.back().note = float(note.note);
		data.back().start = float(note.start);
		data.back().duration = float(note.duration);
		data.back().isMinor = 0.0f;
		data.back().set = float(note.set % CHANNELS_COUNT);
	}

	std::vector<MIDINote> notesm;
	_midiFile.getNotes(notesm, NoteType::MINOR, 0);
	for(auto& note : notesm){
		data.emplace_back();
		data.back().note = float(note.note);
		data.back().start = float(note.start);
		data.back().duration = float(note.duration);
		data.back().isMinor =  1.0f;
		data.back().set = float(note.set % CHANNELS_COUNT);
	}
	// Upload to the GPU.
	upload(data);
	_dataBufferSubsize = int(data.size());
}

void MIDISceneFile::updatesActiveNotes(double time, double speed){
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
	// Get notes actives.
	auto actives = ActiveNotesArray();
	_midiFile.getNotesActive(actives, time, 0);
	for(int i = 0; i < 128; ++i){
		const auto & note = actives[i];
		const int clamped = note.set % CHANNELS_COUNT;
		_actives[i] = note.enabled ? clamped : -1;
		// Check if the note was triggered at this frame.
		if(note.start > _previousTime && note.start <= time){
			// Find an available particles system and update it with the note parameters.
			for(auto & particle : _particles){
				if(particle.note < 0){
					// Update with new note parameter.
					particle.duration = (std::max)(note.duration*2.0f, note.duration + 1.2f);
					particle.start = note.start;
					particle.note = i;
					particle.set = clamped;
					particle.elapsed = 0.0f;
					break;
				}
			}
		}
	}
	_previousTime = time;

	// Update pedal state.
	_pedals.damper = _pedals.sostenuto = _pedals.soft = _pedals.expression = 0.0f;
	_midiFile.getPedalsActive(_pedals.damper, _pedals.sostenuto, _pedals.soft, _pedals.expression, time, 0);
}

double MIDISceneFile::duration() const {
	return _midiFile.duration();
}

double MIDISceneFile::secondsPerMeasure() const {
	return _midiFile.secondsPerMeasure();
}

int MIDISceneFile::notesCount() const {
	return _midiFile.notesCount();
}

void MIDISceneFile::print() const {
	_midiFile.print();
}
