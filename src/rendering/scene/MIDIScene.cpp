#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>


#include "MIDIScene.h"

#ifdef _WIN32
#undef MIN
#undef MAX
#endif

MIDIScene::~MIDIScene(){}

MIDIScene::MIDIScene(){
	// Prepare actives notes array.
	_actives.fill(-1);
	// Particle systems pool.
	_particles = std::vector<Particles>(256);
	_notes = {GPUNote()};
}

void MIDIScene::resetParticles() {
	for (auto & particle : _particles) {
		particle.note = -1;
		particle.set = -1;
		particle.duration = particle.start = particle.elapsed = 0.0f;
	}
}

void MIDIScene::updateSetsAndVisibleNotes(const SetOptions & options, const FilterOptions& filter){
}

void MIDIScene::updateVisibleNotes( const FilterOptions& filter ){
}

void MIDIScene::updatesActiveNotes(double time, double speed, const FilterOptions& filter ){
}

double MIDIScene::duration() const {
	return 0.0;
}

double MIDIScene::secondsPerMeasure() const {
	return 1.0;
}

int MIDIScene::notesCount() const {
	return 0;
}

void MIDIScene::print() const {
}

void MIDIScene::save(std::ofstream& file) const {
}
