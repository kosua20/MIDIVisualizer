#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"
#include <cstring>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <iostream>
#include <nfd.h>
#include <stdio.h>
#include <vector>

#include "Renderer.h"
#include <algorithm>
#include <fstream>



SystemAction::SystemAction(SystemAction::Type act) {
	type = act;
	data = glm::ivec4(0);
}

Renderer::Renderer(int winW, int winH, bool fullscreen) {
	_showGUI = true;
	_showDebug = false;

	_fullscreen = fullscreen;
	_windowSize = glm::ivec2(winW, winH);

	// GL options
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	_camera.screen(winW, winH, 1.0f);
	// Setup framebuffers, size does not really matter as we expect.
	const glm::ivec2 renderSize = _camera.renderSize();
	_particlesFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
		GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_blurFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
		GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_finalFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
		GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));

	_backgroundTexture.init("backgroundtexture_frag", "backgroundtexture_vert");
	_blurringScreen.init(_particlesFramebuffer->textureId(), "particlesblur_frag");
	_passthrough.init("screenquad_frag");

	// Create the layers.
	//_layers[Layer::BGCOLOR].type = Layer::BGCOLOR;
	//_layers[Layer::BGCOLOR].name = "Background color";
	//_layers[Layer::BGCOLOR].toggle = &_state.showBackground;

	_layers[Layer::BGTEXTURE].type = Layer::BGTEXTURE;
	_layers[Layer::BGTEXTURE].name = "Background image";
	_layers[Layer::BGTEXTURE].draw = &Renderer::drawBackgroundImage;

	_layers[Layer::BLUR].type = Layer::BLUR;
	_layers[Layer::BLUR].name = "Blur effect";
	_layers[Layer::BLUR].draw = &Renderer::drawBlur;

	_layers[Layer::ANNOTATIONS].type = Layer::ANNOTATIONS;
	_layers[Layer::ANNOTATIONS].name = "Score";
	_layers[Layer::ANNOTATIONS].draw = &Renderer::drawScore;

	_layers[Layer::KEYBOARD].type = Layer::KEYBOARD;
	_layers[Layer::KEYBOARD].name = "Keyboard";
	_layers[Layer::KEYBOARD].draw = &Renderer::drawKeyboard;

	_layers[Layer::PARTICLES].type = Layer::PARTICLES;
	_layers[Layer::PARTICLES].name = "Particles";
	_layers[Layer::PARTICLES].draw = &Renderer::drawParticles;

	_layers[Layer::NOTES].type = Layer::NOTES;
	_layers[Layer::NOTES].name = "Notes";
	_layers[Layer::NOTES].draw = &Renderer::drawNotes;

	_layers[Layer::FLASHES].type = Layer::FLASHES;
	_layers[Layer::FLASHES].name = "Flashes";
	_layers[Layer::FLASHES].draw = &Renderer::drawFlashes;

	// Register state.
	_layers[Layer::BGTEXTURE].toggle = &_state.background.image;
	_layers[Layer::BLUR].toggle = &_state.showBlur;
	_layers[Layer::ANNOTATIONS].toggle = &_state.showScore;
	_layers[Layer::KEYBOARD].toggle = &_state.showKeyboard;
	_layers[Layer::PARTICLES].toggle = &_state.showParticles;
	_layers[Layer::NOTES].toggle = &_state.showNotes;
	_layers[Layer::FLASHES].toggle = &_state.showFlashes;

	// Check setup errors.
	checkGLError();

	_score.reset(new Score(2.0f));
	_scene.reset(new MIDIScene());
}

Renderer::~Renderer() {}

bool Renderer::loadFile(const std::string &midiFilePath) {
	std::shared_ptr<MIDIScene> scene(nullptr);

	try {
		scene = std::make_shared<MIDIScene>(midiFilePath, _state.setOptions);
	} catch(...){
		// Failed to load.
		return false;
	}
	// Player.
	_timer = -_state.prerollTime;
	_shouldPlay = false;

	// Init objects.
	_scene = scene;
	_score = std::make_shared<Score>(_scene->midiFile().secondsPerMeasure());
	applyAllSettings();
	return true;
}

SystemAction Renderer::draw(float currentTime) {

	if(_recorder.isRecording()){
		_timer = _recorder.currentTime();

		drawScene(_recorder.isTransparent());

		_recorder.record(_finalFramebuffer);
		_recorder.drawProgress();

		// Determine which system action to take.
		SystemAction action = SystemAction::NONE;
		// Look at the frame ID.
		if(_recorder.currentFrame() < 2){
			action = SystemAction::FIX_SIZE;
		} else if(_recorder.currentFrame() >= _recorder.framesCount()){
			action = _exitAfterRecording ? SystemAction::QUIT : SystemAction::FREE_SIZE;
			_timer = 0.0f;
			_timerStart = 0.0f;
			_shouldPlay = false;
			updateSizes();
		}
		// Make sure the backbuffer is updated, this is nicer.
		glViewport(0, 0, GLsizei(_camera.screenSize()[0]), GLsizei(_camera.screenSize()[1]));
		_passthrough.draw(_finalFramebuffer->textureId(), _timer);
		return action;
	}

	// -- Default mode --

	// Compute the time elapsed since last frame, or keep the same value if
	// playback is disabled.
	_timer = _shouldPlay ? (currentTime - _timerStart) : _timer;

	// Render scene and blit, with GUI on top if needed.
	drawScene(false);

	glViewport(0, 0, GLsizei(_camera.screenSize()[0]), GLsizei(_camera.screenSize()[1]));
	_passthrough.draw(_finalFramebuffer->textureId(), _timer);

	SystemAction action = SystemAction::NONE;
	if (_showGUI) {
		action = drawGUI(currentTime);
	}

	return action;
}

void Renderer::drawScene(bool transparentBG){

	// Update active notes listing (for particles).
	_scene->updatesActiveNotes(_timer);

	const glm::vec2 invSizeFb = 1.0f / glm::vec2(_finalFramebuffer->_width, _finalFramebuffer->_height);

	// Blur rendering.
	if (_state.showBlur) {
		blurPrepass();
	}

	// Set viewport
	_finalFramebuffer->bind();
	glViewport(0, 0, _finalFramebuffer->_width, _finalFramebuffer->_height);

	// Final pass (directly on screen).
	// Background color.
	if(transparentBG){
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	} else {
		glClearColor(_state.background.color[0], _state.background.color[1], _state.background.color[2], 1.0f);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the layers in order.
	for (int i = 0; i < _state.layersMap.size(); ++i) {
		const int layerId = _state.layersMap[i];
		if (layerId >= _layers.size()) {
			continue;
		}
		if (_layers[layerId].draw && *(_layers[layerId].toggle)) {
			(this->*_layers[layerId].draw)(invSizeFb);
		}
	}

	_finalFramebuffer->unbind();
}

void Renderer::blurPrepass() {
	const glm::vec2 invSizeB = 1.0f / glm::vec2(_particlesFramebuffer->_width, _particlesFramebuffer->_height);
	// Bind particles buffer.
	_particlesFramebuffer->bind();
	// Set viewport.
	glViewport(0, 0, _particlesFramebuffer->_width, _particlesFramebuffer->_height);
	// Draw blurred particles from previous frames.
	_passthrough.draw(_blurFramebuffer->textureId(), _timer);
	if (_state.showParticles) {
		// Draw the new particles.
		_scene->drawParticles(_timer, invSizeB, _state.particles, true);
	}
	if (_state.showBlurNotes) {
		// Draw the notes.
		_scene->drawNotes(_timer, invSizeB, _state.baseColors, _state.minorColors, true);
	}

	_particlesFramebuffer->unbind();

	// Bind blur framebuffer.
	_blurFramebuffer->bind();
	glViewport(0, 0, _blurFramebuffer->_width, _blurFramebuffer->_height);
	// Perform box blur on result from particles pass.
	_blurringScreen.draw(_timer);
	_blurFramebuffer->unbind();

}

void Renderer::drawBackgroundImage(const glm::vec2 &) {
	// Use background.tex and background.imageAlpha
	// Early exit if no texture or transparent.
	if(_state.background.tex == 0 || _state.background.imageAlpha < 1.0f/255.0f) {
		return;
	}
	glEnable(GL_BLEND);
	glUseProgram(_backgroundTexture.programId());
	glUniform1f(glGetUniformLocation(_backgroundTexture.programId(), "textureAlpha"), _state.background.imageAlpha);
	glUniform1i(glGetUniformLocation(_backgroundTexture.programId(), "behindKeyboard"), _state.background.imageBehindKeyboard);
	_backgroundTexture.draw(_state.background.tex, _timer);
	glDisable(GL_BLEND);
}

void Renderer::drawBlur(const glm::vec2 &) {
	glEnable(GL_BLEND);
	_passthrough.draw(_blurFramebuffer->textureId(), _timer);
	glDisable(GL_BLEND);
}

void Renderer::drawParticles(const glm::vec2 & invSize) {
	_scene->drawParticles(_timer, invSize, _state.particles, false);
}

void Renderer::drawScore(const glm::vec2 & invSize) {
	_score->draw(_timer, invSize);
}

void Renderer::drawKeyboard(const glm::vec2 & invSize) {
	const ColorArray & majColors = _state.keyboard.customKeyColors ? _state.keyboard.majorColor : _state.baseColors;
	const ColorArray & minColors = _state.keyboard.customKeyColors ? _state.keyboard.minorColor : _state.minorColors;
	_scene->drawKeyboard(_timer, invSize, _state.background.keysColor, majColors, minColors, _state.keyboard.highlightKeys);
}

void Renderer::drawNotes(const glm::vec2 & invSize) {
	_scene->drawNotes(_timer, invSize, _state.baseColors, _state.minorColors, false);
}

void Renderer::drawFlashes(const glm::vec2 & invSize) {
	_scene->drawFlashes(_timer, invSize, _state.flashColors, _state.flashSize);
}

SystemAction Renderer::drawGUI(const float currentTime) {

	SystemAction action = SystemAction::NONE;

	if (ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::Button(_shouldPlay ? "Pause (p)" : "Play (p)")) {
			_shouldPlay = !_shouldPlay;
			_timerStart = currentTime - _timer;
		}
		ImGui::SameLine();
		if (ImGui::Button("Restart (r)")) {
			reset();
		}
		ImGui::SameLine();
		if (ImGui::Button("Hide (i)")) {
			_showGUI = false;
		}
		ImGui::SameLine();
		if(ImGui::Button("Display")){
			ImGui::OpenPopup("Display options");

		}
		if(ImGui::BeginPopup("Display options")){
			if(ImGui::Checkbox("Fullscreen", &_fullscreen)){
				action = SystemAction::FULLSCREEN;
			}
			if(!_fullscreen){
				ImGui::PushItemWidth(100);
				ImGui::InputInt2("Window size", &_windowSize[0]);
				ImGui::PopItemWidth();
				ImGui::SameLine();
				if(ImGui::Button("Resize")){
					action = SystemAction::RESIZE;
					action.data[0] = _windowSize[0];
					action.data[1] = _windowSize[1];
				}
			}
			ImGui::EndPopup();
		}

		ImGui::SameLine(340);
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			const std::string versionString = std::string("MIDIVisualizer v") + std::to_string(MIDIVIZ_VERSION_MAJOR) + "." + std::to_string(MIDIVIZ_VERSION_MINOR);
			ImGui::TextUnformatted(versionString.c_str());
			ImGui::TextUnformatted("Created by S. Rodriguez (kosua20)");
			ImGui::TextUnformatted("github.com/kosua20/MIDIVisualizer");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}


		ImGui::Separator();

		if (ImGui::Button("Load MIDI file...")) {
			// Read arguments.
			nfdchar_t *outPath = NULL;
			nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
			if (result == NFD_OKAY) {
				loadFile(std::string(outPath));
			}
		}
		ImGui::SameLine(COLUMN_SIZE);
		ImGui::PushItemWidth(100);
		if (ImGui::InputFloat("Preroll", &_state.prerollTime, 0.1f, 1.0f)) {
			reset();
		}
		ImGui::PopItemWidth();

		if (ImGui::Button("Show layers...")) {
			_showLayers = true;
		}
		ImGui::SameLine(COLUMN_SIZE);
		ImGui::PushItemWidth(100);
		if (ImGui::Combo("Quality", (int *)(&_state.quality), "Half\0Low\0Medium\0High\0Double\0\0")) {
			updateSizes();
		}

		const bool smw0 = ImGui::InputFloat("Scale", &_state.scale, 0.01f, 0.1f);
		ImGui::SameLine(COLUMN_SIZE);
		bool smw1 = ImGui::SliderFloat("Minor size", &_state.background.minorsWidth, 0.1f, 1.0f, "%.2f");
		
		ImGui::PopItemWidth();

		bool colNotesEdit = channelColorEdit("Notes", "Notes", _state.baseColors);
		ImGui::SameLine();
		bool colMinorsEdit = channelColorEdit("Minors", "Minors", _state.minorColors);

		ImGui::SameLine(COLUMN_SIZE);
		if (ImGui::Checkbox("Sync colors", &_state.lockParticleColor)) {
			// If we enable the lock, make sure the colors are synched.
			colNotesEdit = true;
		}

		if(ImGui::Checkbox("Per-set colors", &_state.perChannelColors)){
			if(!_state.perChannelColors){
				_state.synchronizeChannels();
			}
		}
		if(_state.perChannelColors){
			ImGui::SameLine(COLUMN_SIZE);
			if(ImGui::Button("Define sets...")){
				ImGui::OpenPopup("Note sets options");
			}
			showSets();
		}

		bool colFlashesEdit = false;
		if (_state.showFlashes && ImGui::CollapsingHeader("Flashes##HEADER")) {

			colFlashesEdit = channelColorEdit("Color##Flashes", "Color", _state.flashColors);

			ImGui::SameLine(COLUMN_SIZE);
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("Flash size", &_state.flashSize, 0.1f, 3.0f);
			ImGui::PopItemWidth();
		}

		bool colPartsEdit = false;
		if (_state.showParticles && ImGui::CollapsingHeader("Particles##HEADER")) {

			ImGui::PushID("ParticlesSettings");

			colPartsEdit = channelColorEdit("Color##Particles", "Color", _state.particles.colors);

			ImGui::SameLine(COLUMN_SIZE);

			ImGui::PushItemWidth(100);
			if (ImGui::InputFloat("Size", &_state.particles.scale, 1.0f, 10.0f)) {
				_state.particles.scale = std::max(1.0f, _state.particles.scale);
			}

			ImGui::PushItemWidth(COLUMN_SIZE-10);
			if (ImGui::SliderInt("Count", &_state.particles.count, 1, 512)) {
				_state.particles.count = std::min(std::max(_state.particles.count, 1), 512);
			}
			ImGui::PopItemWidth();

			const bool mp0 = ImGui::InputFloat("Speed", &_state.particles.speed, 0.001f, 1.0f);
			ImGui::SameLine(COLUMN_SIZE);
			const bool mp1 = ImGui::InputFloat(	"Expansion", &_state.particles.expansion, 0.1f, 5.0f);
			ImGui::PopItemWidth();

			if (mp1 || mp0) {
				_scene->setParticlesParameters(_state.particles.speed, _state.particles.expansion);
			}

			if (ImGui::Button("Load images...##Particles")) {
				// Read arguments.
				nfdpathset_t outPaths;
				nfdresult_t result = NFD_OpenDialogMultiple("png;jpg,jpeg;", NULL, &outPaths);

				if (result == NFD_OKAY) {
					std::vector<std::string> paths;
					for (size_t i = 0; i < NFD_PathSet_GetCount(&outPaths); ++i) {
						nfdchar_t *outPath = NFD_PathSet_GetPath(&outPaths, i);
						const std::string imageFilePath = std::string(outPath);
						paths.push_back(imageFilePath);
					}
					if (_state.particles.tex != ResourcesManager::getTextureFor("blankarray")) {
						glDeleteTextures(1, &_state.particles.tex);
					}
					_state.particles.tex = loadTextureArray(paths, false, _state.particles.texCount);
					NFD_PathSet_Free(&outPaths);
					if (_state.particles.scale <= 9.0f) {
						_state.particles.scale = 10.0f;
					}
				}
			}
			ImGui::SameLine();
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted("You can select multiple images (PNG or JPEG). They should be square and greyscale, where black is transparent, white opaque.");
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}

			ImGui::SameLine(COLUMN_SIZE);
			if (ImGui::Button("Clear images##TextureParticles")) {
				if (_state.particles.tex != ResourcesManager::getTextureFor("blankarray")) {
					glDeleteTextures(1, &_state.particles.tex);
				}
				// Use a white square particle appearance by default.
				_state.particles.tex =  ResourcesManager::getTextureFor("blankarray");
				_state.particles.texCount = 1;
				_state.particles.scale = 1.0f;
			}
			ImGui::PopID();
		}

		if (_state.showKeyboard && ImGui::CollapsingHeader("Keyboard##HEADER")) {
			ImGui::PushItemWidth(25);
			if (ImGui::ColorEdit3("Color##Keys", &_state.background.keysColor[0], ImGuiColorEditFlags_NoInputs)) {
				_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);
			}
			ImGui::PopItemWidth();
			ImGui::SameLine(COLUMN_SIZE);


			ImGui::PushItemWidth(100);
			if(ImGui::SliderFloat("Size##Keys", &_state.keyboard.size, 0.0f, 1.0f)){
				_state.keyboard.size = (std::min)((std::max)(_state.keyboard.size, 0.0f), 1.0f);
				_scene->setKeyboardSize(_state.keyboard.size);
				_score->setKeyboardSize(_state.keyboard.size);
			}
			ImGui::PopItemWidth();

			ImGui::Checkbox("Highlight keys", &_state.keyboard.highlightKeys);

			if (_state.keyboard.highlightKeys) {
				ImGui::Checkbox("Custom colors", &_state.keyboard.customKeyColors);
				if (_state.keyboard.customKeyColors) {

					ImGui::SameLine(COLUMN_SIZE);
					if(ImGui::ColorEdit3("Major##KeysHighlight", &_state.keyboard.majorColor[0][0], ImGuiColorEditFlags_NoInputs)){
						// Ensure synchronization of the override array.
						for(size_t cid = 1; cid < _state.keyboard.majorColor.size(); ++cid){
							_state.keyboard.majorColor[cid] = _state.keyboard.majorColor[0];
						}
					}
					
					ImGui::SameLine(COLUMN_SIZE+80);
					if(ImGui::ColorEdit3("Minor##KeysHighlight", &_state.keyboard.minorColor[0][0], ImGuiColorEditFlags_NoInputs)){
						// Ensure synchronization of the override array.
						for(size_t cid = 1; cid < _state.keyboard.minorColor.size(); ++cid){
							_state.keyboard.minorColor[cid] = _state.keyboard.minorColor[0];
						}
					}
				}
			}
			
		}

		if (_state.showScore && ImGui::CollapsingHeader("Score##HEADER")) {
			ImGui::PushItemWidth(25);
			const bool cbg0 = ImGui::ColorEdit3("Lines##Background", &_state.background.linesColor[0], ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			const bool cbg1 = ImGui::ColorEdit3("Text##Background", &_state.background.textColor[0], ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::SameLine(COLUMN_SIZE);
			const bool m1 = ImGui::Checkbox("Digits", &_state.background.digits);
			const bool m2 = ImGui::Checkbox("Horizontal lines", &_state.background.hLines);
			ImGui::SameLine(COLUMN_SIZE);
			const bool m3 = ImGui::Checkbox("Vertical lines", &_state.background.vLines);

			if (m1 || m2 || m3) {
				_score->setDisplay(_state.background.digits, _state.background.hLines, _state.background.vLines);
			}

			if (cbg0 || cbg1) {
				_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);
			}
		}

		if (_state.showBlur && ImGui::CollapsingHeader("Blur##HEADER")) {
			ImGui::Checkbox("Blur the notes", &_state.showBlurNotes);
			ImGui::SameLine(COLUMN_SIZE);
			ImGui::PushItemWidth(100);
			if (ImGui::SliderFloat("Fading", &_state.attenuation, 0.0f, 1.0f)) {
				_state.attenuation = std::min(1.0f, std::max(0.0f, _state.attenuation));
				glUseProgram(_blurringScreen.programId());
				const GLuint id1 = glGetUniformLocation(_blurringScreen.programId(), "attenuationFactor");
				glUniform1f(id1, _state.attenuation);
				glUseProgram(0);
			}
			ImGui::PopItemWidth();
		}

		if (ImGui::CollapsingHeader("Background##HEADER")) {
			ImGui::PushItemWidth(25);
			ImGui::ColorEdit3("Color##Background", &_state.background.color[0],
				ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::SameLine(COLUMN_SIZE);
			ImGui::PushItemWidth(100);
			if (ImGui::SliderFloat("Opacity##Background", &_state.background.imageAlpha, 0.0f, 1.0f)) {
				_state.background.imageAlpha = std::min(std::max(_state.background.imageAlpha, 0.0f), 1.0f);
			}
			ImGui::PopItemWidth();
			if (ImGui::Button("Load image...##Background")){
				// Read arguments.
				nfdchar_t *outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("jpg,jpeg;png", NULL, &outPath);
				if (result == NFD_OKAY) {
					glDeleteTextures(1, &_state.background.tex);
					_state.background.tex = loadTexture(std::string(outPath), 4, false);
					_state.background.image = true;
					// Ensure minimal visibility.
					if (_state.background.imageAlpha < 0.1f) {
						_state.background.imageAlpha = 0.1f;
					}
				}
			}
			ImGui::SameLine(COLUMN_SIZE);
			if (ImGui::Button("Clear image##Background")) {
				_state.background.image = false;
				glDeleteTextures(1, &_state.background.tex);
				_state.background.tex = 0;
			}
			ImGui::Checkbox("Image extends under keyboard", &_state.background.imageBehindKeyboard);

		}
		ImGui::Separator();

		if(ImGui::Button("Export...")){
			ImGui::OpenPopup("Export");
		}
		if(_recorder.drawGUI()){
			startRecording();
		}


		ImGui::SameLine();

		if (ImGui::Button("Save config...")) {
			// Read arguments.
			nfdchar_t *savePath = NULL;
			nfdresult_t result = NFD_SaveDialog("ini", NULL, &savePath);
			if (result == NFD_OKAY) {
				_state.save(std::string(savePath));
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Load config...")) {
			// Read arguments.
			nfdchar_t *outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("ini", NULL, &outPath);
			if (result == NFD_OKAY) {
				_state.load(std::string(outPath));
				setState(_state);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset##config")) {
			_state.reset();
			setState(_state);
		}

		if (smw0 || smw1) {
			_state.scale = std::max(_state.scale, 0.01f);
			_state.background.minorsWidth = std::min(std::max(_state.background.minorsWidth, 0.1f), 1.0f);
			_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
			_score->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
		}

		// Keep the colors in sync if needed.
		if (_state.lockParticleColor && (colNotesEdit || colPartsEdit || colMinorsEdit || colFlashesEdit)) {
			for(size_t cid = 0; cid < CHANNELS_COUNT; ++cid){
				glm::vec3 refColor = _state.baseColors[cid];
				if (colPartsEdit) {
					refColor = _state.particles.colors[cid];
				}
				else if (colMinorsEdit) {
					refColor = _state.minorColors[cid];
				}
				else if (colFlashesEdit) {
					refColor = _state.flashColors[cid];
				}
				_state.baseColors[cid] = _state.particles.colors[cid] = _state.minorColors[cid] = _state.flashColors[cid] = refColor;
			}

		}

		if (_showDebug) {
			ImGui::Separator();
			ImGui::Text("Debug: ");
			ImGui::SameLine();
			ImGui::TextDisabled("(press D to hide)");
			ImGui::Text("%.1f FPS / %.1f ms", ImGui::GetIO().Framerate, ImGui::GetIO().DeltaTime * 1000.0f);
			ImGui::Text("Final framebuffer size: %dx%d, screen size: %dx%d", _finalFramebuffer->_width, _finalFramebuffer->_height, _camera.screenSize()[0], _camera.screenSize()[1]);
			if (ImGui::Button("Print MIDI content to console")) {
				_scene->midiFile().print();
			}
		}
	}
	ImGui::End();

	if (_showLayers) {
		showLayers();
	}

	return action;
}

void Renderer::showLayers() {
	const ImVec2 & screenSize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, screenSize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
	
	if (ImGui::Begin("Layers", &_showLayers)) {
		ImGui::TextDisabled("You can drag and drop layers to reorder them.");
		for (int i = int(_state.layersMap.size()) - 1; i >= 0; --i) {
			const int layerId = _state.layersMap[i];
			if (layerId >= _layers.size()) {
				continue;
			}
			auto & layer = _layers[layerId];
			if (layer.type == Layer::BGCOLOR) {
				continue;
			}
			ImGui::Separator();
			ImGui::PushID(layerId);

			ImGui::Checkbox("##LayerCheckbox", layer.toggle);
			ImGui::SameLine();
			ImGui::Selectable(layer.name.c_str());

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::Text("%s", layer.name.c_str());
				ImGui::SetDragDropPayload("REORDER_LAYER", &i, sizeof(int));
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("REORDER_LAYER"))
				{
					int iPayload = *(const int*)payload->Data;
					int newId = _state.layersMap[iPayload];
					// Instead of just swapping, we shift all intermediate indices.
					const int ddlt = (iPayload <= i ? 1 : -1);
					for (int lid = iPayload; lid != i; lid += ddlt) {
						_state.layersMap[lid] = _state.layersMap[lid + ddlt];
					}
					_state.layersMap[i] = newId;
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::PopID();
		}
		ImGui::Separator();
	}
	ImGui::End();
}

void Renderer::showSets(){
	if(ImGui::BeginPopup("Note sets options")){
		ImGui::Text("Decide how notes should be grouped in multiple sets");
		ImGui::Text("(to which you can assign different key/effects colors).");
		ImGui::Text("This can be based on the MIDI channel, the track or by");
		ImGui::Text("separating notes that are lower or higher than a given key.");

		bool shouldUpdate = false;
		shouldUpdate = ImGui::RadioButton("Channel", (int*)(&_state.setOptions.mode), int(SetMode::CHANNEL)) || shouldUpdate;
		ImGui::SameLine(120);
		shouldUpdate = ImGui::RadioButton("Track", (int*)(&_state.setOptions.mode), int(SetMode::TRACK)) || shouldUpdate;
		ImGui::SameLine(2*120);
		shouldUpdate = ImGui::RadioButton("Key", (int*)(&_state.setOptions.mode), int(SetMode::KEY)) || shouldUpdate;
		ImGui::SameLine();

		ImGui::PushItemWidth(100);
		shouldUpdate = ImGui::Combo("##key", &_state.setOptions.key, "A0\0A#0\0B0\0C1\0C#1\0D1\0D#1\0E1\0F1\0F#1\0G1\0G#1\0A1\0A#1\0B1\0C2\0C#2\0D2\0D#2\0E2\0F2\0F#2\0G2\0G#2\0A2\0A#2\0B2\0C3\0C#3\0D3\0D#3\0E3\0F3\0F#3\0G3\0G#3\0A3\0A#3\0B3\0C4\0C#4\0D4\0D#4\0E4\0F4\0F#4\0G4\0G#4\0A4\0A#4\0B4\0C5\0C#5\0D5\0D#5\0E5\0F5\0F#5\0G5\0G#5\0A5\0A#5\0B5\0C6\0C#6\0D6\0D#6\0E6\0F6\0F#6\0G6\0G#6\0A6\0A#6\0B6\0C7\0C#7\0D7\0D#7\0E7\0F7\0F#7\0G7\0G#7\0A7\0A#7\0B7\0C8\0D8\0") || shouldUpdate;
		ImGui::PopItemWidth();

		if(shouldUpdate){
			_scene->updateSets(_state.setOptions);
		}
		ImGui::EndPopup();
	}
}

void Renderer::applyAllSettings() {
	// Apply all modifications.

	// One-shot parameters.
	_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_score->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_scene->setParticlesParameters(_state.particles.speed, _state.particles.expansion);
	_score->setDisplay(_state.background.digits, _state.background.hLines, _state.background.vLines);
	_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);
	_scene->setKeyboardSize(_state.keyboard.size);
	_score->setKeyboardSize(_state.keyboard.size);

	// Reset buffers.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	_particlesFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_particlesFramebuffer->unbind();
	_blurFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_blurFramebuffer->unbind();
	glUseProgram(_blurringScreen.programId());
	GLuint id2 = glGetUniformLocation(_blurringScreen.programId(), "attenuationFactor");
	glUniform1f(id2, _state.attenuation);
	glUseProgram(0);

	// Resize the framebuffers.
	updateSizes();

	// Finally, restore the track at the beginning.
	reset();
	// All other parameters are directly used at render.
}

void Renderer::clean() {

	// Clean objects.
	_scene->clean();
	_score->clean();
	_blurringScreen.clean();
	_passthrough.clean();
	_backgroundTexture.clean();
	_particlesFramebuffer->clean();
	_blurFramebuffer->clean();
	_finalFramebuffer->clean();
}

void Renderer::rescale(float scale){
	resizeAndRescale(_camera.screenSize()[0], _camera.screenSize()[1], scale);
}

void Renderer::resize(int width, int height) {
	resizeAndRescale(width, height, _camera.scale());
}

void Renderer::resizeAndRescale(int width, int height, float scale) {
	// Update the projection matrix.
	_camera.screen(width, height, scale);
	updateSizes();
}

void Renderer::updateSizes(){
	// Resize the framebuffers.
	const auto &currentQuality = Quality::availables.at(_state.quality);
	const glm::vec2 baseRes(_camera.renderSize());
	_particlesFramebuffer->resize(currentQuality.particlesResolution * baseRes);
	_blurFramebuffer->resize(currentQuality.blurResolution * baseRes);
	_finalFramebuffer->resize(currentQuality.finalResolution * baseRes);
	_recorder.setSize(glm::ivec2(_finalFramebuffer->_width, _finalFramebuffer->_height));
}

void Renderer::keyPressed(int key, int action) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_P) {
			_shouldPlay = !_shouldPlay;
			_timerStart = DEBUG_SPEED * float(glfwGetTime()) - _timer;
		}
		else if (key == GLFW_KEY_R) {
			reset();
		}
		else if (key == GLFW_KEY_I) {
			_showGUI = !_showGUI;
		}
		else if (key == GLFW_KEY_D) {
			_showDebug = !_showDebug;
		}
	}
}

void Renderer::reset() {
	_timer = -_state.prerollTime;
	_timerStart = DEBUG_SPEED * float(glfwGetTime()) + (_shouldPlay ? _state.prerollTime : 0.0f);
	_scene->resetParticles();
}

void Renderer::setState(const State & state){
	_state = state;

	// Update toggles.
	_layers[Layer::BGTEXTURE].toggle = &_state.background.image;
	_layers[Layer::BLUR].toggle = &_state.showBlur;
	_layers[Layer::ANNOTATIONS].toggle = &_state.showScore;
	_layers[Layer::KEYBOARD].toggle = &_state.showKeyboard;
	_layers[Layer::PARTICLES].toggle = &_state.showParticles;
	_layers[Layer::NOTES].toggle = &_state.showNotes;
	_layers[Layer::FLASHES].toggle = &_state.showFlashes;

	// Update split notes.
	if(_scene){
		_scene->updateSets(_state.setOptions);
	}
	applyAllSettings();
}

void Renderer::startDirectRecording(const std::string & path, Recorder::Format format, int framerate, int bitrate, bool skipBackground, const glm::vec2 & size){
	_recorder.setParameters(path, format, framerate, bitrate, skipBackground);
	_recorder.setSize(size);
	startRecording();
	_exitAfterRecording = true;
}

void Renderer::startRecording(){
	// We need to provide some information for the recorder to start.
	_recorder.start(_state.prerollTime, float(_scene->duration()));

	// Start by clearing up all buffers.
	// We need:
	// - the rendering res (taking into account quality and screen scaling) to be equal to requiredSize().
	// - the camera screen size and scale to remain the same afterwards (and during for a nice background display).
	// We can do this by leveraging the fact that camera parameters are not used during render.
	// We can thus:
	// - backup the camera parameters
	// - trigger a buffers size update at the target resolution
	// - restore the camera parameters.
	// All that will be left is to trigger a size update at the end of the recording.

	const glm::ivec2 backSize = _camera.screenSize();
	const float backScale = _camera.scale();

	const auto &currentQuality = Quality::availables.at(_state.quality);
	const glm::vec2 finalSize = glm::vec2(_recorder.requiredSize()) / currentQuality.finalResolution;
	resizeAndRescale(int(finalSize[0]), int(finalSize[1]), 1.0f);

	_camera.screen(backSize[0], backSize[1], backScale);

	// Reset buffers.
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	_particlesFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_blurFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_finalFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_finalFramebuffer->unbind();
}

bool Renderer::channelColorEdit(const char * name, const char * displayName, ColorArray & colors){
	if(!_state.perChannelColors){
		// If locked, display color sink.
		ImGui::PushItemWidth(25);
		const bool inter = ImGui::ColorEdit3(name, &colors[0][0], ImGuiColorEditFlags_NoInputs);
		ImGui::PopItemWidth();
		if(inter){
			// Ensure synchronization.
			for(size_t cid = 1; cid < colors.size(); ++cid){
				colors[cid] = colors[0];
			}
		}
		return inter;
	}

	// Else, we display a drop down and a popup.
	if(ImGui::ArrowButton(name, ImGuiDir_Down)){
		ImGui::OpenPopup(name);
	}
	ImGui::SameLine(); ImGui::Text("%s", displayName);

	if(ImGui::BeginPopup(name)){
		// Do 2x4 color sinks.
		bool edit = false;
		ImGui::PushItemWidth(25);
		for(size_t cid = 0; cid < colors.size(); ++cid){
			const std::string nameC = "Set " + std::to_string(cid);
			edit = ImGui::ColorEdit3(nameC.c_str(), &colors[cid][0], ImGuiColorEditFlags_NoInputs) || edit;
			if(cid % 2 == 0 && cid != colors.size()-1){
				ImGui::SameLine();
			}
		}
		ImGui::PopItemWidth();
		ImGui::EndPopup();
		return edit;
	}
	return false;
}
