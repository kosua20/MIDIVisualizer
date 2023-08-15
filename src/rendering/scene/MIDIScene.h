#pragma once
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include "../midi/MIDIFile.h"
#include "../State.h"
#include "../../helpers/ProgramUtilities.h"

#include <fstream>

class MIDIScene {

public:

	struct Pedals {
		float damper = 0.0f;
		float sostenuto = 0.0f;
		float soft = 0.0f;
		float expression = 0.0f;
	};

	struct Particles {
		int note = -1;
		int set = -1;
		float duration = 0.0f;
		float start = 1000000.0f;
		float elapsed = 0.0f;
	};

	struct GPUNote {
		float note = 0.0f;
		float start = 0.0f;
		float duration = 0.0f;
		float isMinor = 0.0f;
		float set = 0.0f;
	};


	MIDIScene();

	void resetParticles();

	virtual void updateSets(const SetOptions & options);

	virtual void updatesActiveNotes(double time, double speed);

	virtual double duration() const;

	virtual double secondsPerMeasure() const;

	virtual int notesCount() const;

	virtual void print() const;

	virtual void save(std::ofstream& file) const;

	virtual ~MIDIScene();

	// Getters

	const Pedals& getPedals() const { return _pedals; }

	const std::array<int, 128>& getActiveKeys() const { return _actives; }

	const std::vector<Particles>& getParticles() const { return _particles; }

	const std::vector<GPUNote>& getNotes() const { return _notes; };

	int getEffectiveNotesCount() const { return _effectiveNotesCount; }

	bool dirtyNotes(glm::ivec2& range) { range = _dirtyNotesRange; return _dirtyNotes; }

	void setUpToDate() { _dirtyNotes = false; _dirtyNotesRange = {0, 0}; }

protected:

	std::vector<GPUNote> _notes;
	std::array<int, 128> _actives;
	std::vector<Particles> _particles;
	Pedals _pedals;
	int _effectiveNotesCount = 0;

	glm::ivec2 _dirtyNotesRange{0,0};
	bool _dirtyNotes = true;
	// Active keys, particles and pedals are always dirty.

};
