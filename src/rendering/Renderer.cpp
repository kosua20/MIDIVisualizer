#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"
#include "../helpers/ImGuiStyle.h"
#include "../helpers/System.h"
#include <cstring>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <iostream>
#include <sr_gui.h>
#include <stdio.h>
#include <vector>

#include "Renderer.h"
#include "scene/MIDIScene.h"
#include "scene/MIDISceneFile.h"
#include "scene/MIDISceneLive.h"

#include <algorithm>
#include <fstream>


bool ImGuiSliderPercent(const char* label, float* v, float v_min, float v_max){
	float tmp = (*v) * 100.0f;
	const bool press = ImGui::SliderFloat(label, &tmp, v_min * 100.0f, v_max * 100.0f, "%.0f%%");
	if(press){
		*v = tmp / 100.0f;
	}
	return press;
}

SystemAction::SystemAction(SystemAction::Type act) {
	type = act;
	data = glm::ivec4(0);
}

Renderer::Renderer(const Configuration& config) :
	_supportTransparency(!config.preventTransparency) {

	_showGUI = true;
	_showDebug = false;

	_fullscreen = config.fullscreen;
	_windowSize = config.windowSize;
	_useTransparency = config.useTransparency && _supportTransparency;

	// GL options
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	_camera.screen(config.windowSize[0], config.windowSize[1], 1.0f);
	_backbufferSize = glm::vec2(config.windowSize);

	// Setup framebuffers, size does not really matter as we expect a resize event just after.
	const glm::ivec2 renderSize = _camera.renderSize();
	_particlesFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
		GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_blurFramebuffer0 = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
		GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_blurFramebuffer1 = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
		GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_renderFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
	GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_finalFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(renderSize[0], renderSize[1],
		GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));

	_backgroundTexture.init("backgroundtexture_frag", "backgroundtexture_vert");
	_blurringScreen.init(_particlesFramebuffer->textureId(), "particlesblur_frag");
	_fxaa.init("fxaa_frag");
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

	_layers[Layer::PEDAL].type = Layer::PEDAL;
	_layers[Layer::PEDAL].name = "Pedal";
	_layers[Layer::PEDAL].draw = &Renderer::drawPedals;

	_layers[Layer::WAVE].type = Layer::WAVE;
	_layers[Layer::WAVE].name = "Waves";
	_layers[Layer::WAVE].draw = &Renderer::drawWaves;

	// Register state.
	_layers[Layer::BGTEXTURE].toggle = &_state.background.image;
	_layers[Layer::BLUR].toggle = &_state.showBlur;
	_layers[Layer::ANNOTATIONS].toggle = &_state.showScore;
	_layers[Layer::KEYBOARD].toggle = &_state.showKeyboard;
	_layers[Layer::PARTICLES].toggle = &_state.showParticles;
	_layers[Layer::NOTES].toggle = &_state.showNotes;
	_layers[Layer::FLASHES].toggle = &_state.showFlashes;
	_layers[Layer::PEDAL].toggle = &_state.showPedal;
	_layers[Layer::WAVE].toggle = &_state.showWave;

	// Check setup errors.
	checkGLError();

	_score.reset(new Score(2.0f));
	_scene.reset(new MIDISceneEmpty());
}

Renderer::~Renderer() {}

bool Renderer::loadFile(const std::string& midiFilePath) {
	std::shared_ptr<MIDIScene> scene(nullptr);

	try {
		scene = std::make_shared<MIDISceneFile>(midiFilePath, _state.setOptions);
	} catch(...){
		// Failed to load.
		return false;
	}
	// Player.
	_timer = -_state.prerollTime;
	_shouldPlay = false;
	_liveplay = false;

	// Init objects.
	_scene = scene;
	_score = std::make_shared<Score>(_scene->secondsPerMeasure());
	applyAllSettings();
	return true;
}

bool Renderer::connectDevice(const std::string& deviceName) {
	std::shared_ptr<MIDIScene> scene(nullptr);
	_selectedPort = -1;
	
	const auto & devices = MIDISceneLive::availablePorts(true);
	for(int i = 0; i < devices.size(); ++i){
		if(devices[i] == deviceName){
			_selectedPort = i;
		}
	}

	if(_selectedPort == -1){
		if(deviceName != VIRTUAL_DEVICE_NAME){
			std::cerr << "[MIDI] Unable to connect to device named " << deviceName << "." << std::endl;
			return false;
		}
	}

	_scene = std::make_shared<MIDISceneLive>(_selectedPort, _verbose);
	_timer = 0.0f;
	// Don't start immediately
	// _shouldPlay = true;
	_state.reverseScroll = true;
	_state.scrollSpeed = 1.0f;
	_liveplay = true;
	_score = std::make_shared<Score>(_scene->secondsPerMeasure());
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
			resize(_backbufferSize[0], _backbufferSize[1]);
		}
		// Make sure the backbuffer is updated, this is nicer.
		glViewport(0, 0, GLsizei(_backbufferSize[0]), GLsizei(_backbufferSize[1]));
		_passthrough.draw(_finalFramebuffer->textureId(), _timer);
		return action;
	}

	// -- Default mode --

	// Compute the time elapsed since last frame, or keep the same value if
	// playback is disabled.
	_timer = _shouldPlay ? (currentTime - _timerStart) : _timer;

	// Render scene and blit, with GUI on top if needed.
	drawScene(_useTransparency);

	glViewport(0, 0, GLsizei(_backbufferSize[0]), GLsizei(_backbufferSize[1]));
	_passthrough.draw(_finalFramebuffer->textureId(), _timer);

	SystemAction action = SystemAction::NONE;
	if(_showGUI){
		action = drawGUI(currentTime);
	}

	return action;
}

void Renderer::drawScene(bool transparentBG){

	// Update active notes listing (for particles).
	_scene->updatesActiveNotes(_state.scrollSpeed * _timer, _state.scrollSpeed);

	const glm::vec2 invSizeFb = 1.0f / glm::vec2(_renderFramebuffer->_width, _renderFramebuffer->_height);

	// Blur rendering.
	if (_state.showBlur) {
		blurPrepass();
	}

	// Set viewport
	_renderFramebuffer->bind();
	glViewport(0, 0, _renderFramebuffer->_width, _renderFramebuffer->_height);

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

	_renderFramebuffer->unbind();

	// Apply fxaa.
	if(_state.applyAA){
		_finalFramebuffer->bind();
		_fxaa.draw(_renderFramebuffer->textureId(), 0.0, invSizeFb);
		_finalFramebuffer->unbind();
	} else {
		// Else just do a blit.
		_renderFramebuffer->bind(GL_READ_FRAMEBUFFER);
		_finalFramebuffer->bind(GL_DRAW_FRAMEBUFFER);
		glBlitFramebuffer(0, 0, _renderFramebuffer->_width, _renderFramebuffer->_height, 0, 0, _finalFramebuffer->_width, _finalFramebuffer->_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		_finalFramebuffer->unbind();
	}

}

void Renderer::blurPrepass() {
	const glm::vec2 invSizeB = 1.0f / glm::vec2(_particlesFramebuffer->_width, _particlesFramebuffer->_height);
	// Bind particles buffer.
	_particlesFramebuffer->bind();
	// Set viewport.
	glViewport(0, 0, _particlesFramebuffer->_width, _particlesFramebuffer->_height);
	// Draw blurred particles from previous frames.
	_passthrough.draw(_blurFramebuffer1->textureId(), _timer);
	if (_state.showParticles) {
		// Draw the new particles.
		_scene->drawParticles(_timer, invSizeB, _state.particles, true);
	}
	if (_state.showBlurNotes) {
		// Draw the notes.
		_scene->drawNotes(_timer, invSizeB, _state.notes, _state.reverseScroll, true);
	}

	_particlesFramebuffer->unbind();

	// Perform blur on result from particles pass.
	// We re-use the 'time' uniform to denote if this is the vertical or horizontal pass.
	// Horizontal pass
	const glm::vec2 invBlurSize0 = 1.0f / glm::vec2(_particlesFramebuffer->_width, _particlesFramebuffer->_height);
	glViewport(0, 0, _blurFramebuffer0->_width, _blurFramebuffer0->_height);
	_blurFramebuffer0->bind();
	_blurringScreen.draw(_particlesFramebuffer->textureId(), 0.0f, invBlurSize0);
	_blurFramebuffer0->unbind();
	// Vertical pass
	const glm::vec2 invBlurSize1 = 1.0f / glm::vec2(_blurFramebuffer0->_width, _blurFramebuffer0->_height);
	glViewport(0, 0, _blurFramebuffer1->_width, _blurFramebuffer1->_height);
	_blurFramebuffer1->bind();
	_blurringScreen.draw(_blurFramebuffer0->textureId(), 1.0f, invBlurSize1);
	_blurFramebuffer1->unbind();

}

void Renderer::drawBackgroundImage(const glm::vec2 &) {
	// Use background.tex and background.imageAlpha
	// Early exit if no texture or transparent.
	if(_state.background.tex == 0 || _state.background.imageAlpha < 1.0f/255.0f) {
		return;
	}
	glEnable(GL_BLEND);
	ShaderProgram& bgProg = _backgroundTexture.program();
	bgProg.use();
	bgProg.uniform("textureAlpha", _state.background.imageAlpha);
	bgProg.uniform("behindKeyboard", _state.background.imageBehindKeyboard);
	bgProg.uniform("keyboardHeight", _state.keyboard.size);
	_backgroundTexture.draw(_state.background.tex, _timer);

	glDisable(GL_BLEND);
}

void Renderer::drawBlur(const glm::vec2 &) {
	glEnable(GL_BLEND);
	_passthrough.draw(_blurFramebuffer1->textureId(), _timer);
	glDisable(GL_BLEND);
}

void Renderer::drawParticles(const glm::vec2 & invSize) {
	_scene->drawParticles(_timer, invSize, _state.particles, false);
}

void Renderer::drawScore(const glm::vec2 & invSize) {
	glEnable(GL_BLEND);
	_score->draw(_timer * _state.scrollSpeed, invSize);
	glDisable(GL_BLEND);
}

void Renderer::drawKeyboard(const glm::vec2 & invSize) {
	const ColorArray & majColors = _state.keyboard.customKeyColors ? _state.keyboard.majorColor : _state.notes.majorColors;
	const ColorArray & minColors = _state.keyboard.customKeyColors ? _state.keyboard.minorColor : _state.notes.minorColors;
	_scene->drawKeyboard(_timer, invSize, _state.background.keysColor, majColors, minColors, _state.keyboard.highlightKeys);
}

void Renderer::drawNotes(const glm::vec2 & invSize) {
	glEnable(GL_BLEND);
	_scene->drawNotes(_timer * _state.scrollSpeed, invSize, _state.notes, _state.reverseScroll, false);
	glDisable(GL_BLEND);
}

void Renderer::drawFlashes(const glm::vec2 & invSize) {
	_scene->drawFlashes(_timer, invSize, _state.flashes);
}

void Renderer::drawPedals(const glm::vec2 & invSize){
	// Extra shift above the waves.
	_scene->drawPedals(_timer, invSize, _state.pedals, _state.keyboard.size + (_state.showWave ? 0.01f : 0.0f), _state.horizontalScroll);
}

void Renderer::drawWaves(const glm::vec2 & invSize){
	_scene->drawWaves(_timer, invSize, _state.waves, _state.keyboard.size);
}

SystemAction Renderer::drawGUI(const float currentTime) {

	SystemAction action = SystemAction::NONE;

	if (ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		action = showTopButtons(currentTime);
		ImGui::Separator();

		// Detail text.
		const int nCount = _scene->notesCount();
		const double duration = _scene->duration();
		const int speed = int(std::round(double(nCount)/(std::max)(0.001, duration)));
		ImGui::Text("Time: %.2f, notes: %d, duration: %.1fs, speed: %d notes/s", _timer * _state.scrollSpeed, nCount, duration, speed);

		ImGui::Separator();
		
		// Load button.
		if(ImGui::Button("Load MIDI file...")) {
			// Read arguments.
			char* outPath = nullptr;
			int res = sr_gui_ask_load_file("Load MIDI file", "", "mid", &outPath);
			System::forceLocale();
			if(res == SR_GUI_VALIDATED && outPath){
				loadFile(std::string(outPath));
			}
			free(outPath);
		}
		ImGuiSameLine(COLUMN_SIZE);
		if(_liveplay){
			if (ImGui::Button("Clear and stop session")) {
				_scene = std::make_shared<MIDISceneEmpty>();
				_liveplay = false;
				_shouldPlay = false;
				_timer = 0.0f;
				_score = std::make_shared<Score>(_scene->secondsPerMeasure());
				applyAllSettings();
			}
		} else {
			if (ImGui::Button("Connect to device...")) {
				ImGui::OpenPopup("Devices");
			}
			showDevices();
		}

		const bool emptyScene = std::dynamic_pointer_cast<MIDISceneEmpty>(_scene) != nullptr;
		if(!emptyScene)
		{
			if(ImGui::Button("Export MIDI file...")) {
				char* savePath = nullptr;
				int res = sr_gui_ask_save_file("Save MIDI file", "", "mid", &savePath);
				System::forceLocale();
				if(res == SR_GUI_VALIDATED && savePath){
					std::ofstream outFile = System::openOutputFile(std::string(savePath), true);
					_scene->save(outFile);
					outFile.close();
				}
				free(savePath);
			}
		}

		ImGui::Separator();

		ImGuiPushItemWidth(100);
		if (ImGui::Combo("Quality", (int *)(&_state.quality), "Half\0Low\0Medium\0High\0Double\0\0")) {
			updateSizes();
		}
		ImGui::PopItemWidth();

		// Add FXAA.
		ImGuiSameLine(COLUMN_SIZE);
		ImGui::Checkbox("Smoothing", &_state.applyAA);



		if (ImGui::Button("Show effect layers...")) {
			_showLayers = true;
		}
		ImGuiSameLine(COLUMN_SIZE);
		if (ImGui::Checkbox("Use same colors for all effects", &_state.lockParticleColor)) {
			// If we enable the lock, make sure the colors are synched.
			synchronizeColors(_state.notes.majorColors);
		}
		if(ImGui::Checkbox("Per-set colors", &_state.perSetColors)){
			if(!_state.perSetColors){
				_state.synchronizeSets();
			}
		}
		if(_state.perSetColors){
			ImGuiSameLine(COLUMN_SIZE);
			if(ImGui::Button("Define sets...")){
				ImGui::OpenPopup("Note sets options");
			}
			showSets();
		}

		if(ImGui::CollapsingHeader("Playback##HEADER")){
			ImGuiPushItemWidth(100);
			if(ImGui::Combo("Min key", &_state.minKey, midiKeysStrings, 128)){
				updateMinMaxKeys();
			}

			ImGuiSameLine(COLUMN_SIZE);
			if(ImGui::Combo("Max key", &_state.maxKey, midiKeysStrings, 128)){
				updateMinMaxKeys();
			}

			if (ImGui::InputFloat("Preroll", &_state.prerollTime, 0.1f, 1.0f, "%.1fs")) {
				reset();
			}

			if(!_liveplay){
				ImGuiSameLine(COLUMN_SIZE);
				if(ImGui::SliderFloat("Speed", &_state.scrollSpeed, 0.1f, 5.0f, "%.1fx")){
					_state.scrollSpeed = (std::max)(0.01f, _state.scrollSpeed);
				}
			}
			ImGui::PopItemWidth();

			if(ImGui::Checkbox("Horizontal scroll", &_state.horizontalScroll)){
				_score->setOrientation(_state.horizontalScroll);
				_scene->setOrientation(_state.horizontalScroll);
			}
			if(!_liveplay){
				ImGuiSameLine(COLUMN_SIZE);
				if(ImGui::Checkbox("Reverse scroll", &_state.reverseScroll)){
					_score->setPlayDirection(_state.reverseScroll);
				}
			}

		}

		if(ImGui::CollapsingHeader("Notes##HEADER")){
			showNoteOptions();
		}

		if (_state.showFlashes && ImGui::CollapsingHeader("Flashes##HEADER")) {
			showFlashOptions();
		}

		if (_state.showParticles && ImGui::CollapsingHeader("Particles##HEADER")) {
			showParticleOptions();
		}

		if (_state.showKeyboard && ImGui::CollapsingHeader("Keyboard##HEADER")) {
			showKeyboardOptions();
		}

		if(_state.showPedal && ImGui::CollapsingHeader("Pedal##HEADER")){
			showPedalOptions();
		}

		if(_state.showWave && ImGui::CollapsingHeader("Wave##HEADER")){
			showWaveOptions();
		}

		if (_state.showScore && ImGui::CollapsingHeader("Score##HEADER")) {
			showScoreOptions();
		}

		if (_state.showBlur && ImGui::CollapsingHeader("Blur##HEADER")) {
			showBlurOptions();
		}

		if (ImGui::CollapsingHeader("Background##HEADER")) {
			showBackgroundOptions();
		}
		ImGui::Separator();

		showBottomButtons();

		if (_showDebug) {
			ImGui::Separator();
			ImGui::Text("Debug: ");
			ImGuiSameLine();
			ImGui::TextDisabled("(press D to hide)");
			ImGui::Text("%.1f FPS / %.1f ms", ImGui::GetIO().Framerate, ImGui::GetIO().DeltaTime * 1000.0f);
			ImGui::Text("Render size: %dx%d, screen size: %dx%d", _renderFramebuffer->_width, _renderFramebuffer->_height, _camera.screenSize()[0], _camera.screenSize()[1]);
			if (ImGui::Button("Print MIDI content to console")) {
				_scene->print();
			}
			ImGui::Checkbox("Verbose log", &_verbose);
			if(ImGui::Button("Assign random colors")){
				_state.lockParticleColor = true;

				const ColorArray debugColors = {
					glm::vec3{1.0f, 0.0f, 0.0f},
					glm::vec3{1.0f, 0.5f, 0.0f},
					glm::vec3{1.0f, 1.0f, 0.0f},
					glm::vec3{0.5f, 1.0f, 0.0f},
					glm::vec3{0.0f, 1.0f, 0.0f},
					glm::vec3{0.0f, 1.0f, 0.5f},
					glm::vec3{0.0f, 1.0f, 1.0f},
					glm::vec3{0.0f, 0.5f, 1.0f},
					glm::vec3{0.0f, 0.0f, 1.0f},
					glm::vec3{0.5f, 0.0f, 1.0f},
					glm::vec3{1.0f, 0.0f, 1.0f},
					glm::vec3{1.0f, 0.0f, 0.5f}
				};
				synchronizeColors(debugColors);
			}
		}
	}
	ImGui::End();

	if(_showLayers){
		showLayers();
	}

	if(_showSetListEditor){
		showSetEditor();
	}

	if(_showParticleEditor){
		showParticlesEditor();
	}

	if(_shouldQuit != 0){
		// We should only open the popup once.
		if(_shouldQuit == 1){
			ImGui::OpenPopup("Quit");
			_shouldQuit = 2;
		}

		if(ImGui::BeginPopupModal("Quit", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)){
			ImGui::Text("Are you sure you want to quit?");
			const ImVec2 buttonSize(_guiScale * (COLUMN_SIZE - 20.0f), 0.0f);
			if(ImGui::Button("No", buttonSize)){
				_shouldQuit = 0;
			}
			ImGuiSameLine(COLUMN_SIZE);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
			if(ImGui::Button("Yes", buttonSize)){
				ImGui::PopStyleColor();
				ImGui::EndPopup();
				return SystemAction::QUIT;
			}
			ImGui::PopStyleColor();
			ImGui::EndPopup();
		}
	}
	return action;
}

void Renderer::synchronizeColors(const ColorArray & colors){
	// Keep the colors in sync if needed.
	if (!_state.lockParticleColor) {
		return;
	}

	for(size_t cid = 0; cid < SETS_COUNT; ++cid){
		_state.notes.majorColors[cid] = _state.particles.colors[cid] = _state.notes.minorColors[cid] = _state.flashes.colors[cid] = colors[cid];
	}

	// If we have only one channel, synchronize one-shot effects.
	// Disable this because it's not symmetric.
	//if(!_state.perChannelColors){
	//	_state.pedals.color = _state.waves.color = _state.notes.majorColors[0];
	//}
}

SystemAction Renderer::showTopButtons(double currentTime){
	if (ImGui::Button(_shouldPlay ? "Pause (p)" : "Play (p)")) {
		_shouldPlay = !_shouldPlay;
		_timerStart = float(currentTime) - _timer;
	}
	ImGuiSameLine();
	if (ImGui::Button("Restart (r)")) {
		reset();
	}
	ImGuiSameLine();
	if (ImGui::Button("Hide (i)")) {
		_showGUI = false;
	}
	ImGuiSameLine();
	if(ImGui::Button("Display")){
		ImGui::OpenPopup("Display options");
	}

	SystemAction action = SystemAction::NONE;
	if(ImGui::BeginPopup("Display options")){
		
		ImGuiPushItemWidth(100);
		if(ImGui::InputFloat("GUI size", &_guiScale, 0.25f, 1.0f, "%.2fx")){
			_guiScale = glm::clamp(_guiScale, 0.25f, 4.0f);
			setGUIScale(_guiScale);
		}
		ImGui::PopItemWidth();
		ImGuiSameLine(EXPORT_COLUMN_SIZE);
		if(ImGui::Button("Reset##GUI")){
			setGUIScale(1.0f);
		}

		if(ImGui::Checkbox("Fullscreen", &_fullscreen)){
			action = SystemAction::FULLSCREEN;
		}
		if(!_fullscreen){
			ImGuiPushItemWidth(100);
			ImGui::InputInt2("Window size", &_windowSize[0]);
			ImGui::PopItemWidth();
			ImGuiSameLine(EXPORT_COLUMN_SIZE);
			if(ImGui::Button("Resize")){
				action = SystemAction::RESIZE;
				action.data[0] = _windowSize[0];
				action.data[1] = _windowSize[1];
			}
		}

		if(_supportTransparency) {
			if(ImGui::Checkbox("Transparent", &_useTransparency)){
				_useTransparency = _useTransparency && _supportTransparency;
			}
		}

		ImGui::EndPopup();
	}

	ImGuiSameLine(360);
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
	return action;
}

void Renderer::showNoteOptions() {

	if(channelColorEdit("Notes", "Notes", _state.notes.majorColors)){
		synchronizeColors(_state.notes.majorColors);
	}
	ImGuiSameLine();
	if(channelColorEdit("Minors", "Minors", _state.notes.minorColors)){
		synchronizeColors(_state.notes.minorColors);
	}

	ImGuiSameLine(COLUMN_SIZE);
	if(ImGui::Button("Use background images...")){
		ImGui::OpenPopup("Note background textures");
	}

	if(ImGui::BeginPopup("Note background textures")){
		struct BackgroundTextureParams {
			const char* name;
			std::vector<std::string>& paths;
			float& scale;
			float& alpha;
			bool& scroll;
			GLuint& tex;
		};

		BackgroundTextureParams paramGroups[2] = {
			{"Major notes", _state.notes.majorImagePath, _state.notes.majorTexScale, _state.notes.majorTexAlpha, _state.notes.majorTexScroll, _state.notes.majorTex},
			{"Minor notes", _state.notes.minorImagePath, _state.notes.minorTexScale, _state.notes.minorTexAlpha, _state.notes.minorTexScroll, _state.notes.minorTex},
		};

		for(unsigned int i = 0; i < 2; ++i){
			BackgroundTextureParams& group = paramGroups[i];
			ImGui::PushID(group.name);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("%s", group.name);
			ImGuiSameLine(100);
			if(ImGui::Button("Load...")){
				 // Read arguments.
				 char *outPath = nullptr;
				 int res = sr_gui_ask_load_file("Select image", "", "jpg,jpeg,png", &outPath);
				 System::forceLocale();
				 if(res == SR_GUI_VALIDATED && outPath){
					 group.paths = { std::string(outPath) };
					 glDeleteTextures(1, &group.tex);
					 group.tex = loadTexture(group.paths[0], 4, false);
				 }
				 free(outPath);
			 }
			 ImGuiSameLine();
			 if(ImGui::Button("Clear")) {
				 group.paths.clear();
				 glDeleteTextures(1, &group.tex);
				 group.tex = 0;
			 }
			ImGuiSameLine();
			ImGuiPushItemWidth(100);
			if(ImGui::SliderFloat("Scale", &group.scale, 0.1f, 15.0f, "%.2fx")){
				group.scale = glm::max(group.scale, 0.001f);
			}
			ImGuiSameLine();
			if(ImGuiSliderPercent("Intensity", &group.alpha, 0.0f, 1.0f)){
				group.alpha = glm::clamp(group.alpha, 0.0f, 1.0f);
			}
			ImGuiSameLine();
			ImGui::Checkbox("Scroll", &group.scroll);
			ImGui::PopItemWidth();
			ImGui::PopID();
		}

		ImGui::EndPopup();
	}


	ImGuiPushItemWidth(100);
	bool smw0 = ImGui::InputFloat("Scale", &_state.scale, 0.01f, 0.1f, "%.2fx");
	ImGuiSameLine(COLUMN_SIZE);
	smw0 = ImGuiSliderPercent("Minor width", &_state.background.minorsWidth, 0.1f, 1.0f) || smw0;
	ImGui::PopItemWidth();

	if (smw0) {
		_state.scale = (std::max)(_state.scale, 0.01f);
		_state.background.minorsWidth = glm::clamp(_state.background.minorsWidth, 0.1f, 1.0f);
		_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
		_score->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	}

	ImGuiPushItemWidth(100);
	if(ImGui::SliderFloat("Radius", &_state.notes.cornerRadius, 0.0f, 1.0f)){
		_state.notes.cornerRadius = glm::clamp(_state.notes.cornerRadius, 0.0f, 1.0f);
	}
	ImGuiSameLine(COLUMN_SIZE);
	if(ImGui::SliderFloat("Fadeout", &_state.notes.fadeOut, 0.0f, 1.0f)){
		_state.notes.fadeOut = glm::clamp(_state.notes.fadeOut, 0.0f, 1.0f);
		_scene->setKeyboardSizeAndFadeout(_state.keyboard.size, _state.notes.fadeOut);
	}
	if(ImGuiSliderPercent("Edge", &_state.notes.edgeWidth, 0.0f, 1.0f)){
		_state.notes.edgeWidth = glm::clamp(_state.notes.edgeWidth, 0.0f, 1.0f);
	}
	ImGuiSameLine(COLUMN_SIZE);
	if(ImGuiSliderPercent("Intensity", &_state.notes.edgeBrightness, 0.0f, 4.0f)){
		_state.notes.edgeBrightness = glm::max(_state.notes.edgeBrightness, 0.0f);
	}
	ImGui::PopItemWidth();


}

void Renderer::showFlashOptions() {
	if(channelColorEdit("Color##Flashes", "Color", _state.flashes.colors)){
		synchronizeColors(_state.flashes.colors);
	}

	ImGuiSameLine(COLUMN_SIZE);
	ImGuiPushItemWidth(100);
	ImGui::SliderFloat("Scale##flash", &_state.flashes.size, 0.1f, 3.0f, "%.2fx");
	ImGui::PopItemWidth();
	// TODO: (MV) customize texture atlas
	// TODO: (MV) additional halo control
}

void Renderer::showParticleOptions(){
	ImGui::PushID("ParticlesSettings");

	if(channelColorEdit("Color##Particles", "Color", _state.particles.colors)){
		synchronizeColors(_state.particles.colors);
	}

	ImGuiSameLine(COLUMN_SIZE);

	ImGuiPushItemWidth(100);
	if (ImGui::InputFloat("Size##particles", &_state.particles.scale, 1.0f, 10.0f, "%.0fpx")) {
		_state.particles.scale = (std::max)(1.0f, _state.particles.scale);
	}

	const bool mp0 = ImGui::InputFloat("Speed", &_state.particles.speed, 0.01f, 1.0f, "%.2fx");
	ImGuiSameLine(COLUMN_SIZE);
	const bool mp1 = ImGui::InputFloat("Spread", &_state.particles.expansion, 0.1f, 5.0f, "%.1fx");

	if (ImGui::SliderInt("Count", &_state.particles.count, 1, 512)) {
		_state.particles.count = glm::clamp(_state.particles.count, 1, 512);
	}

	ImGui::PopItemWidth();

	if (mp1 || mp0) {
		_scene->setParticlesParameters(_state.particles.speed, _state.particles.expansion);
	}

	ImGuiSameLine(COLUMN_SIZE);

	if(ImGui::Button("Configure images...##Particles")) {
		_showParticleEditor = true;
		_backupParticlesOptions = _state.particles;
	}

	ImGui::PopID();
}

void Renderer::showKeyboardOptions(){
	ImGuiPushItemWidth(25);
	if (ImGui::ColorEdit3("Fill Color##Keys", &_state.background.keysColor[0], ImGuiColorEditFlags_NoInputs)) {
		_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);
	}
	ImGui::PopItemWidth();
	ImGuiSameLine(COLUMN_SIZE);


	ImGuiPushItemWidth(100);
	if(ImGuiSliderPercent("Height##Keys", &_state.keyboard.size, 0.0f, 1.0f)){
		_state.keyboard.size = glm::clamp(_state.keyboard.size, 0.0f, 1.0f);
		_scene->setKeyboardSizeAndFadeout(_state.keyboard.size, _state.notes.fadeOut);
		_score->setKeyboardSize(_state.keyboard.size);
	}
	ImGui::PopItemWidth();

	ImGuiPushItemWidth(25);
	if (ImGui::Checkbox("Minor edges##Keys", &_state.keyboard.minorEdges)){
		_scene->setMinorEdgesAndHeight(_state.keyboard.minorEdges, _state.keyboard.minorHeight);
	}
	ImGui::PopItemWidth();
	ImGuiSameLine(COLUMN_SIZE);

	ImGuiPushItemWidth(100);
	if(ImGuiSliderPercent("Minor height##Keys", &_state.keyboard.minorHeight, 0.0f, 1.0f)){
		_state.keyboard.minorHeight = glm::clamp(_state.keyboard.minorHeight, 0.0f, 1.0f);
		_scene->setMinorEdgesAndHeight(_state.keyboard.minorEdges, _state.keyboard.minorHeight);
	}
	ImGui::PopItemWidth();

	ImGui::Checkbox("Highlight keys", &_state.keyboard.highlightKeys);

	if (_state.keyboard.highlightKeys) {
		ImGui::Checkbox("Custom colors", &_state.keyboard.customKeyColors);
		if (_state.keyboard.customKeyColors) {

			ImGuiSameLine(COLUMN_SIZE);
			ImGuiPushItemWidth(25);
			if(ImGui::ColorEdit3("Major##KeysHighlight", &_state.keyboard.majorColor[0][0], ImGuiColorEditFlags_NoInputs)){
				// Ensure synchronization of the override array.
				for(size_t cid = 1; cid < _state.keyboard.majorColor.size(); ++cid){
					_state.keyboard.majorColor[cid] = _state.keyboard.majorColor[0];
				}
			}

			ImGuiSameLine(COLUMN_SIZE+80);
			if(ImGui::ColorEdit3("Minor##KeysHighlight", &_state.keyboard.minorColor[0][0], ImGuiColorEditFlags_NoInputs)){
				// Ensure synchronization of the override array.
				for(size_t cid = 1; cid < _state.keyboard.minorColor.size(); ++cid){
					_state.keyboard.minorColor[cid] = _state.keyboard.minorColor[0];
				}
			}
			ImGui::PopItemWidth();
		}
	}
}

void Renderer::showPedalOptions(){
	ImGuiPushItemWidth(25);
	ImGui::ColorEdit3("Color##Pedals", &_state.pedals.color[0], ImGuiColorEditFlags_NoInputs);
	ImGui::PopItemWidth();

	ImGuiPushItemWidth(100);
	ImGuiSameLine(COLUMN_SIZE);
	ImGui::Combo("Location", (int*)&_state.pedals.location, "Top left\0Bottom left\0Top right\0Bottom right\0");

	if(ImGuiSliderPercent("Opacity##Pedals", &_state.pedals.opacity, 0.0f, 1.0f)){
		_state.pedals.opacity = glm::clamp(_state.pedals.opacity, 0.0f, 1.0f);
	}
	ImGuiSameLine(COLUMN_SIZE);
	if(ImGuiSliderPercent("Size##Pedals", &_state.pedals.size, 0.05f, 0.5f)){
		_state.pedals.size = glm::clamp(_state.pedals.size, 0.05f, 0.5f);
	}
	ImGui::PopItemWidth();

	ImGui::Checkbox("Merge pedals", &_state.pedals.merge);

	// TODO: (MV) customize textures for each pedal
	// TODO: (MV) relative placement.
}

void Renderer::showWaveOptions(){
	ImGuiPushItemWidth(25);
	ImGui::ColorEdit3("Color##Waves", &_state.waves.color[0], ImGuiColorEditFlags_NoInputs);
	ImGui::PopItemWidth();

	ImGuiPushItemWidth(100);
	ImGuiSameLine(COLUMN_SIZE);
	ImGui::SliderFloat("Amplitude##Waves", &_state.waves.amplitude, 0.0f, 5.0f, "%.2fx");

	ImGui::SliderFloat("Spread##Waves", &_state.waves.spread, 0.0f, 5.0f, "%.2fx");
	ImGuiSameLine(COLUMN_SIZE);
	ImGui::SliderFloat("Frequency##Waves", &_state.waves.frequency, 0.0f, 5.0f, "%.2fx");

	if(ImGuiSliderPercent("Opacity##Waves", &_state.waves.opacity, 0.0f, 1.0f)){
		_state.waves.opacity = glm::clamp(_state.waves.opacity, 0.0f, 1.0f);
	}
	ImGui::PopItemWidth();

}

void Renderer::showBlurOptions(){
	ImGui::Checkbox("Blur the notes", &_state.showBlurNotes);
	ImGuiSameLine(COLUMN_SIZE);
	ImGuiPushItemWidth(100);
	if (ImGui::SliderFloat("Fading", &_state.attenuation, 0.0f, 1.0f)) {
		_state.attenuation = glm::clamp(_state.attenuation, 0.0f, 1.0f);
		_blurringScreen.program().use();
		_blurringScreen.program().uniform("attenuationFactor", _state.attenuation);
		glUseProgram(0);
	}
	ImGui::PopItemWidth();
}

void Renderer::showScoreOptions(){
	ImGuiPushItemWidth(25);
	const bool cbg0 = ImGui::ColorEdit3("Lines##Background", &_state.background.linesColor[0], ImGuiColorEditFlags_NoInputs);
	ImGuiSameLine();
	const bool cbg1 = ImGui::ColorEdit3("Text##Background", &_state.background.textColor[0], ImGuiColorEditFlags_NoInputs);
	ImGui::PopItemWidth();
	ImGuiSameLine(COLUMN_SIZE);
	const bool m1 = ImGui::Checkbox("Digits", &_state.background.digits);
	const bool m2 = ImGui::Checkbox("Horizontal lines", &_state.background.hLines);
	ImGuiSameLine(COLUMN_SIZE);
	const bool m3 = ImGui::Checkbox("Vertical lines", &_state.background.vLines);

	if (m1 || m2 || m3) {
		_score->setDisplay(_state.background.digits, _state.background.hLines, _state.background.vLines);
	}

	if (cbg0 || cbg1) {
		_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);
	}
	// TODO: (MV) lines width and text size
}

void Renderer::showBackgroundOptions(){
	ImGuiPushItemWidth(25);
	const glm::vec3 oldColor(_state.background.color);
	ImGui::ColorEdit3("Color##Background", &_state.background.color[0],
		ImGuiColorEditFlags_NoInputs);
	if(oldColor != _state.background.color){
		applyBackgroundColor();
	}
	ImGui::PopItemWidth();
	ImGuiSameLine(COLUMN_SIZE);
	ImGuiPushItemWidth(100);

	if (ImGuiSliderPercent("Opacity##Background", &_state.background.imageAlpha, 0.0f, 1.0f)) {
		_state.background.imageAlpha = glm::clamp(_state.background.imageAlpha, 0.0f, 1.0f);
	}

	ImGui::PopItemWidth();
	if (ImGui::Button("Load image...##Background")){
		// Read arguments.
		char *outPath = nullptr;
		int res = sr_gui_ask_load_file("Select image", "", "jpg,jpeg,png", &outPath);
		System::forceLocale();
		if(res == SR_GUI_VALIDATED && outPath){
			_state.background.imagePath = { std::string(outPath) };
			glDeleteTextures(1, &_state.background.tex);
			_state.background.tex = loadTexture(_state.background.imagePath[0], 4, false);
			if(_state.background.tex != 0){
				_state.background.image = true;
				// Ensure minimal visibility.
				if (_state.background.imageAlpha < 0.1f) {
					_state.background.imageAlpha = 0.1f;
				}
			}
		}
		free(outPath);
	}
	ImGuiSameLine(COLUMN_SIZE);
	if (ImGui::Button("Clear image##Background")) {
		_state.background.image = false;
		_state.background.imagePath.clear();
		glDeleteTextures(1, &_state.background.tex);
		_state.background.tex = 0;
	}
	ImGui::Checkbox("Image extends under keyboard", &_state.background.imageBehindKeyboard);

}

void Renderer::showBottomButtons(){
	if(ImGui::Button("Export...")){
		ImGui::OpenPopup("Export");
	}
	if(_recorder.drawGUI(_guiScale)){
		startRecording();
	}
	ImGuiSameLine();

	if (ImGui::Button("Save config...")) {
		// Read arguments.
		char* savePath = nullptr;
		int res = sr_gui_ask_save_file("Create config file", "", "ini", &savePath);
		System::forceLocale();
		if(res == SR_GUI_VALIDATED && savePath){
			_state.save(std::string(savePath));
		}
		free(savePath);
	}
	ImGuiSameLine();

	if (ImGui::Button("Load config...")) {
		// Read arguments.
		char* outPath = nullptr;
		int res = sr_gui_ask_load_file("Select config file", "", "ini", &outPath);
		System::forceLocale();
		if(res == SR_GUI_VALIDATED && outPath){
			if(_state.load(std::string(outPath))){
				setState(_state);
			}
		}
		free(outPath);
	}
	ImGuiSameLine();

	if (ImGui::Button("Reset##config")) {
		_state.reset();
		setState(_state);
	}
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
			ImGuiSameLine();
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

void Renderer::showDevices(){
	if(ImGui::BeginPopupModal("Devices", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){

		bool starting = false;
		const ImVec2 buttonSize(_guiScale * (EXPORT_COLUMN_SIZE-20.0f), 0.0f);

		ImGui::Text("Select a device to listen to or");

		ImGuiSameLine();

		if(ImGui::SmallButton("start virtual device")){
			_scene = std::make_shared<MIDISceneLive>(-1, _verbose);
			starting = true;
		}
		if(ImGui::IsItemHovered()){
			ImGui::SetTooltip("Act as a virtual device (via JACK)\nother MIDI elements can connect to");
		}
		ImGui::Separator();

		const auto & devices = MIDISceneLive::availablePorts();
		for(int i = 0; i < devices.size(); ++i){
			ImGui::RadioButton(devices[i].c_str(), &_selectedPort, i);
		}

		if(devices.empty()){
			ImGui::TextDisabled("No device found.");
		}

		ImGui::Separator();

		if(ImGui::Button("Cancel", buttonSize)){
			ImGui::CloseCurrentPopup();
		}

		if(!devices.empty()){
			ImGuiSameLine(EXPORT_COLUMN_SIZE);
			if(ImGui::Button("Start", buttonSize)){
				_scene = std::make_shared<MIDISceneLive>(_selectedPort, _verbose);
				starting = true;
			}
		}

		if(starting){
			_timer = 0.0f;
			_shouldPlay = true;
			_state.reverseScroll = true;
			_state.scrollSpeed = 1.0f;
			_liveplay = true;
			_score = std::make_shared<Score>(_scene->secondsPerMeasure());
			applyAllSettings();

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Renderer::showSets(){
	if(ImGui::BeginPopup("Note sets options")){
		ImGui::Text("Decide how notes should be grouped in multiple sets");
		ImGui::Text("(to which you can assign different key/effects colors).");
		ImGui::Text("This can be based on the MIDI channel, the track, the key,");
		ImGui::Text("using the chromatic sequence, separating notes that are");
		ImGui::Text("lower or higher than a given key, or defining custom lists.");

		bool shouldUpdate = false;
		shouldUpdate = ImGui::RadioButton("Channel", (int*)(&_state.setOptions.mode), int(SetMode::CHANNEL)) || shouldUpdate;
		ImGuiSameLine(90);
		shouldUpdate = ImGui::RadioButton("Track", (int*)(&_state.setOptions.mode), int(SetMode::TRACK)) || shouldUpdate;
		ImGuiSameLine(2*90);
		shouldUpdate = ImGui::RadioButton("Key", (int*)(&_state.setOptions.mode), int(SetMode::KEY)) || shouldUpdate;
		ImGuiSameLine(3*90);
		shouldUpdate = ImGui::RadioButton("Chromatic", (int*)(&_state.setOptions.mode), int(SetMode::CHROMATIC)) || shouldUpdate;

		shouldUpdate = ImGui::RadioButton("Split", (int*)(&_state.setOptions.mode), int(SetMode::SPLIT)) || shouldUpdate;
		ImGuiSameLine();
		ImGuiPushItemWidth(100);
		shouldUpdate = ImGui::Combo("##key", &_state.setOptions.key, midiKeysStrings, 128) || shouldUpdate;
		ImGui::PopItemWidth();

		ImGuiSameLine(2*90);
		shouldUpdate = ImGui::RadioButton("List", (int*)(&_state.setOptions.mode), int(SetMode::LIST)) || shouldUpdate;
		ImGuiSameLine();
		if(ImGui::Button("Configure...")){
			_showSetListEditor = true;
			_backupSetOptions = _state.setOptions;
			ImGui::CloseCurrentPopup();
		}

		if(shouldUpdate){
			_state.setOptions.rebuild();
			_scene->updateSets(_state.setOptions);
		}
		ImGui::EndPopup();
	}

}

static constexpr char const* kSetsComboString = " 0\0 1\0 2\0 3\0 4\0 5\0 6\0 7\0 8\0 9\0 10\0 11\0\0";

void Renderer::showSetEditor(){

	const unsigned int colWidth = 80;
	const unsigned int colButtonWidth = 50;
	const float offset = 8;

	// For editing.
	static SetOptions::KeyFrame newKey;

	// Initial window position.
	const ImVec2 & screenSize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, screenSize.y * 0.1f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.0f));
	ImGui::SetNextWindowSize({360, 360}, ImGuiCond_FirstUseEver);

	if(ImGui::Begin("Set List Editor", &_showSetListEditor)){

		bool refreshSetOptions = false;

		// Header
		ImGui::TextWrapped("Control points will determine which range of notes belong to each set at a given time. All notes below the given key will be assigned to the specified set (except if a set with a lower number is defined). Control points will kept being applied until a new one is encountered for the set.");

		// Load/save as CSV.
		if(ImGui::Button("Save control points...")){
			char* savePath = nullptr;
			int res = sr_gui_ask_save_file("Create CSV file", "", "csv", &savePath);
			System::forceLocale();
			if(res == SR_GUI_VALIDATED && savePath){
				const std::string content = _state.setOptions.toKeysString("\n");
				System::writeStringToFile(std::string(savePath), content);
			}
			free(savePath);
		}
		ImGuiSameLine();
		if(ImGui::Button("Load control points...")){
			// Read arguments.
			char* outPath = nullptr;
			int res = sr_gui_ask_load_file("Select CSV file", "", "csv", &outPath);
			System::forceLocale();
			if(res == SR_GUI_VALIDATED && outPath){
				const std::string str = System::loadStringFromFile(std::string(outPath));
				_state.setOptions.fromKeysString(str);
			}
			free(outPath);
			refreshSetOptions = true;
		}
		ImGuiSameLine();
		// Just restore the last backup.
		if(ImGui::Button("Reset")){
			_state.setOptions = _backupSetOptions;
			refreshSetOptions = true;
		}
		ImGui::Separator();

		// List of existing keys.
		// Keep some room at the bottom for the "new key" section.
		ImVec2 listSize = ImGui::GetContentRegionAvail();
		listSize.y -= 1.5 * ImGui::GetTextLineHeightWithSpacing();

		if(ImGui::BeginTable("#List", 4, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX |  ImGuiTableFlags_BordersH, listSize)){
			const size_t rowCount = _state.setOptions.keys.size();

			// Header
			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, _guiScale * colWidth);
			ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, _guiScale * colWidth);
			ImGui::TableSetupColumn("Set", ImGuiTableColumnFlags_WidthFixed, _guiScale * colWidth);
			ImGui::TableSetupColumn("Remove", ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_WidthFixed, colButtonWidth);
			ImGui::TableHeadersRow();

			int removeIndex = -1;

			for(size_t row = 0u; row < rowCount; ++row){

				SetOptions::KeyFrame& key = _state.setOptions.keys[row];

				ImGui::TableNextColumn();
				ImGui::PushID(row);

				ImGuiPushItemWidth(colWidth);
				if(ImGui::InputDouble("##Time", &key.time, 0, 0, "%.3fs")){
					key.time = (std::max)(key.time, 0.0);
				}
				// Postpone update until we are not focused anymore (else rows will jump around).
				if(ImGui::IsItemDeactivatedAfterEdit()){
					refreshSetOptions = true;
				}
				ImGui::PopItemWidth();

				ImGui::TableNextColumn();
				ImGuiPushItemWidth(colWidth);
				if(ImGui::Combo("##Key", &key.key, midiKeysStrings, 128)){
					refreshSetOptions = true;
				}
				ImGui::PopItemWidth();

				ImGui::TableNextColumn();
				ImGuiPushItemWidth(colWidth);
				// It is simpler to use a combo here (no weird focus issues when sorting rows).
				if(ImGui::Combo("##Set", &key.set, kSetsComboString)){
					refreshSetOptions = true;
				}
				ImGui::PopItemWidth();

				ImGui::TableNextColumn();
				if(ImGui::Button("x")){
					removeIndex = int(row);
				}

				ImGui::PopID();
			}

			ImGui::EndTable();

			// Remove after displaying the table.
			if(removeIndex >= 0){
				_state.setOptions.keys.erase(_state.setOptions.keys.begin() + removeIndex);
				refreshSetOptions = true;
			}
		}

		// Section to add a new key.
		// Mimic the inputs and size/alignment of the table items.
		ImGuiPushItemWidth(colWidth);
		if(ImGui::InputDouble("##Time", &newKey.time, 0, 0, "%.3fs")){
			newKey.time = (std::max)(0.0, newKey.time);
		}
		ImGui::PopItemWidth();
		ImGuiSameLine(colWidth + 2 * offset);
		ImGuiPushItemWidth(colWidth);
		ImGui::Combo("##Key", &newKey.key, midiKeysStrings, 128);
		ImGui::PopItemWidth();

		ImGuiSameLine(2 * colWidth + 3 * offset);
		ImGuiPushItemWidth(colWidth);
		ImGui::Combo("##Set", &newKey.set, kSetsComboString);
		ImGui::PopItemWidth();

		ImGuiSameLine(3 * colWidth + 4 * offset);
		if(ImGui::Button("Add")){
			auto insert = std::upper_bound(_state.setOptions.keys.begin(), _state.setOptions.keys.end(), newKey);
			_state.setOptions.keys.insert(insert, newKey);
			refreshSetOptions = true;
		}

		// Actions
		if(!_showSetListEditor){
			// If we are exiting, refresh the existing set.
			refreshSetOptions = true;
		}

		// If refresh is needed, ensure the set options are rebuilt and the scene udpated for live preview.
		if(refreshSetOptions){
			_state.setOptions.rebuild();
			if(_scene){
				_scene->updateSets(_state.setOptions);
			}
		}

		if(_showDebug){
			const double time = _timer * _state.scrollSpeed;

			std::array<int, SETS_COUNT> firstNoteInSet;
			std::array<int, SETS_COUNT> lastNoteInSet;
			firstNoteInSet.fill(-1);
			lastNoteInSet.fill(128);
			int lastSet = -1;
			for(int nid = 0; nid < 128; ++nid){
				int newSet = _state.setOptions.apply(nid, 0, 0, time);
				if(newSet != lastSet){
					if(lastSet >= 0){
						lastNoteInSet[lastSet] = nid - 1;
					}
					firstNoteInSet[newSet] = nid;
					lastSet = newSet;
				}
			}
			ImGui::Separator();
			ImGui::Text("Debug: ");
			ImGuiSameLine();
			ImGui::TextDisabled("(press D to hide)");
			ImGui::Text("At time %.3fs: ", time);

			for(unsigned int sid = 0; sid < SETS_COUNT; ++sid){
				int firstNote = firstNoteInSet[sid];
				int lastNote = lastNoteInSet[sid];

				if((firstNote != -1) || (lastNote != 128)){
					firstNote = glm::clamp(firstNote, 0, 127);
					lastNote  = glm::clamp(lastNote, 0, 127);

					ImGui::Text("* Set %u contains notes from %s to %s", sid, midiKeysStrings[firstNote], midiKeysStrings[lastNote]);
				}
			}

		}
	}
	ImGui::End();
}

void Renderer::showParticlesEditor(){

	const unsigned int colWidth = 270;
	const unsigned int colButtonWidth = 20;
	const float offset = 8;
	const unsigned int thumbSize = 24;
	const unsigned int thumbDisplaySize = _guiScale * thumbSize;

	// For previewing.
	static std::vector<GLuint> previewTextures;
	if(previewTextures.empty() && !_state.particles.imagePaths.empty()){
		previewTextures = generate2DViewsOfArray(_state.particles.tex, thumbSize);
	}

	// Initial window position.
	const ImVec2 & screenSize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(screenSize.x * 0.5f, screenSize.y * 0.1f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.0f));
	ImGui::SetNextWindowSize({360, 360}, ImGuiCond_FirstUseEver);

	if(ImGui::Begin("Particle Images Editor", &_showParticleEditor)){

		bool refreshTextures = false;

		// Header
		ImGui::TextWrapped("You can select multiple images (PNG or JPEG). They should be square and in grey levels, where black indicates transparent regions, and white regions are fully opaque.");

		if(ImGui::Button("Clear all")){
			_state.particles.imagePaths.clear();
			refreshTextures = true;
		}
		ImGuiSameLine();
		// Just restore the last backup.
		if(ImGui::Button("Reset")){
			_state.particles = _backupParticlesOptions;
			refreshTextures = true;
		}
		ImGui::Separator();

		// List of existing keys.
		// Keep some room at the bottom for the "new key" section.
		ImVec2 listSize = ImGui::GetContentRegionAvail();
		listSize.y -= 1.5 * ImGui::GetTextLineHeightWithSpacing();

		if(ImGui::BeginTable("#List", 3, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX |  ImGuiTableFlags_BordersH, listSize)){
			const size_t rowCount = _state.particles.imagePaths.size();

			// Header
			ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
			ImGui::TableSetupColumn("Preview", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderLabel, 1.5f * thumbDisplaySize);
			ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Remove", ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_WidthFixed, _guiScale * colButtonWidth);
			ImGui::TableHeadersRow();

			int removeIndex = -1;
			for(size_t row = 0u; row < rowCount; ++row){

				const std::string& path = _state.particles.imagePaths[row];

				ImGui::TableNextColumn();
				ImGui::PushID(row);
				if(ImGui::Selectable("##rowSelector", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap, ImVec2(0, thumbDisplaySize))) {
					// Open directory in file browser.
					sr_gui_open_in_explorer(path.c_str());
				}
				if(ImGui::IsItemHovered()){
					ImGui::SetTooltip("%s",path.c_str());
				}
				if(row < previewTextures.size()){
					ImGui::SameLine();
					ImGui::Image((ImTextureID)(uint64_t)previewTextures[row], ImVec2(thumbDisplaySize,thumbDisplaySize), ImVec2(0,1), ImVec2(1,0));
				}
				ImGui::TableNextColumn();
				ImGui::AlignTextToFramePadding();
				ImGuiPushItemWidth(colWidth);
				// Display the filename.
				std::string::size_type pos = path.find_last_of("/\\");
				pos = (pos == std::string::npos) ? 0 : (pos + 1);
				ImGui::Text("%s", path.c_str() + pos);

				ImGui::PopItemWidth();

				ImGui::TableNextColumn();
				if(ImGui::Button("x")){
					removeIndex = int(row);
				}
				ImGui::PopID();
			}
			ImGui::EndTable();

			// Remove after displaying the table.
			if(removeIndex >= 0){
				_state.particles.imagePaths.erase(_state.particles.imagePaths.begin() + removeIndex);
				refreshTextures = true;
			}
		}

		// Section to add a new key.
		if(ImGui::Button("Add")){
			// Read arguments.
			char** outPaths = nullptr;
			int outCount = 0;
			int res = sr_gui_ask_load_files("Select images", "", "png,jpg,jpeg", &outPaths, &outCount);
			System::forceLocale();
			if((res == SR_GUI_VALIDATED) && outPaths && outCount > 0){
				bool wasEmpty = _state.particles.imagePaths.empty();
				for (unsigned int i = 0; i < (unsigned int)outCount; ++i) {
					const std::string imageFilePath = std::string(outPaths[i]);
					_state.particles.imagePaths.push_back(imageFilePath);
				}
				// Ensure particles are zoomed in enough.
				if(wasEmpty && (_state.particles.scale <= 9.0f)) {
					_state.particles.scale = 10.0f;
				}
				refreshTextures = true;
			}
			for(unsigned int i = 0; i < (unsigned int)outCount; ++i){
				free(outPaths[i]);
			}
			free(outPaths);
		}

		// Actions
		if(!_showParticleEditor){
			// If we are exiting, refresh the existing set.
			refreshTextures = true;
		}

		// If refresh is needed, ensure that the texture array is up to date.
		if(refreshTextures){
			if (_state.particles.tex != ResourcesManager::getTextureFor("blankarray")) {
				glDeleteTextures(1, &_state.particles.tex);
			}
			glDeleteTextures(previewTextures.size(), previewTextures.data());
			previewTextures.clear();

			if(_state.particles.imagePaths.empty()){
				// Use a white square particle appearance by default.
				_state.particles.tex =  ResourcesManager::getTextureFor("blankarray");
				_state.particles.texCount = 1;
				_state.particles.scale = 1.f;

			} else {
				// Load new particles.
				_state.particles.tex = loadTextureArray(_state.particles.imagePaths, false, _state.particles.texCount);
				previewTextures = generate2DViewsOfArray(_state.particles.tex, thumbSize);
			}
		}
	}
	ImGui::End();
}

void Renderer::applyBackgroundColor(){
	// Clear all buffers with this color.
	glClearColor(_state.background.color[0], _state.background.color[1], _state.background.color[2], _useTransparency ? 0.0f : 1.0f);
	_particlesFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_particlesFramebuffer->unbind();
	_blurFramebuffer0->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_blurFramebuffer0->unbind();
	_blurFramebuffer1->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_blurFramebuffer1->unbind();
	// Update parameter.
	_blurringScreen.program().use();
	_blurringScreen.program().uniform("backgroundColor", _state.background.color);
	glUseProgram(0);
}

void Renderer::applyAllSettings() {
	// Apply all modifications.

	// One-shot parameters.
	_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_score->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_scene->setParticlesParameters(_state.particles.speed, _state.particles.expansion);
	_score->setDisplay(_state.background.digits, _state.background.hLines, _state.background.vLines);
	_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);
	_scene->setKeyboardSizeAndFadeout(_state.keyboard.size, _state.notes.fadeOut);
	_scene->setMinorEdgesAndHeight(_state.keyboard.minorEdges, _state.keyboard.minorHeight);
	_score->setKeyboardSize(_state.keyboard.size);
	_score->setPlayDirection(_state.reverseScroll);
	
	_score->setOrientation(_state.horizontalScroll);
	_scene->setOrientation(_state.horizontalScroll);

	updateMinMaxKeys();

	// Reset buffers.
	applyBackgroundColor();
	_blurringScreen.program().use();
	_blurringScreen.program().uniform("attenuationFactor", _state.attenuation);
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
	_fxaa.clean();
	_particlesFramebuffer->clean();
	_blurFramebuffer0->clean();
	_blurFramebuffer1->clean();
	_finalFramebuffer->clean();
	_renderFramebuffer->clean();
}

void Renderer::rescale(float scale){
	resizeAndRescale(_camera.screenSize()[0], _camera.screenSize()[1], scale);
}

void Renderer::resize(int width, int height) {
	resizeAndRescale(width, height, _camera.scale());
}

void Renderer::resizeAndRescale(int width, int height, float scale) {
	
	_backbufferSize[0] = width;
	_backbufferSize[1] = height;
	// Don't update the rendering buffers if the size is 0, to handle minimizing
	// or if we are currently recording.
	if(width == 0 || height == 0 || _recorder.isRecording()){
		return;
	}

	if (_verbose) {
		std::cout << "[INFO]: Resizing to " << width << " x " << height << std::endl;
	}
	// Update the projection matrix.
	_camera.screen(width, height, scale);
	updateSizes();
}

void Renderer::updateSizes(){
	// Resize the framebuffers.
	const auto &currentQuality = Quality::availables.at(_state.quality);
	const glm::vec2 baseRes(_camera.renderSize());
	_particlesFramebuffer->resize(currentQuality.particlesResolution * baseRes);
	_blurFramebuffer0->resize(currentQuality.blurResolution * baseRes);
	_blurFramebuffer1->resize(currentQuality.blurResolution * baseRes);
	_renderFramebuffer->resize(currentQuality.finalResolution * baseRes);
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
		else if (key == GLFW_KEY_ESCAPE){
			_shouldQuit = 1;
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
	_state.setOptions.rebuild();
	_backupSetOptions = _state.setOptions;
	_backupParticlesOptions = _state.particles;
	
	// Update toggles.
	_layers[Layer::BGTEXTURE].toggle = &_state.background.image;
	_layers[Layer::BLUR].toggle = &_state.showBlur;
	_layers[Layer::ANNOTATIONS].toggle = &_state.showScore;
	_layers[Layer::KEYBOARD].toggle = &_state.showKeyboard;
	_layers[Layer::PARTICLES].toggle = &_state.showParticles;
	_layers[Layer::NOTES].toggle = &_state.showNotes;
	_layers[Layer::FLASHES].toggle = &_state.showFlashes;
	_layers[Layer::PEDAL].toggle = &_state.showPedal;
	_layers[Layer::WAVE].toggle = &_state.showWave;

	// Update split notes.
	if(_scene){
		_scene->updateSets(_state.setOptions);
	}
	applyAllSettings();

	// Textures.
	// Load the background image.
	if(!_state.background.imagePath.empty()){
		glDeleteTextures(1, &_state.background.tex);
		_state.background.tex = loadTexture(_state.background.imagePath[0], 4, false);
		// Don't modify the rest of the potentially restored state.
	}
	// Load notes images
	if(!_state.notes.majorImagePath.empty()){
		glDeleteTextures(1, &_state.notes.majorTex);
		_state.notes.majorTex = loadTexture(_state.notes.majorImagePath[0], 4, false);
	}
	if(!_state.notes.minorImagePath.empty()){
		glDeleteTextures(1, &_state.notes.minorTex);
		_state.notes.minorTex = loadTexture(_state.notes.minorImagePath[0], 4, false);
	}

	if(!_state.particles.imagePaths.empty()){
		// Cleanup texture if it's not the default one.
		if (_state.particles.tex != ResourcesManager::getTextureFor("blankarray")) {
			glDeleteTextures(1, &_state.particles.tex);
		}
		// Load new particles.
		_state.particles.tex = loadTextureArray(state.particles.imagePaths, false, _state.particles.texCount);
		// Don't modify the rest of the potentially restored state.
	}
}

void  Renderer::setGUIScale(float scale){
	_guiScale = (std::max)(0.25f, scale);
	ImGui::GetStyle() = ImGuiStyle();
	ImGui::configureStyle();
	ImGui::GetIO().FontGlobalScale = _guiScale;
	ImGui::GetStyle().ScaleAllSizes(_guiScale);
	ImGui::GetStyle().FrameRounding = 3 * _guiScale;
}


void Renderer::updateConfiguration(Configuration& config){
	// Reset
	config.lastMidiPath = "";
	config.lastMidiDevice = "";
	// General settings
	config.fullscreen = _fullscreen;
	config.useTransparency = _useTransparency;
	config.guiScale = _guiScale;
	// Settings file.
	config.lastConfigPath = _state.filePath();
	// MIDI File.
	std::shared_ptr<MIDISceneFile> fileScene = std::dynamic_pointer_cast<MIDISceneFile>(_scene);
	if(fileScene){
		config.lastMidiPath = fileScene->filePath();
	}
	// MIDI device.
	std::shared_ptr<MIDISceneLive> liveScene = std::dynamic_pointer_cast<MIDISceneLive>(_scene);
	if(liveScene){
		config.lastMidiDevice = liveScene->deviceName();
	}
}

bool Renderer::startDirectRecording(const Export& exporting, const glm::vec2 & size){
	const bool success = _recorder.setParameters(exporting);
	if(!success){
		std::cerr << "[EXPORT]: Unable to start direct export." << std::endl;
		return false;
	}
	_recorder.setSize(size);
	startRecording();
	_exitAfterRecording = true;
	return true;
}

void Renderer::startRecording(){
	// We need to provide some information for the recorder to start.
	_recorder.prepare(_state.prerollTime, float(_scene->duration()), _state.scrollSpeed);

	// Start by clearing up all buffers.
	// We need:
	// - the rendering res (taking into account quality and screen scaling) to be equal to requiredSize().
	// - the camera screen size and scale to remain the same afterwards (and during for a nice background display).
	// We can do this by leveraging the fact that camera parameters are not used during render.
	// We can thus:
	// - backup the camera parameters and backbuffer resolution
	// - trigger a buffers size update at the target resolution
	// - restore the camera parameters and backbuffer resolution
	// All that will be left is to trigger a size update at the end of the recording.

	const glm::ivec2 backSize = _camera.screenSize();
	const float backScale = _camera.scale();
	const glm::ivec2 backBufferSize = _backbufferSize;

	const auto &currentQuality = Quality::availables.at(_state.quality);
	const glm::vec2 finalSize = glm::vec2(_recorder.requiredSize()) / currentQuality.finalResolution;
	resizeAndRescale(int(finalSize[0]), int(finalSize[1]), 1.0f);

	_camera.screen(backSize[0], backSize[1], backScale);
	_backbufferSize = backBufferSize;

	// Reset buffers.
	glClearColor(_state.background.color[0], _state.background.color[1], _state.background.color[2], _recorder.isTransparent() ? 0.0f : 1.0f);
	_particlesFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_blurFramebuffer0->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_blurFramebuffer1->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_renderFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_finalFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_finalFramebuffer->unbind();

	_recorder.start(_verbose);
}

bool Renderer::channelColorEdit(const char * name, const char * displayName, ColorArray & colors){
	if(!_state.perSetColors){
		// If locked, display color sink.
		ImGuiPushItemWidth(25);
		const bool inter = ImGui::ColorEdit3(name, &colors[0][0], ImGuiColorEditFlags_NoInputs);
		ImGui::PopItemWidth();

		if(_state.lockParticleColor && ImGui::IsItemHovered()){
			ImGui::SetTooltip("(!) Effect colors\n    are synced");
		}

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
	ImGuiSameLine(); ImGui::Text("%s", displayName);

	if(ImGui::BeginPopup(name)){
		// Do 3 columns of color sinks.
		bool edit = false;
		ImGuiPushItemWidth(35);
		for(size_t cid = 0; cid < colors.size(); ++cid){
			const std::string nameC = "Set " + std::to_string(cid);
			edit = ImGui::ColorEdit3(nameC.c_str(), &colors[cid][0], ImGuiColorEditFlags_NoInputs) || edit;
			if(cid % 3 != 2 && cid != colors.size()-1){
				ImGuiSameLine(75 * (cid%3+1));
			}
		}
		ImGui::PopItemWidth();
		ImGui::EndPopup();
		return edit;
	}
	return false;
}


void Renderer::updateMinMaxKeys(){
	// Make sure keys are properly ordered.
	if(_state.minKey > _state.maxKey){
		std::swap(_state.minKey, _state.maxKey);
	}

	// Force edges to align with a major key.
	int realMinKey = _state.minKey;
	if(noteIsMinor[_state.minKey % 12]){
		realMinKey -= 1;
	}

	int realMaxKey = _state.maxKey;
	if(noteIsMinor[_state.maxKey % 12]){
		realMaxKey += 1;
	}

	// Convert to "major" only indices.
	const int minKeyMaj = (_state.minKey/12) * 7 + noteShift[realMinKey % 12];
	const int maxKeyMaj = (_state.maxKey/12) * 7 + noteShift[realMaxKey % 12];
	const int noteCount = (maxKeyMaj - minKeyMaj + 1);

	_scene->setMinMaxKeys(realMinKey, minKeyMaj, noteCount);
	_score->setMinMaxKeys(realMinKey, minKeyMaj, noteCount);
}


void Renderer::ImGuiPushItemWidth(int w){
	ImGui::PushItemWidth(_guiScale * w);
}

void Renderer::ImGuiSameLine(int w){
	ImGui::SameLine(_guiScale * w);
}
