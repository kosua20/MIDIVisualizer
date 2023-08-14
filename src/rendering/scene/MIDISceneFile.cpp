#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iterator>

#include "../../helpers/ProgramUtilities.h"
#include "../../helpers/ResourcesManager.h"

#include "MIDISceneFile.h"

#ifdef _WIN32
#undef MIN
#undef MAX
#endif

MIDISceneFile::~MIDISceneFile(){}

MIDISceneFile::MIDISceneFile(const std::string & midiFilePath, const SetOptions & options) : MIDIScene() {

	_filePath = midiFilePath;
	// MIDI processing.
	_midiFile = MIDIFile(_filePath);

	updateSets(options);

	std::cout << "[INFO]: Final track duration " << _midiFile.duration() << " sec." << std::endl;
}


void MIDISceneFile::updateSets(const SetOptions & options){
	// Generate note data for rendering.
	_midiFile.updateSets(options);

	std::vector<MIDINote> notesM;
	_midiFile.getNotes(notesM, NoteType::MAJOR, 0);
	std::vector<MIDINote> notesm;
	_midiFile.getNotes(notesm, NoteType::MINOR, 0);

	// Load notes shared data.
	const size_t majorCount = notesM.size();
	const size_t minorCount = notesm.size();
	const size_t totalCount = majorCount + minorCount;
	_notes.resize(totalCount);

	for(size_t i = 0; i < majorCount; ++i){
		const MIDINote& note = notesM[i];
		GPUNote& data = _notes[i];
		data.note = float(note.note);
		data.start = float(note.start);
		data.duration = float(note.duration);
		data.isMinor = 0.0f;
		data.set = float(note.set);
	}

	for(size_t i = 0; i < minorCount; ++i){
		const MIDINote& note = notesm[i];
		GPUNote& data = _notes[i + majorCount];
		data.note = float(note.note);
		data.start = float(note.start);
		data.duration = float(note.duration);
		data.isMinor =  1.0f;
		data.set = float(note.set);
	}
	// Upload to the GPU.
	assert(totalCount < (1 << 31));
	_dirtyNotes = true;
	_effectiveNotesCount = int(totalCount);
	_dirtyNotesRange = {0, 0}; // Means full array
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
		_actives[i] = note.enabled ? note.set : -1;
		// Check if the note was triggered at this frame.
		if(note.start > _previousTime && note.start <= time){
			// Find an available particles system and update it with the note parameters.
			for(auto & particle : _particles){
				if(particle.note < 0){
					// Update with new note parameter.
					//const float durationTweak = 3.0f - note.velocity / 127.0f * 2.5f;
					particle.duration = (std::max)(note.duration*2.0f, note.duration + 1.2f);
					particle.start = note.start;
					particle.note = i;
					particle.set = note.set;
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

void MIDISceneFile::save(std::ofstream& file) const {
	// Do it the dumb way.
	std::ifstream input(_filePath);
	if(input.is_open() && file.is_open()){
		std::copy(std::istreambuf_iterator<char>(input),
				  std::istreambuf_iterator<char>(),
				  std::ostream_iterator<char>(file));
	}
	input.close();
}

const std::string& MIDISceneFile::filePath() const {
	return _filePath;
}
