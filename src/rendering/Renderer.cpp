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

#include <lodepng/lodepng.h>

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::init(int width, int height) {
	_showGUI = true;
	_showDebug = false;
	_exportFramerate = 60;
	_performExport = 0;

	ResourcesManager::loadResources();

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
	// Setup projection matrix.
	_camera.screen(width, height);

	// Setup framebuffers.
	_particlesFramebuffer = std::shared_ptr<Framebuffer>(
		new Framebuffer(int(_camera._screenSize[0]), int(_camera._screenSize[1]), GL_RGBA,
			GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_blurFramebuffer = std::shared_ptr<Framebuffer>(
		new Framebuffer(int(_camera._screenSize[0]), int(_camera._screenSize[1]), GL_RGBA,
			GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));
	_finalFramebuffer = std::shared_ptr<Framebuffer>(
		new Framebuffer(int(_camera._screenSize[0]), int(_camera._screenSize[1]), GL_RGB,
			GL_UNSIGNED_BYTE, GL_LINEAR, GL_CLAMP_TO_EDGE));

	_backgroundTexture.init("backgroundtexture_frag", "backgroundtexture_vert");
	_blurringScreen.init(_particlesFramebuffer->textureId(), "particlesblur_frag");
	_passthrough.init("screenquad_frag");

	_state.reset();

	// Create the layers.
	//_layers[Layer::BGCOLOR].type = Layer::BGCOLOR;
	//_layers[Layer::BGCOLOR].name = "Background color";
	//_layers[Layer::BGCOLOR].toggle = &_state.showBackground;

	_layers[Layer::BGTEXTURE].type = Layer::BGTEXTURE;
	_layers[Layer::BGTEXTURE].name = "Background image";
	_layers[Layer::BGTEXTURE].draw = &Renderer::drawBackgroundImage;
	_layers[Layer::BGTEXTURE].toggle = &_state.background.image;

	_layers[Layer::BLUR].type = Layer::BLUR;
	_layers[Layer::BLUR].name = "Blur effect";
	_layers[Layer::BLUR].draw = &Renderer::drawBlur;
	_layers[Layer::BLUR].toggle = &_state.showBlur;

	_layers[Layer::ANNOTATIONS].type = Layer::ANNOTATIONS;
	_layers[Layer::ANNOTATIONS].name = "Score";
	_layers[Layer::ANNOTATIONS].draw = &Renderer::drawScore;
	_layers[Layer::ANNOTATIONS].toggle = &_state.showScore;

	_layers[Layer::KEYBOARD].type = Layer::KEYBOARD;
	_layers[Layer::KEYBOARD].name = "Keyboard";
	_layers[Layer::KEYBOARD].draw = &Renderer::drawKeyboard;
	_layers[Layer::KEYBOARD].toggle = &_state.background.keys;

	_layers[Layer::PARTICLES].type = Layer::PARTICLES;
	_layers[Layer::PARTICLES].name = "Particles";
	_layers[Layer::PARTICLES].draw = &Renderer::drawParticles;
	_layers[Layer::PARTICLES].toggle = &_state.showParticles;

	_layers[Layer::NOTES].type = Layer::NOTES;
	_layers[Layer::NOTES].name = "Notes";
	_layers[Layer::NOTES].draw = &Renderer::drawNotes;
	_layers[Layer::NOTES].toggle = &_state.showNotes;

	_layers[Layer::FLASHES].type = Layer::FLASHES;
	_layers[Layer::FLASHES].name = "Flashes";
	_layers[Layer::FLASHES].draw = &Renderer::drawFlashes;
	_layers[Layer::FLASHES].toggle = &_state.showFlashes;

	// Check setup errors.
	checkGLError();
}

void Renderer::setColorAndScale(const glm::vec3 &baseColor, const float scale) {
	_state.scale = scale;
	_state.baseColor = baseColor;
	_state.minorColor = baseColor;
	_state.flashColor = baseColor;
	_state.particles.color = _state.baseColor;
}

void Renderer::loadFile(const std::string &midiFilePath) {
	// Player.
	_timer = -_state.prerollTime;
	_shouldPlay = false;

	// Init objects.
	_scene = std::make_shared<MIDIScene>(midiFilePath);
	_score = std::make_shared<Score>(_scene->midiFile().tracks[0].secondsPerMeasure);

	applyAllSettings();
}

void Renderer::draw(const float currentTime) {

	if (_performExport > 0) {
		// Let a bit of time at ImGui to display the modal message.
		if (_performExport < 10) {
			// Spawn the popup on first frame.
			if (_performExport == 1) {
				ImGui::OpenPopup("Exporting...");
			}
			// Pop up details.
			if (ImGui::BeginPopupModal("Exporting...", NULL,
				ImGuiWindowFlags_AlwaysAutoResize)) {
				const int framesCount =
					int(std::ceil((_scene->duration() + 10.0) * float(_exportFramerate)));
				ImGui::Text("Scene duration: %ds. (+10s. buffer).",
					int(std::round(_scene->duration())));
				ImGui::Text("Framerate: %d fps.", _exportFramerate);
				ImGui::Text("Destination directory: %s", _exportPath.c_str());
				ImGui::Text("Exporting %d frames at resolution %dx%d...", framesCount,
					_finalFramebuffer->_width, _finalFramebuffer->_height);
				ImGui::TextDisabled("For more info and real-time progress, please look "
					"at the console log.");

				ImGui::EndPopup();
			}
			// Next "frame".
			_performExport += 1;
			return;

		}
		else {
			// After the popup animation, start the real export.
			_performExport = 0;
			renderFile(_exportPath, float(_exportFramerate));
		}
	}

	// Compute the time elapsed since last frame, or keep the same value if
	// playback is disabled.
	_timer = _shouldPlay ? (currentTime - _timerStart) : _timer;

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
	glClearColor(_state.background.color[0], _state.background.color[1], _state.background.color[2], 1.0f);
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

	glViewport(0, 0, GLsizei(_camera._screenSize[0]), GLsizei(_camera._screenSize[1]));
	_passthrough.draw(_finalFramebuffer->textureId(), _timer);

	if (_showGUI) {
		drawGUI(currentTime);
	}
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
		_scene->drawNotes(_timer, invSizeB, _state.baseColor, _state.minorColor, true);
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
	_scene->drawKeyboard(_timer, invSize, _state.background.keysColor, _state.baseColor, _state.minorColor, _state.highlightKeys);
}

void Renderer::drawNotes(const glm::vec2 & invSize) {
	_scene->drawNotes(_timer, invSize, _state.baseColor, _state.minorColor, false);
}

void Renderer::drawFlashes(const glm::vec2 & invSize) {
	_scene->drawFlashes(_timer, invSize, _state.flashColor, _state.flashSize);
}

void Renderer::drawGUI(const float currentTime) {
	
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
		
		ImGui::SameLine(320);
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
		ImGui::SameLine(160);
		ImGui::PushItemWidth(100);
		if (ImGui::InputFloat("Preroll", &_state.prerollTime, 0.1f, 1.0f)) {
			reset();
		}
		ImGui::PopItemWidth();

		if (ImGui::Button("Show layers...")) {
			_showLayers = true;
		}
		ImGui::SameLine(160);
		ImGui::PushItemWidth(100);
		if (ImGui::Combo("Quality", (int *)(&_state.quality), "Half\0Low\0Medium\0High\0Double\0\0")) {
			resize(int(_camera._screenSize[0]), int(_camera._screenSize[1]));
		}

		const bool smw0 = ImGui::InputFloat("Scale", &_state.scale, 0.01f, 0.1f);
		ImGui::SameLine(160);
		bool smw1 = ImGui::SliderFloat("Minor size", &_state.background.minorsWidth, 0.1f, 1.0f, "%.2f");
		
		ImGui::PopItemWidth();

		ImGui::PushItemWidth(25);
		bool colNotesEdit = ImGui::ColorEdit3("Notes", &_state.baseColor[0],
			ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		bool colMinorsEdit = ImGui::ColorEdit3("Minors", &_state.minorColor[0],
			ImGuiColorEditFlags_NoInputs);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Checkbox("Sync colors", &_state.lockParticleColor)) {
			// If we enable the lock, make sure the colors are synched.
			colNotesEdit = true;
		}

		bool colFlashesEdit = false;
		if (_state.showFlashes && ImGui::CollapsingHeader("Flashes##HEADER")) {
			ImGui::PushItemWidth(25);
			colFlashesEdit = ImGui::ColorEdit3("Color##Flashes", &_state.flashColor[0],	ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::SameLine(160);
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("Flash size", &_state.flashSize, 0.1f, 3.0f);
			ImGui::PopItemWidth();
		}

		bool colPartsEdit = false;
		if (_state.showParticles && ImGui::CollapsingHeader("Particles##HEADER")) {

			ImGui::PushID("ParticlesSettings");
			ImGui::PushItemWidth(25);
			colPartsEdit = ImGui::ColorEdit3("Color##Particles", &_state.particles.color[0], ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::SameLine(160);

			ImGui::PushItemWidth(100);
			if (ImGui::InputFloat("Size", &_state.particles.scale, 1.0f, 10.0f)) {
				_state.particles.scale = std::max(1.0f, _state.particles.scale);
			}
			ImGui::PushItemWidth(150);

			if (ImGui::SliderInt("Count", &_state.particles.count, 1, 512)) {
				_state.particles.count = std::min(std::max(_state.particles.count, 1), 512);
			}
			ImGui::PopItemWidth();

			const bool mp0 = ImGui::InputFloat("Speed", &_state.particles.speed, 0.001f, 1.0f);
			ImGui::SameLine(160);
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

			ImGui::SameLine(160);
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

		
		if (_state.background.keys && ImGui::CollapsingHeader("Keyboard##HEADER")) {
			ImGui::PushItemWidth(25);
			const bool cbg2 = ImGui::ColorEdit3("Color##Keys", &_state.background.keysColor[0], ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::SameLine(160);
			ImGui::Checkbox("Highlight keys", &_state.highlightKeys);

			if (cbg2) {
				_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);
			}
		}

		if (_state.showScore && ImGui::CollapsingHeader("Score##HEADER")) {
			ImGui::PushItemWidth(25);
			const bool cbg0 = ImGui::ColorEdit3("Lines##Background", &_state.background.linesColor[0], ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			const bool cbg1 = ImGui::ColorEdit3("Text##Background", &_state.background.textColor[0], ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::SameLine(160);
			const bool m1 = ImGui::Checkbox("Digits", &_state.background.digits);
			const bool m2 = ImGui::Checkbox("Horizontal lines", &_state.background.hLines);
			ImGui::SameLine(160);
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
			ImGui::SameLine(160);
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
			ImGui::SameLine(160);
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
			ImGui::SameLine(160);
			if (ImGui::Button("Clear image##Background")) {
				_state.background.image = false;
				glDeleteTextures(1, &_state.background.tex);
				_state.background.tex = 0;
			}
			ImGui::Checkbox("Image extends under keyboard", &_state.background.imageBehindKeyboard);

		}
		ImGui::Separator();

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
				applyAllSettings();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset##config")) {
			_state.reset();
			applyAllSettings();
		}

		if (smw0 || smw1) {
			_state.scale = std::max(_state.scale, 0.01f);
			_state.background.minorsWidth = std::min(std::max(_state.background.minorsWidth, 0.1f), 1.0f);
			_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
			_score->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
		}

		// Keep the colors in sync if needed.
		if (_state.lockParticleColor && (colNotesEdit || colPartsEdit || colMinorsEdit || colFlashesEdit)) {
			glm::vec3 refColor = _state.baseColor;
			if (colPartsEdit) {
				refColor = _state.particles.color;
			}
			else if (colMinorsEdit) {
				refColor = _state.minorColor;
			}
			else if (colFlashesEdit) {
				refColor = _state.flashColor;
			}
			_state.baseColor = _state.particles.color = _state.minorColor = _state.flashColor = refColor;
		}

		ImGui::Separator();
		if (ImGui::Button("Render offline...")) {
			// Read arguments.
			nfdchar_t *outPath = NULL;
			nfdresult_t result = NFD_PickFolder(NULL, &outPath);
			if (result == NFD_OKAY) {
				_exportPath = std::string(outPath);
				_performExport = int(!_exportPath.empty());
			}
		}
		ImGui::SameLine(160);
		ImGui::PushItemWidth(100);
		ImGui::InputInt("Rate", &_exportFramerate);
		ImGui::PopItemWidth();

		if (_showDebug) {
			ImGui::Separator();
			ImGui::Text("Debug: ");
			ImGui::SameLine();
			ImGui::TextDisabled("(press D to hide)");
			ImGui::Text("%.1f FPS / %.1f ms", ImGui::GetIO().Framerate, ImGui::GetIO().DeltaTime * 1000.0f);
			if (ImGui::Button("Print MIDI content to console")) {
				_scene->midiFile().printTracks();
			}
		}
	}
	ImGui::End();

	if (_showLayers) {
		showLayers();
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
			ImGui::SameLine();
			ImGui::Selectable(layer.name.c_str());

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::Text(layer.name.c_str());
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

void Renderer::renderFile(const std::string &outputDirPath,
	const float frameRate) {
	_showGUI = false;
	// Reset.
	_timer = -_state.prerollTime;
	_timerStart = 0;
	// Start playing.
	_shouldPlay = true;
	// Image writing setup.
	GLubyte *data = new GLubyte[_finalFramebuffer->_width * _finalFramebuffer->_height * 3];
	// Generate and save frames.
	int framesCount = int(std::ceil((_scene->duration() + 10.0f + _state.prerollTime) * frameRate));
	int targetSize = int(std::to_string(framesCount).size());

	// Start by clearing up the blur and particles buffers.
	resize(int(_camera._screenSize[0]), int(_camera._screenSize[1]));

	std::cout << "[EXPORT]: Will export " << framesCount << " frames to \"" << outputDirPath << "\"." << std::endl;
	for (size_t fid = 0; fid < framesCount; ++fid) {
		std::cout << "\r[EXPORT]: Processing frame " << (fid + 1) << "/" << framesCount << "." << std::flush;
		// Render.
		draw(_timer);
		glFinish();
		glFlush();
		// Readback.
		_finalFramebuffer->bind();
		glReadPixels(0, 0, (GLsizei)_finalFramebuffer->_width, (GLsizei)_finalFramebuffer->_height, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
		_finalFramebuffer->unbind();
		// Write to disk.
		std::string intString = std::to_string(fid);
		while (intString.size() < targetSize) {
			intString = "0" + intString;
		}
		const std::string outputFilePath = outputDirPath + "/output_" + intString + ".png";

		int width = _finalFramebuffer->_width;
		int height = _finalFramebuffer->_height;
		char rgb[3];

		for (int y = 0; y < height / 2; ++y) {
			for (int x = 0; x < width; ++x) {
				int top = (x + y * width) * 3;
				int bottom = (x + (height - y - 1) * width) * 3;

				memcpy(rgb, data + top, sizeof(rgb));
				memcpy(data + top, data + bottom, sizeof(rgb));
				memcpy(data + bottom, rgb, sizeof(rgb));
			}
		}
		unsigned error = lodepng_encode_file( outputFilePath.c_str(), data, _finalFramebuffer->_width, _finalFramebuffer->_height, LCT_RGB, 8);
		if (error) {
			printf("error %u: %s\n", error, lodepng_error_text(error));
		}

		_timer += (1.0f / frameRate);
	}
	std::cout << std::endl;
	std::cout << "[EXPORT]: Done." << std::endl;

	_showGUI = true;
	_shouldPlay = false;
	_timer = -_state.prerollTime;
	_exportPath = "";
}

void Renderer::applyAllSettings() {
	// Apply all modifications.

	// One-shot parameters.
	_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_score->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_scene->setParticlesParameters(_state.particles.speed, _state.particles.expansion);
	_score->setDisplay(_state.background.digits, _state.background.hLines, _state.background.vLines);
	_score->setColors(_state.background.linesColor, _state.background.textColor, _state.background.keysColor);

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
	resize(int(_camera._screenSize[0]), int(_camera._screenSize[1]));

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

void Renderer::resize(int width, int height) {

	// Update the size of the viewport.
	glViewport(0, 0, width, height);
	// Update the projection matrix.
	_camera.screen(width, height);
	// Resize the framebuffers.
	const auto &currentQuality = Quality::availables.at(_state.quality);
	_particlesFramebuffer->resize(currentQuality.particlesResolution * _camera._screenSize);
	_blurFramebuffer->resize(currentQuality.blurResolution * _camera._screenSize);
	_finalFramebuffer->resize(currentQuality.finalResolution * _camera._screenSize);
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
