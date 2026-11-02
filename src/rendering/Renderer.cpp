#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "../helpers/ResourcesManager.h"
#include <lodepng/lodepng.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>

#include "Renderer.h"
#include "Framebuffer.h"
#include "scene/MIDIScene.h"

#include <imgui/imgui.h>

#ifdef _WIN32
#undef MIN
#undef MAX
#endif


void Renderer::renderSetup(){
	// - Buffers

	// -- Basic quad
	std::vector<float> vertices = {-0.5,-0.5, 0.5, -0.5, 0.5,0.5, -0.5, 0.5};
	std::vector<unsigned int> indices = {1, 2, 0, 2, 3, 0};

	// Create an array buffer to host the geometry data.

	glGenBuffers(1, &_quadVertices);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);

	// We load the indices data
	glGenBuffers(1, &_quadIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &(indices[0]), GL_STATIC_DRAW);
	_quadPrimitiveCount = indices.size();

	// -- Notes data
	// Current notes on screen
	_notesDataBuffer = 0;
	glGenBuffers(1, &_notesDataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _notesDataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 1000, nullptr, GL_STATIC_DRAW);

	// Active keys buffer (empty for now).
	_keysDataBuffer = 0;
	glGenBuffers(1, &_keysDataBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _keysDataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 128, nullptr, GL_DYNAMIC_DRAW);

	// -- Wave strips
	const int numSegments = 512;
	std::vector<glm::vec2> waveVerts((numSegments+1)*2);
	for(int vid = 0; vid < numSegments+1; ++vid){
		const float x = 2.0f * float(vid) / float(numSegments) - 1.0f;
		waveVerts[2*vid] = glm::vec2(x, 1.0f);
		waveVerts[2*vid+1] = glm::vec2(x, -1.0f);
	}
	std::vector<unsigned int> waveInds(3 * 2 * numSegments);
	for(int sid = 0; sid < numSegments; ++sid){
		const int bid = 6 * sid;
		const int vid = 2 * sid;
		waveInds[bid  ] = vid;
		waveInds[bid+1] = vid + 1;
		waveInds[bid+2] = vid + 2;
		waveInds[bid+3] = vid + 2;
		waveInds[bid+4] = vid + 1;
		waveInds[bid+5] = vid + 3;
	}

	glGenBuffers(1, &_waveVertices);
	glBindBuffer(GL_ARRAY_BUFFER, _waveVertices);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat) * waveVerts.size(), &(waveVerts[0][0]), GL_STATIC_DRAW);
	// Corresponding indices.
	glGenBuffers(1, &_waveIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _waveIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * waveInds.size(), &(waveInds[0]), GL_STATIC_DRAW);
	_wavePrimitiveCount = waveInds.size();

	// Reset state
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// - Vertex arrays

	// -- Basic quad
	glGenVertexArrays(1, &_vaoQuad);
	glBindVertexArray(_vaoQuad);
	// Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);
	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIndices);
	glBindVertexArray(0);

	// -- Quad with notes data.
	glGenVertexArrays(1, &_vaoQuadWithNoteData);
	glBindVertexArray(_vaoQuadWithNoteData);
	// Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);
	// Notes data part 1
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _notesDataBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
	glVertexAttribDivisor(1, 1);
	// Notes data part 2
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, _notesDataBuffer);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
	glVertexAttribDivisor(2, 1);
	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIndices);
	glBindVertexArray(0);

	// -- Quad with active keys data.
	glGenVertexArrays(1, &_vaoQuadWithKeyData);
	glBindVertexArray(_vaoQuadWithKeyData);
	// Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);
	// Keys data
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _keysDataBuffer);
	glVertexAttribIPointer(1, 1, GL_INT, 0, NULL);
	glVertexAttribDivisor(1, 1);
	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quadIndices);
	glBindVertexArray(0);

	// -- Wave
	glGenVertexArrays(1, &_vaoWave);
	glBindVertexArray(_vaoWave);
	// Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _waveVertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);
	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _waveIndices);
	glBindVertexArray(0);

	checkGLError();

	// Programs.
	_programNotes.init("notes_vert", "notes_frag");
	_programFlashes.init("flashes_vert", "flashes_frag");
	_programParticules.init("particles_vert", "particles_frag");
	_programKeyMajors.init("majorKeys_vert", "majorKeys_frag");
	_programKeyMinors.init("minorKeys_vert", "minorKeys_frag");
	_programKeys3D.init("keys3d_vert", "keys3d_frag");
	_keys3DUpscale.init("screenquad_frag");
	_programPedals.init("pedal_vert", "pedal_frag");
	_programScoreBars.init("score_bars_vert", "score_bars_frag");
	_programScoreLabels.init("score_labels_vert", "score_labels_frag");
	_programWave.init("wave_vert", "wave_frag");
	_programWaveNoise.init("wave_noise_vert", "wave_noise_frag");

	// Pass texture size to shader.
	_texFont = ResourcesManager::getTextureFor("font");
	_texNoise = ResourcesManager::getTextureFor("noise");
	_texParticles = ResourcesManager::getTextureFor("particles");
	const glm::vec2 tsize = ResourcesManager::getTextureSizeFor("particles");
	_programParticules.use();
	_programParticules.uniform("inverseTextureSize", 1.0f / tsize);
	glUseProgram(0);

}

void Renderer::upload(const std::shared_ptr<MIDIScene>& scene){
	glm::ivec2 uploadRange;
	if(scene->dirtyNotes(uploadRange)){
		constexpr size_t noteSize = sizeof(MIDIScene::GPUNote);
		const auto& notes = scene->getNotes();
		glBindBuffer(GL_ARRAY_BUFFER, _notesDataBuffer);
		if(uploadRange.y == 0){ // Full vector
			glBufferData(GL_ARRAY_BUFFER, noteSize * notes.size(), &(notes[0]), GL_DYNAMIC_DRAW);
		} else {
			const int size = uploadRange.y - uploadRange.x + 1;
			const int first = uploadRange.x;
			glBufferSubData(GL_ARRAY_BUFFER, first * noteSize, size * noteSize, &(notes[first]));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		scene->setUpToDate();
	}

	// Update the flags buffer accordingly.
	const auto& actives = scene->getActiveKeys();
	glBindBuffer(GL_ARRAY_BUFFER, _keysDataBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, actives.size()*sizeof(int) ,&(actives[0]));

	_programKeyMajors.use();
	_programKeyMajors.uniforms("actives", actives.size(), actives.data());
	_programKeyMinors.use();
	_programKeyMinors.uniforms("actives", actives.size(), actives.data());
	_programKeys3D.use();
	_programKeys3D.uniforms("actives", actives.size(), actives.data());
	glUseProgram(0);
}

void Renderer::setScaleAndMinorWidth(const float scale, const float minorWidth){
	_programNotes.use();
	_programNotes.uniform("mainSpeed", scale);
	_programNotes.uniform("minorsWidth", minorWidth);
	_programKeyMinors.use();
	_programKeyMinors.uniform("minorsWidth", minorWidth);
	_programKeys3D.use();
	_programKeys3D.uniform("minorsWidth", minorWidth);
	_minorWidth = minorWidth;
	glUseProgram(0);
}

void Renderer::setParticlesParameters(const float speed, const float expansion){
	_programParticules.use();
	_programParticules.uniform("speedScaling", speed);
	_programParticules.uniform("expansionFactor", expansion);
	glUseProgram(0);
}

void Renderer::setKeyboardSizeAndFadeout(float keyboardHeight, float fadeOut){
	const float fadeOutFinal = keyboardHeight + (1.0f - keyboardHeight) * (1.0f - fadeOut);

	_programNotes.use();
	_programNotes.uniform("keyboardHeight", keyboardHeight);
	_programNotes.uniform("fadeOut", fadeOutFinal);
	_programParticules.use();
	_programParticules.uniform("keyboardHeight", keyboardHeight);
	_programKeyMinors.use();
	_programKeyMinors.uniform("keyboardHeight", keyboardHeight);
	_programKeyMajors.use();
	_programKeyMajors.uniform("keyboardHeight", keyboardHeight);
	_programKeys3D.use();
	_programKeys3D.uniform("keyboardHeight", keyboardHeight);
	_keyboardHeight = keyboardHeight;
	_programFlashes.use();
	_programFlashes.uniform("keyboardHeight", keyboardHeight);
	glUseProgram(0);
}

void Renderer::setMinorEdgesAndHeight(bool minorEdges, float minorHeight){
	_programKeyMinors.use();
	_programKeyMinors.uniform("edgeOnMinors", minorEdges);
	_programKeyMinors.uniform("minorsHeight", minorHeight);
	glUseProgram(0);
}

void Renderer::setMinMaxKeys(int minKey, int minKeyMajor, int notesCount){
	_programNotes.use();
	_programNotes.uniform("minNoteMajor", minKeyMajor);
	_programNotes.uniform("notesCount", float(notesCount));
	_programFlashes.use();
	_programFlashes.uniform("minNote", minKey);
	_programFlashes.uniform("notesCount", float(notesCount));
	_programKeyMajors.use();
	_programKeyMajors.uniform("minNoteMajor", minKeyMajor);
	_programKeyMajors.uniform("notesCount", float(notesCount));
	_programKeyMinors.use();
	_programKeyMinors.uniform("minNoteMajor", minKeyMajor);
	_programKeyMinors.uniform("notesCount", float(notesCount));
	_programKeys3D.use();
	_programKeys3D.uniform("minNoteMajor", minKeyMajor);
	_programKeys3D.uniform("notesCount", float(notesCount));
	_programParticules.use();
	_programParticules.uniform("minNote", minKey);
	_programParticules.uniform("notesCount", float(notesCount));
	// Refresh score settings (used on CPU)
	_minKeyMajor = minKeyMajor;
	_keyCount = notesCount;
	glUseProgram(0);
}

void Renderer::setOrientation(bool horizontal){
	_programNotes.use();
	_programNotes.uniform("horizontalMode", horizontal);
	_programFlashes.use();
	_programFlashes.uniform("horizontalMode", horizontal);
	_programKeyMajors.use();
	_programKeyMajors.uniform("horizontalMode", horizontal);
	_programKeyMinors.use();
	_programKeyMinors.uniform("horizontalMode", horizontal);
	_programParticules.use();
	_programParticules.uniform("horizontalMode", horizontal);
	_programWave.use();
	_programWave.uniform("horizontalMode", horizontal);
	_programWaveNoise.use();
	_programWaveNoise.uniform("horizontalMode", horizontal);
	_programScoreBars.use();
	_programScoreBars.uniform("horizontalMode", horizontal);
	_programScoreLabels.use();
	_programScoreLabels.uniform("horizontalMode", horizontal);
	glUseProgram(0);
}

void Renderer::drawParticles(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::ParticlesState & state, bool prepass){

	glEnable(GL_BLEND);
	_programParticules.use();
	
	// Common uniforms values.
	_programParticules.uniform("inverseScreenSize", invScreenSize);
	_programParticules.uniform("time", 0.0f);

	// Prepass : bigger, darker particles.
	_programParticules.uniform("colorScale", prepass ? 0.6f : 1.6f);
	_programParticules.uniform("scale", state.scale * (prepass ? 2.0f : 1.0f));

	_programParticules.uniforms("baseColor", state.colors.size(), state.colors.data());
	
	// Particles trajectories texture.
	_programParticules.texture("textureParticles", _texParticles, GL_TEXTURE_2D);
	_programParticules.texture("textureNoise", _texNoise, GL_TEXTURE_2D);
	_programParticules.texture("lookParticles", state.tex, GL_TEXTURE_2D_ARRAY);
	_programParticules.uniform("turbulenceStrength", state.turbulenceStrength);
	_programParticules.uniform("turbulenceScale", state.turbulenceScale);
	_programParticules.uniform("texCount", state.texCount);

	// Select the geometry.
	glBindVertexArray(_vaoQuad);
	// For each active particles system, draw it with the right parameters.
	for(const auto & particle : scene->getParticles()){
		if(particle.note >= 0){
			_programParticules.uniform("globalId", particle.note);
			_programParticules.uniform("time", particle.elapsed);
			_programParticules.uniform("duration", particle.duration);
			_programParticules.uniform("channel", particle.set);
			glDrawElementsInstanced(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0, state.count);
		}
	}
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);

}

void Renderer::drawNotes(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::NotesState & state, bool reverseScroll, bool prepass){
	
	_programNotes.use();
	
	// Uniforms setup.
	const float maxCornerRadius = 0.12f; // Internal tweaking to avoid notes whose edges are not timing-aligned.

	_programNotes.uniform("inverseScreenSize", invScreenSize);
	_programNotes.uniform("time", time);
	_programNotes.uniform("colorScale", prepass ? 0.6f: 1.0f);

	_programNotes.uniforms("baseColor" , state.majorColors.size(), state.majorColors.data());
	_programNotes.uniforms("minorColor", state.minorColors.size(), state.minorColors.data());
	_programNotes.uniform("reverseMode", reverseScroll);
	_programNotes.uniform("edgeWidth", state.edgeWidth);
	_programNotes.uniform("edgeBrightness", state.edgeBrightness);

	_programNotes.uniform("texturesScale", glm::vec2{state.majorTexScale, state.minorTexScale});
	_programNotes.uniform("texturesStrength", glm::vec2{state.majorTexAlpha, state.minorTexAlpha});
	_programNotes.uniform("scrollMajorTexture", state.majorTexScroll);
	_programNotes.uniform("scrollMinorTexture", state.minorTexScroll);
	_programNotes.uniform("cornerRadius", state.cornerRadius * maxCornerRadius);

	// Pass note textures if present, and corresponding flags.
	_programNotes.uniform("useMajorTexture", state.majorTex != 0);
	_programNotes.uniform("useMinorTexture", state.minorTex != 0);
	// IDs below can be 0
	_programNotes.texture("majorTexture", state.majorTex, GL_TEXTURE_2D);
	_programNotes.texture("minorTexture", state.minorTex, GL_TEXTURE_2D);
	
	// Draw the geometry.
	glBindVertexArray(_vaoQuadWithNoteData);
	glDrawElementsInstanced(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0, GLsizei(scene->getEffectiveNotesCount()));

	glBindVertexArray(0);
	glUseProgram(0);
	
}

void Renderer::drawFlashes(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::FlashesState& state){
	
	// Need alpha blending.
	glEnable(GL_BLEND);

	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
	
	_programFlashes.use();
	
	// Uniforms setup.
	_programFlashes.uniform("inverseScreenSize", invScreenSize);
	_programFlashes.uniform("time",time);
	_programFlashes.uniforms("baseColor", state.colors.size(), state.colors.data());
	_programFlashes.uniform("userScale", state.size);
	_programFlashes.uniform("haloInnerRadius", state.haloInnerRadius);
	_programFlashes.uniform("haloOuterRadius", state.haloOuterRadius);
	_programFlashes.uniform("haloIntensity", state.haloIntensity);
	_programFlashes.uniform("texRowCount", state.texRowCount);
	_programFlashes.uniform("texColCount", state.texColCount);
	// Flash texture.
	_programFlashes.texture("textureFlash", state.tex, GL_TEXTURE_2D);
	
	// Draw the geometry.
	glBindVertexArray(_vaoQuadWithKeyData);
	glDrawElementsInstanced(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0, 128);
	
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);

	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

void Renderer::drawKeyboard(const std::shared_ptr<MIDIScene>& scene, float, const glm::vec2 & invScreenSize, const glm::vec3 & edgeColor, const glm::vec3 & keyColor, const ColorArray & majorColors, const ColorArray & minorColors, bool highlightKeys) {

	{
		_programKeyMajors.use();
		// Uniforms setup.
		_programKeyMajors.uniform("inverseScreenSize", invScreenSize);
		_programKeyMajors.uniform("edgeColor", edgeColor);
		_programKeyMajors.uniform("keyColor", keyColor);
		_programKeyMajors.uniforms("majorColor", majorColors.size(), majorColors.data());
		_programKeyMajors.uniform("highlightKeys", highlightKeys);

		// Draw the geometry.
		glBindVertexArray(_vaoQuad);
		glDrawElements(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0);
	}

	{
		_programKeyMinors.use();
		// Uniforms setup.
		_programKeyMinors.uniform("inverseScreenSize", invScreenSize);
		_programKeyMinors.uniform("edgeColor", edgeColor);
		_programKeyMinors.uniforms("minorColor", minorColors.size(), minorColors.data());
		_programKeyMinors.uniform("highlightKeys", highlightKeys);

		glBindVertexArray(_vaoQuad);
		glDrawElementsInstanced(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0, 53);
	}
	
	glBindVertexArray(0);
	glUseProgram(0);
}

bool Renderer::loadMascot(const std::string & path){
	_mascotPath = path;
	++_mascotGeneration;
	glDeleteTextures(1, &_mascotDist);
	glDeleteTextures(1, &_mascotColor);
	_mascotDist = _mascotColor = 0;
	_mascotError.clear();
	if(path.empty()){
		return true;
	}

	// Header with the default settings, then zlib-compressed distances (16 bits, x fastest) followed by colors (rgba 8 bits).
	struct Header {
		char magic[4];
		uint32_t version;
		uint32_t dims[3];
		uint32_t colorDims[3];
		float half[3];
		float center[3];
		float distRange[2];
		float position[3];
		float turn, lean, size, glow, light;
		float lightPosition[3];
		float bulb;
		float color[3];
		uint32_t dataSize;
	};
	std::string fullPath = path;
	if(fullPath.size() > 1 && fullPath[0] == '~' && std::getenv("HOME")){
		fullPath = std::string(std::getenv("HOME")) + fullPath.substr(1);
	}
	std::ifstream file(fullPath, std::ios::binary);
	if(!file.is_open()){
		_mascotError = "Unable to open the file";
		std::cerr << "[MASCOT]: " << path << ": " << _mascotError << std::endl;
		return false;
	}
	const std::vector<unsigned char> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	Header header;
	if(content.size() < sizeof(Header)){
		_mascotError = "Not a mascot file";
		std::cerr << "[MASCOT]: " << path << ": " << _mascotError << std::endl;
		return false;
	}
	std::memcpy(&header, content.data(), sizeof(Header));
	if(std::strncmp(header.magic, "MSCT", 4) != 0 || header.version != 3 || content.size() < sizeof(Header) + header.dataSize){
		_mascotError = "Not a mascot file, or baked for another version (download it again)";
		std::cerr << "[MASCOT]: " << path << ": " << _mascotError << std::endl;
		return false;
	}
	std::vector<unsigned char> data;
	const size_t distCount = size_t(header.dims[0]) * header.dims[1] * header.dims[2];
	const size_t colorCount = size_t(header.colorDims[0]) * header.colorDims[1] * header.colorDims[2];
	if(lodepng::decompress(data, content.data() + sizeof(Header), header.dataSize) != 0 || data.size() != 2 * distCount + 4 * colorCount){
		_mascotError = "The file is damaged";
		std::cerr << "[MASCOT]: " << path << ": " << _mascotError << std::endl;
		return false;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint textures[2];
	glGenTextures(2, textures);
	for(int i = 0; i < 2; ++i){
		const uint32_t* dims = i == 0 ? header.dims : header.colorDims;
		glBindTexture(GL_TEXTURE_3D, textures[i]);
		glTexImage3D(GL_TEXTURE_3D, 0, i == 0 ? GL_R16 : GL_RGBA8, GLsizei(dims[0]), GLsizei(dims[1]), GLsizei(dims[2]), 0,
					 i == 0 ? GL_RED : GL_RGBA, i == 0 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE, data.data() + (i == 0 ? 0 : 2 * distCount));
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	glBindTexture(GL_TEXTURE_3D, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	_mascotDist = textures[0];
	_mascotColor = textures[1];
	_mascotHalf = glm::vec3(header.half[0], header.half[1], header.half[2]);
	_mascotCenter = glm::vec3(header.center[0], header.center[1], header.center[2]);
	_mascotDistRange = glm::vec2(header.distRange[0], header.distRange[1]);
	_mascotDefaults.path.clear();
	_mascotDefaults.position = glm::vec3(header.position[0], header.position[1], header.position[2]);
	_mascotDefaults.turn = header.turn;
	_mascotDefaults.lean = header.lean;
	_mascotDefaults.size = header.size;
	_mascotDefaults.glow = header.glow;
	_mascotDefaults.light = header.light;
	_mascotDefaults.lightPosition = glm::vec3(header.lightPosition[0], header.lightPosition[1], header.lightPosition[2]);
	_mascotDefaults.bulb = header.bulb;
	_mascotDefaults.color = glm::vec3(header.color[0], header.color[1], header.color[2]);
	return true;
}

void Renderer::drawKeyboard3D(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::KeyboardState & state, const State::MascotState & mascot, const ColorArray & majorColors, const ColorArray & minorColors) {

	// Key travel: a damped spring towards pressed or released, stiffer on the way down.
	// The slight overshoot is the key hitting the felt at the bottom, and bouncing back on release.
	// Driven by the scene time, so that exports are deterministic; when paused or seeking, keys snap.
	const auto& actives = scene->getActiveKeys();
	const float dt = time - _keyAnimTime;
	_keyAnimTime = time;
	const bool snap = dt <= 0.0f || dt > 0.25f;
	const int substeps = snap ? 0 : int(std::ceil(dt / 0.001f));
	const float h = substeps > 0 ? dt / float(substeps) : 0.0f;
	for(size_t k = 0; k < actives.size(); ++k){
		const float target = (state.highlightKeys && actives[k] >= 0) ? 1.0f : 0.0f;
		if(snap){
			_keyTravel[k] = target;
			_keyVelocity[k] = 0.0f;
			continue;
		}
		const bool pressing = target > 0.5f;
		// Natural frequency (rad/s) and damping ratio: ~35ms down, ~70ms back up at speed 1.
		const float omega = (pressing ? 75.0f : 40.0f) * state.keySpeed;
		const float zeta = pressing ? 0.55f : 0.5f;
		for(int i = 0; i < substeps; ++i){
			const float acc = omega * omega * (target - _keyTravel[k]) - 2.0f * zeta * omega * _keyVelocity[k];
			_keyVelocity[k] += acc * h;
			_keyTravel[k] += _keyVelocity[k] * h;
		}
		_keyTravel[k] = glm::clamp(_keyTravel[k], -0.2f, 1.2f);
	}

	// Position of a key along the keyboard, laid out exactly as the shader places the keys (in white key
	// widths from the left edge). Black key between white keys i and i+1, shifted like in the flat keyboard.
	std::array<float, 128> keyX;
	std::array<bool, 128> keyVisible;
	{
		int majorIndex = 0; // Index of the next white key among all notes 0..127.
		for(int note = 0; note < 128; ++note){
			const int pc = note % 12;
			const bool isMinor = pc == 1 || pc == 3 || pc == 6 || pc == 8 || pc == 10;
			if(!isMinor){
				const int i = majorIndex - int(_minKeyMajor);
				keyVisible[note] = i >= 0 && i < int(_keyCount);
				keyX[note] = float(i) + 0.5f;
				++majorIndex;
			} else {
				const int i = majorIndex - 1 - int(_minKeyMajor);
				const float shift = (pc == 1 || pc == 6) ? -0.09f : ((pc == 3 || pc == 10) ? 0.09f : 0.0f);
				keyVisible[note] = i >= 0 && i < int(_keyCount) - 1;
				keyX[note] = float(i + 1) + shift * _minorWidth;
			}
		}
	}
	auto isMinorNote = [](int note){ const int pc = note % 12; return pc == 1 || pc == 3 || pc == 6 || pc == 8 || pc == 10; };

	// Lit notes.
	constexpr int maxNoteLights = 24;
	std::array<glm::vec3, maxNoteLights> noteLights;
	std::array<glm::vec3, maxNoteLights> noteLightColors;
	int noteLightCount = 0;
	if(state.highlightKeys){
		for(int note = 0; note < 128 && noteLightCount < maxNoteLights; ++note){
			const int set = actives[note];
			if(set < 0 || !keyVisible[note]){
				continue;
			}
			const bool minor = isMinorNote(note);
			// Just above the key top. In front of the felt for white keys; against the canvas for black keys,
			// which go through the felt: their top behind a light in front of the felt would stay dark.
			noteLights[noteLightCount] = minor ? glm::vec3(keyX[note], 0.142f + 0.467f, 0.03f)
											   : glm::vec3(keyX[note], 0.142f, 0.183f + 0.061f);
			noteLightColors[noteLightCount] = minor ? minorColors[set] : majorColors[set];
			++noteLightCount;
		}
	}

	// Parts of the keyboard that differ from the baked image: around lit notes, as far as their light reaches,
	// and around keys that are not at rest (their neighbours' walls and shadows change too).
	constexpr int maxRanges = 48;
	std::vector<glm::vec2> ranges;
	const float reach = 4.0f * state.noteLightRadius; // Same as in the shader.
	for(int i = 0; i < noteLightCount; ++i){
		ranges.emplace_back(noteLights[i].x - reach, noteLights[i].x + reach);
	}
	for(int note = 0; note < 128; ++note){
		if(keyVisible[note] && std::abs(_keyTravel[note]) > 1e-3f){
			ranges.emplace_back(keyX[note] - 2.0f, keyX[note] + 2.0f);
		}
	}
	std::sort(ranges.begin(), ranges.end(), [](const glm::vec2& a, const glm::vec2& b){ return a.x < b.x; });
	std::vector<glm::vec2> merged;
	for(const glm::vec2& r : ranges){
		if(!merged.empty() && r.x <= merged.back().y){
			merged.back().y = (std::max)(merged.back().y, r.y);
		} else {
			merged.push_back(r);
		}
	}
	if(merged.size() > size_t(maxRanges)){
		merged = { glm::vec2(-1e6f, 1e6f) };
	}

	// Offscreen buffers must be cleared to transparent: pixels the keys do not cover show what is behind.
	// (Framebuffer::resize clears with the current clear color, which is the opaque background color.)
	auto clearTransparent = [](){
		GLfloat previous[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, previous);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(previous[0], previous[1], previous[2], previous[3]);
	};

	// Render size of the keyboard: lower than the frame to spare the GPU, then scaled up.
	const float resolution = glm::clamp(state.resolution, 0.25f, 1.0f);
	const int frameWidth = int(std::round(1.0f / invScreenSize.x));
	const int frameHeight = int(std::round(1.0f / invScreenSize.y));
	const int width = (std::max)(1, int(std::round(float(frameWidth) * resolution)));
	const int height = (std::max)(1, int(std::round(float(frameHeight) * resolution)));
	const bool scaled = width != frameWidth || height != frameHeight;

	_programKeys3D.use();
	_programKeys3D.uniform("inverseScreenSize", glm::vec2(1.0f / float(width), 1.0f / float(height)));
	_programKeys3D.uniform("edgeColor", state.edgeColor);
	_programKeys3D.uniform("keyColor", state.backColor);
	_programKeys3D.uniforms("majorColor", majorColors.size(), majorColors.data());
	_programKeys3D.uniforms("minorColor", minorColors.size(), minorColors.data());
	_programKeys3D.uniform("highlightKeys", state.highlightKeys);
	_programKeys3D.uniform("tilt", state.tilt);
	_programKeys3D.uniform("perspective", state.perspective);
	_programKeys3D.uniform("fillLight", state.fillLight);
	_programKeys3D.uniform("ambientLight", state.ambientLight);
	_programKeys3D.uniform("noteLight", state.noteLight);
	_programKeys3D.uniform("noteLightRadius", state.noteLightRadius);
	_programKeys3D.uniform("keyDip", state.dip);
	_programKeys3D.uniform("feltColor", state.feltColor);
	_programKeys3D.uniform("backLight", state.backLight);
	_programKeys3D.uniform("backLightRadius", state.backLightRadius);
	_programKeys3D.uniform("backLightColor", state.backLightColor);
	_programKeys3D.uniform("backLightPosition", state.backLightPosition);
	_programKeys3D.uniform("backLightHeight", state.backLightHeight);

	// Mascot. Its file path is split at spaces by the settings parser.
	std::string mascotPath;
	for(const std::string& part : mascot.path){
		mascotPath += (mascotPath.empty() ? "" : " ") + part;
	}
	if(mascotPath != _mascotPath){
		loadMascot(mascotPath);
	}
	const bool mascotOn = _mascotDist != 0;
	// Model to scene: scaled, turned counterclockwise seen from above, then leaning towards the keys (+x).
	const float mascotSize = (std::max)(mascot.size, 0.01f);
	const glm::mat3 mascotTurn(glm::rotate(glm::mat4(1.0f), glm::radians(-mascot.turn), glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::mat3 mascotLean(glm::rotate(glm::mat4(1.0f), glm::radians(-mascot.lean), glm::vec3(0.0f, 0.0f, 1.0f)));
	const glm::mat3 mascotToScene = mascotLean * mascotTurn * mascotSize;
	const glm::vec3 mascotLightOffset = mascotToScene * mascot.lightPosition;
	// Highest point it can reach above its base: rays start above it.
	float mascotTop = mascotSize * (glm::length(_mascotCenter) + glm::length(_mascotHalf));
	if(mascot.bulb > 0.0f){
		mascotTop = (std::max)(mascotTop, mascotLightOffset.y + mascot.bulb);
	}
	mascotTop += mascot.position.y;
	_programKeys3D.uniform("mascotEnabled", mascotOn);
	_programKeys3D.uniform("mascotHalf", _mascotHalf);
	_programKeys3D.uniform("mascotCenter", _mascotCenter);
	_programKeys3D.uniform("mascotDistRange", _mascotDistRange);
	_programKeys3D.uniform("mascotOffset", mascot.position);
	_programKeys3D.uniform("mascotToModel", glm::inverse(mascotToScene));
	_programKeys3D.uniform("mascotSize", mascotSize);
	_programKeys3D.uniform("mascotTop", mascotTop);
	_programKeys3D.uniform("mascotGlow", mascot.glow);
	_programKeys3D.uniform("mascotLight", mascot.light);
	_programKeys3D.uniform("mascotLightOffset", mascotLightOffset);
	_programKeys3D.uniform("mascotBulb", mascot.bulb);
	_programKeys3D.uniform("mascotColor", mascot.color);
	_programKeys3D.texture("mascotDist", _mascotDist, GL_TEXTURE_3D);
	_programKeys3D.texture("mascotColors", _mascotColor, GL_TEXTURE_3D);
	// The quad covers what rises above the keyboard line: black keys and the felt, and the mascot
	// (its height plus the cheek block, with a margin for the perspective).
	_programKeys3D.uniform("topMargin", mascotOn ? (std::max)(2.0f, 1.3f * (mascotTop + 0.9f) + 1.0f) : 2.0f);

	GLint frameFramebuffer = 0;
	GLint frameViewport[4];
	if(scaled){
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frameFramebuffer);
		glGetIntegerv(GL_VIEWPORT, frameViewport);
		if(!_keys3DLow){
			_keys3DLow = std::make_shared<Framebuffer>(width, height, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE);
		}
		_keys3DLow->resize(width, height);
		_keys3DLow->bind();
		glViewport(0, 0, width, height);
		clearTransparent();
	}

	// Bake the keyboard at rest when anything it depends on has changed (settings, layout, render size).
	const std::vector<float> signature = {
		float(width), float(height), _keyboardHeight, float(_keyCount), float(_minKeyMajor), _minorWidth,
		state.tilt, state.perspective, state.fillLight, state.ambientLight,
		state.backLight, state.backLightRadius, state.backLightPosition, state.backLightHeight,
		state.backLightColor.r, state.backLightColor.g, state.backLightColor.b,
		state.feltColor.r, state.feltColor.g, state.feltColor.b,
		state.edgeColor.r, state.edgeColor.g, state.edgeColor.b,
		state.backColor.r, state.backColor.g, state.backColor.b,
		mascotOn ? float(_mascotGeneration) : -1.0f, mascot.position.x, mascot.position.y, mascot.position.z,
		mascot.turn, mascot.lean, mascot.size, mascot.glow, mascot.light,
		mascot.lightPosition.x, mascot.lightPosition.y, mascot.lightPosition.z, mascot.bulb,
		mascot.color.r, mascot.color.g, mascot.color.b };
	if(!_keys3DBake || signature != _keys3DBakeSignature){
		_keys3DBakeSignature = signature;
		GLint previousFramebuffer = 0;
		GLint previousViewport[4];
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);
		glGetIntegerv(GL_VIEWPORT, previousViewport);
		if(!_keys3DBake){
			_keys3DBake = std::make_shared<Framebuffer>(width, height, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
		}
		_keys3DBake->resize(width, height);
		_keys3DBake->bind();
		glViewport(0, 0, width, height);
		clearTransparent();
		_programKeys3D.use();
		static const std::array<float, 128> atRest{};
		_programKeys3D.uniforms("keyTravel", atRest.size(), atRest.data());
		_programKeys3D.uniform("noteLightCount", 0);
		_programKeys3D.uniform("useBake", false);
		glBindVertexArray(_vaoQuad);
		glDrawElements(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0);
		glBindFramebuffer(GL_FRAMEBUFFER, GLuint(previousFramebuffer));
		glViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
	}

	_programKeys3D.use();
	_programKeys3D.uniforms("keyTravel", _keyTravel.size(), _keyTravel.data());
	_programKeys3D.uniform("noteLightCount", noteLightCount);
	if(noteLightCount > 0){
		_programKeys3D.uniforms("noteLights", size_t(noteLightCount), noteLights.data());
		_programKeys3D.uniforms("noteLightColors", size_t(noteLightCount), noteLightColors.data());
	}
	_programKeys3D.uniform("useBake", true);
	_programKeys3D.texture("bakeTex", _keys3DBake->textureId(), GL_TEXTURE_2D);
	_programKeys3D.uniform("dynamicRangeCount", int(merged.size()));
	if(!merged.empty()){
		_programKeys3D.uniforms("dynamicRanges", merged.size(), merged.data());
	}

	// Black keys and the felt strip rise above the keyboard line: the quad extends above it,
	// pixels where rays miss the keys are discarded so the notes stay visible there.
	glBindVertexArray(_vaoQuad);
	glDrawElements(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	glUseProgram(0);

	if(scaled){
		// Scale up onto the frame. Colors are either opaque or fully transparent black, so they are
		// premultiplied already: filtered edges blend correctly over the notes.
		glBindFramebuffer(GL_FRAMEBUFFER, GLuint(frameFramebuffer));
		glViewport(frameViewport[0], frameViewport[1], frameViewport[2], frameViewport[3]);
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		_keys3DUpscale.draw(_keys3DLow->textureId(), 0.0f);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		glDisable(GL_BLEND);
	}
}

void Renderer::drawPedals(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::PedalsState & state, float keyboardHeight, bool horizontalMode) {

	glEnable(GL_BLEND);
	_programPedals.use();

	// Adjust for aspect ratio.
	const float rat = invScreenSize.y/invScreenSize.x;
	const glm::vec2 ratioScale = (rat < 1.0f) ? glm::vec2(1.0f, rat) : glm::vec2(1.0f/rat, 1.0f);
	const glm::vec2 globalScale = 2.0f /* quad is -0.5,0.5 */ * state.size * ratioScale * glm::vec2(1.2f, 1.0f)  /* existing pedal ratio correction */;
	const State::PedalsState::Location mode = state.location;
	const float vertSign  = (mode == State::PedalsState::TOPLEFT || mode == State::PedalsState::TOPRIGHT) ? 1.0f : -1.0f;
	const float horizSign = (mode == State::PedalsState::TOPLEFT || mode == State::PedalsState::BOTTOMLEFT) ? -1.0f : 1.0f;
	const float safetyMargin = 0.02f;
	glm::vec2 globalShift = glm::vec2(horizSign, vertSign) * (1.0f - 0.5f * globalScale - safetyMargin * ratioScale);

	// If at the bottom, shift above the keyboard.
	if(horizontalMode){
		if(mode == State::PedalsState::TOPLEFT || mode == State::PedalsState::BOTTOMLEFT){
			globalShift[0] += 2.0f * keyboardHeight;
		}
	} else {
		if(mode == State::PedalsState::BOTTOMLEFT || mode == State::PedalsState::BOTTOMRIGHT){
			globalShift[1] += 2.0f * keyboardHeight;
		}
	}

	const float tightenShiftX = state.margin.x;
	const float tightenShiftY = state.margin.y;
	const GLuint textures[4] = { state.texSides[0], state.texCenter, state.texSides[1], state.texTop };


	// (1)soft, (2)sostenuto, (3)damper, (4)expression
	//   *-----------------^-----*
	//   |          (4)    | 20% |
	//   |-----------------v-----|
	//   |  (1)  |  (2)  |  (3)  |
	//   |       |   x   |       |
	//   |       |       |       |
	//   |<-35%->|<-30%->|<-35%->|
	//   |       |       |       |
	//   *-----------------------*

	const float expressionHeight = 0.20f;
	const float expressionWidth = 1.0f;
	const float sidesWidth = 0.35f;
	const float sidesHeight = 1.0f - expressionHeight;
	const float centralWidth = 1.0f - 2.0f * sidesWidth;
	const float centralHeight = sidesHeight;
	const float expressionShiftX = 0.0f;
	const float expressionShiftY = 0.5f * (1.0f - expressionHeight ) - tightenShiftY;
	const float sidesShiftX = 0.5f * (sidesWidth + centralWidth) - tightenShiftX;
	const float sidesShiftY = 0.5f * (sidesHeight-1.0f);
	const float centralShiftX = 0.0f;
	const float centralShiftY = 0.5f * (sidesHeight-1.0f);

	const glm::vec2 localScales[4] = {
		glm::vec2(sidesWidth, sidesHeight), glm::vec2(centralWidth, 	centralHeight),
		glm::vec2(sidesWidth, sidesHeight), glm::vec2(expressionWidth,	expressionHeight)
	};
	// By default all quads are centered at 0.5,0.5 in local space, we need to shift them post-scaling.
	const glm::vec2 localShifts[4] = {
		glm::vec2(-sidesShiftX, sidesShiftY), glm::vec2(centralShiftX,		centralShiftY),
		glm::vec2( sidesShiftX, sidesShiftY), glm::vec2(expressionShiftX,	expressionShiftY)
	};

	const MIDIScene::Pedals& pedals = scene->getPedals();
	const float actives[4] = { pedals.soft, pedals.sostenuto, pedals.damper, pedals.expression };
	const glm::vec3* colors[4] = { &state.leftColor, &state.centerColor, &state.rightColor, &state.topColor};
	// Uniforms setup.
	_programPedals.uniform("pedalOpacity", state.opacity);

	glBindVertexArray(_vaoQuadWithNoteData);
	if(state.merge){
		const float active = (glm::max)((glm::max)(actives[0], actives[1]), (glm::max)(actives[2], actives[3]));
		// We want to move the central pedal to the side
		const glm::vec2 localShift = glm::vec2(horizSign, vertSign) * glm::vec2(sidesShiftX, 0.5f * expressionHeight);

		_programPedals.uniform("scale", globalScale * localScales[1]);
		_programPedals.uniform("shift", globalShift + globalScale * localShift);
		_programPedals.uniform("pedalFlag", active);
		_programPedals.uniform("mirror", false);
		_programPedals.texture("pedalTexture", textures[1], GL_TEXTURE_2D);
		_programPedals.uniform("pedalColor", *colors[1]);

		glDrawElements(GL_TRIANGLES, int(_vaoQuadWithKeyData), GL_UNSIGNED_INT, (void*)0);
	} else {
		for(unsigned int i = 0; i < 4; ++i){
			_programPedals.uniform("scale", globalScale * localScales[i]);
			_programPedals.uniform("shift", globalShift + globalScale * localShifts[i]);
			_programPedals.uniform("pedalFlag", actives[i]);
			_programPedals.texture("pedalTexture", textures[i], GL_TEXTURE_2D);
			_programPedals.uniform("mirror", state.mirror && (i==2));
			_programPedals.uniform("pedalColor", *colors[i]);
			glDrawElements(GL_TRIANGLES, int(_vaoQuadWithKeyData), GL_UNSIGNED_INT, (void*)0);
		}
	}

	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
}

void Renderer::drawWaves(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::WaveState & state, float keyboardHeight) {

	_programWave.use();
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

	// Uniforms setup.
	_programWave.uniform("waveColor", state.color);
	_programWave.uniform("keyboardSize", keyboardHeight);
	_programWave.uniform("waveOpacity", state.opacity);
	_programWave.uniform("spread", state.spread);
	glBindVertexArray(_vaoWave);

	// Fixed initial parameters.
	const float ampls[4] = {-0.023f, -0.011f, 0.017f, 0.009f};
	const float freqs[4] = {10.3f, -8.27f, -4.4f, 13.7f};
	const float phases[4] = {5.2f, 4.7f, 9.3f, -7.1f};
	// Render multiple waves with additive blending.
	for(int i = 0; i < 4; ++i){
		const float ampl = state.amplitude * ampls[i];
		const float freq = state.frequency * freqs[i];
		const float phase = phases[i] * time * state.speed + float(i+1) * 7.39f;
		_programWave.uniform("amplitude", ampl);
		_programWave.uniform("freq", freq);
		_programWave.uniform("phase", phase);
		glDrawElements(GL_TRIANGLES, int(_wavePrimitiveCount), GL_UNSIGNED_INT, (void*)0);
	}

	glBindVertexArray(0);
	glUseProgram(0);

	_programWaveNoise.use();
	_programWaveNoise.uniform("keyboardSize", keyboardHeight);
	_programWaveNoise.uniform("scale", state.noiseSize * 0.5f);

	_programWaveNoise.texture("textureNoise", _texNoise, GL_TEXTURE_2D);
	_programWaveNoise.uniform("offset", time * state.speed * 0.05f);
	_programWaveNoise.uniform("waveColor", state.color);
	_programWaveNoise.uniform("noiseScale", state.frequency);
	_programWaveNoise.uniform("waveOpacity", state.noiseIntensity);
	_programWaveNoise.uniform("inverseScreenSize", invScreenSize);

	glBindVertexArray(_vaoQuadWithKeyData);
	glDrawElements(GL_TRIANGLES, int(_vaoQuadWithKeyData), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glDisable(GL_BLEND);
}

void Renderer::drawScore(const std::shared_ptr<MIDIScene>& scene, float time, const glm::vec2 & invScreenSize, const State::ScoreState & state, float measureScale, float qualityScale, float keyboardHeight, bool horizontalMode, bool reverseScroll){

	const glm::vec2 pixelSize = qualityScale * (horizontalMode ? glm::vec2(invScreenSize.y, invScreenSize.x) : invScreenSize);

	// Draw vertical lines.
	if(state.vLines){
		// 7 major keys.
		const int firstBar = (_minKeyMajor + 6) / 7;
		const int barCount = _keyCount / 7 + 1;

		// minKeyMajor maps to -1
		// _minKeyMajor + _keyCount maps to 1
		float firstBarCoord = (firstBar * 7 - _minKeyMajor) / (float)_keyCount;
		firstBarCoord = 2.0f * firstBarCoord - 1.0f;
		float nextBarDeltaCoord = 7.0f / (float)_keyCount;
		nextBarDeltaCoord *= 2.0f;

		_programScoreBars.use();
		_programScoreBars.uniform("baseOffset", glm::vec2(firstBarCoord, 0.0f));
		_programScoreBars.uniform("nextOffset", glm::vec2(nextBarDeltaCoord, 0.0f));
		_programScoreBars.uniform("scale", glm::vec2(state.vLinesWidth * pixelSize.x, 1.0f));
		_programScoreBars.uniform("color", state.vLinesColor);
		glBindVertexArray(_vaoQuad);
		glDrawElementsInstanced(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0, barCount);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	if(state.hLines || state.digits){

		const float currentAbscisse = time/float(scene->secondsPerMeasure());

		// Find the measure at the bottom of the screen if forward, top of the screen if bottom
		// TODO: (MV) explain the magic values.
		const float magigValue0 = 2.f; // Maybe UV -> NDC ?
		const float magicValue1 = measureScale * float(scene->secondsPerMeasure()); // Units don't make sense...

		const float distanceToScreenEdge = magigValue0 * (reverseScroll ? (1.0f - keyboardHeight) : (keyboardHeight) );

		float firstMesureAbscisse = currentAbscisse - distanceToScreenEdge / measureScale;
		int firstMeasure = int(std::floor(firstMesureAbscisse));
		const float firstMeasureTime = float(scene->secondsPerMeasure()) * float(firstMeasure);

		const float direction = (reverseScroll ? -1.0f : 1.0f);
		const float keyboardPos = 2.0f * keyboardHeight - 1.0f;

		const float nextBarDeltaCoord = direction * magicValue1;

		float firstBarCoord = direction * (firstMeasureTime - time) * measureScale + keyboardPos;
		
		// Corrective step.
		// while the next bar is not on screen, keep stepping forwards.
		while( direction * ( firstBarCoord + nextBarDeltaCoord ) < -1.f )
		{
			firstBarCoord += nextBarDeltaCoord;
			++firstMeasure;
		}
		// while the first bar is on screen, keep stepping backwards.
		while( direction * ( firstBarCoord ) > -1.f )
		{
			firstBarCoord -= nextBarDeltaCoord;
			--firstMeasure;
		}

		const int barCount = std::ceil( std::abs( 2.f / nextBarDeltaCoord ) ) + 2; // Two extra bars because of corrective stepping.

		// Draw horizontal lines
		if(state.hLines){
			_programScoreBars.use();
			_programScoreBars.uniform("baseOffset", glm::vec2(0.0f, firstBarCoord));
			_programScoreBars.uniform("nextOffset", glm::vec2(0.0f, nextBarDeltaCoord));
			_programScoreBars.uniform("scale", glm::vec2(1.0f, state.hLinesWidth * pixelSize.y));
			_programScoreBars.uniform("color", state.hLinesColor);
			glBindVertexArray(_vaoQuad);
			glDrawElementsInstanced(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0, barCount);
			glBindVertexArray(0);
			glUseProgram(0);
		}

		if(state.digits){
			const float textScale = state.digitsScale;
			// Based on texture size.
			const glm::vec2 digitResolution = glm::vec2(200.0f, 256.0f);
			const glm::vec2 digitSize = textScale * qualityScale * invScreenSize * digitResolution;
			// Ensure at least one measure.
			const float maxMeasureCount = float(scene->duration() / scene->secondsPerMeasure()) + 1;
			// Add one extra digit so that postroll measures are displayed entirely in all cases.
			const float digitCount = (std::floor(std::log10((std::max)(1.f, maxMeasureCount))) + 1) + 1;

			const glm::vec2 offset = 2.0f * digitSize * state.digitsOffset;
			const glm::vec2 margin = horizontalMode ? glm::vec2(offset.y, offset.x) : offset;

			_programScoreLabels.use();
			_programScoreLabels.uniform("baseOffset", glm::vec2(-1.0f, firstBarCoord) + margin);
			_programScoreLabels.uniform("nextOffset", glm::vec2(0.0f, nextBarDeltaCoord));
			_programScoreLabels.uniform("scale", digitSize);
			_programScoreLabels.uniform("color", state.digitsColor);
			_programScoreLabels.uniform("maxDigitCount", int(digitCount));
			_programScoreLabels.uniform("firstMeasure", firstMeasure);
			_programScoreLabels.texture("font", _texFont, GL_TEXTURE_2D);
			glBindVertexArray(_vaoQuad);
			glDrawElementsInstanced(GL_TRIANGLES, int(_quadPrimitiveCount), GL_UNSIGNED_INT, (void*)0, barCount);
			glBindVertexArray(0);
			glUseProgram(0);
		}
	}
}

void Renderer::clean(){
	GLuint vaos[] = {_vaoQuad, _vaoQuadWithNoteData, _vaoQuadWithKeyData, _vaoWave};
	GLuint buffers[] = {_quadVertices, _quadIndices, _waveVertices, _waveIndices, _notesDataBuffer, _keysDataBuffer };
	glDeleteVertexArrays(sizeof(vaos)/sizeof(vaos[0]), vaos);
	glDeleteBuffers(sizeof(buffers)/sizeof(buffers[0]), buffers);

	_programNotes.clean();
	_programFlashes.clean();
	_programParticules.clean();
	_programKeyMinors.clean();
	_programKeyMajors.clean();
	_programKeys3D.clean();
	_keys3DBake.reset();
	_keys3DLow.reset();
	_keys3DUpscale.clean();
	loadMascot("");
	_programPedals.clean();
	_programWave.clean();
	_programWaveNoise.clean();
	_programScoreBars.clean();
	_programScoreLabels.clean();
}
