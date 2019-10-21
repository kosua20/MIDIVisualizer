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

	_blurringScreen.init(
		_particlesFramebuffer->textureId(),
		ResourcesManager::getStringForShader("particlesblur_frag"));
	_blurryScreen.init(_blurFramebuffer->textureId(),
		ResourcesManager::getStringForShader("screenquad_frag"));
	_finalScreen.init(_finalFramebuffer->textureId(),
		ResourcesManager::getStringForShader("screenquad_frag"));

	_state.reset();

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
	_background = std::make_shared<Background>(
		_scene->midiFile().tracks[0].secondsPerMeasure);

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

	const glm::vec2 invSizeFb = glm::vec2(1.0f / _finalFramebuffer->_width,
		1.0f / _finalFramebuffer->_height);

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

	// Background texture.
	// TODO

	// Draw the blurred particles.
	if (_state.showBlur) {
		drawBlur(invSizeFb);
	}
	// Draw the particles.
	if (_state.showParticles) {
		drawParticles(invSizeFb);
	}
	// Draw the keys, grid, and measure numbers.
	drawBackground(invSizeFb);

	if (_state.background.keys) {
		drawKeyboard(invSizeFb);
	}
	// Draw the notes.
	if (_state.showNotes) {
		drawNotes(invSizeFb);
	}

	if (_state.showFlashes) {
		// Draw the flashes.
		drawFlashes(invSizeFb);
	}
	_finalFramebuffer->unbind();

	glViewport(0, 0, GLsizei(_camera._screenSize[0]), GLsizei(_camera._screenSize[1]));
	_finalScreen.draw(_timer, {});

	if (_showGUI) {
		drawGUI(currentTime);
	}
}

void Renderer::blurPrepass() {
	const glm::vec2 invSizeB = 1.0f / glm::vec2(_particlesFramebuffer->_width, _particlesFramebuffer->_height);
	// Bind particles buffer.
	_particlesFramebuffer->bind();
	// Set viewport.
	glViewport(0, 0, _particlesFramebuffer->_width,
		_particlesFramebuffer->_height);
	// Draw blurred particles from previous frames.
	_blurryScreen.draw(_timer);
	if (_state.showParticles) {
		// Draw the new particles.
		_scene->drawParticles(_timer, invSizeB, _state.particles.color,
			_state.particles.scale, _state.particles.tex, _state.particles.texCount,
			_state.particles.count, true);
	}
	if (_state.showBlurNotes) {
		// Draw the notes.
		_scene->draw(_timer, invSizeB, _state.baseColor, _state.minorColor, true);
	}

	_particlesFramebuffer->unbind();

	// Bind blur framebuffer.
	_blurFramebuffer->bind();
	glViewport(0, 0, _blurFramebuffer->_width, _blurFramebuffer->_height);
	// Perform box blur on result from particles pass.
	_blurringScreen.draw(_timer);
	_blurFramebuffer->unbind();

}

void Renderer::drawBlur(const glm::vec2 &  ) {
	glEnable(GL_BLEND);
	_blurryScreen.draw(_timer);
	glDisable(GL_BLEND);
}

void Renderer::drawParticles(const glm::vec2 & invSize) {
	_scene->drawParticles(_timer, invSize, _state.particles.color,
		_state.particles.scale, _state.particles.tex, _state.particles.texCount,
		_state.particles.count, false);
}

void Renderer::drawBackground(const glm::vec2 & invSize) {
	_background->draw(_timer, invSize);
}

void Renderer::drawKeyboard(const glm::vec2 & invSize) {
	_scene->drawKeyboard(_timer, invSize, _state.background.keysColor);
}

void Renderer::drawNotes(const glm::vec2 & invSize) {
	_scene->draw(_timer, invSize, _state.baseColor, _state.minorColor, false);
}

void Renderer::drawFlashes(const glm::vec2 & invSize) {
	_scene->drawFlashes(_timer, invSize, _state.flashColor, _state.flashSize);
}

void Renderer::drawGUI(const float currentTime) {
	// ImGui::ShowTestWindow();

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
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			const std::string versionString = std::string("MIDIVisualizer v") +
				std::to_string(MIDIVIZ_VERSION_MAJOR) +
				"." +
				std::to_string(MIDIVIZ_VERSION_MINOR);
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

		ImGui::PushItemWidth(100);
		ImGui::SameLine(160);
		if (ImGui::InputFloat("Preroll", &_state.prerollTime, 0.1f, 1.0f)) {
			reset();
		}
		ImGui::PopItemWidth();

		ImGui::PushItemWidth(80);
		if (ImGui::Combo("Quality", (int *)(&_state.quality),
			"Half\0Low\0Medium\0High\0Double\0\0")) {
			resize(int(_camera._screenSize[0]), int(_camera._screenSize[1]));
		}
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(100);
		ImGui::SameLine(160);
		const bool smw0 = ImGui::InputFloat("Scale", &_state.scale, 0.01f, 0.1f);
		ImGui::PopItemWidth();

		bool colNotesEdit = ImGui::ColorEdit3("Notes", &_state.baseColor[0],
			ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine(80);
		bool colMinorsEdit = ImGui::ColorEdit3("Minors", &_state.minorColor[0],
			ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine(160);
		bool colPartsEdit = ImGui::ColorEdit3("Effects", &_state.particles.color[0],
			ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine(240);
		bool colFlashesEdit = ImGui::ColorEdit3("Flashes", &_state.flashColor[0],
			ImGuiColorEditFlags_NoInputs);

		if (ImGui::Checkbox("Lock colors", &_state.lockParticleColor)) {
			// If we enable the lock, make sure the colors are synched.
			colNotesEdit = true;
		}
		ImGui::SameLine(160);
		ImGui::Checkbox("Blur", &_state.showBlur);
		if (_state.showBlur) {
			ImGui::Checkbox("Blur notes", &_state.showBlurNotes);
			ImGui::SameLine(160);
			ImGui::PushItemWidth(86);
			if (ImGui::SliderFloat("Attenuation", &_state.attenuation, 0.0f, 1.0f)) {
				_state.attenuation = std::min(1.0f, std::max(0.0f, _state.attenuation));
				glUseProgram(_blurringScreen.programId());
				const GLuint id1 = glGetUniformLocation(_blurringScreen.programId(),
					"attenuationFactor");
				glUniform1f(id1, _state.attenuation);
				glUseProgram(0);
			}
			ImGui::PopItemWidth();
		}

		ImGui::Checkbox("Notes", &_state.showNotes);
		ImGui::SameLine(160);
		ImGui::Checkbox("Particles", &_state.showParticles);
		ImGui::Checkbox("Flashes", &_state.showFlashes);
		ImGui::SameLine(160);
		ImGui::PushItemWidth(86);
		ImGui::SliderFloat("Flash size", &_state.flashSize, 0.1f, 3.0f);
		ImGui::PopItemWidth();

		bool smw1 = false;

		if (ImGui::CollapsingHeader("Background##HEADER",
			ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::ColorEdit3("Color##Background", &_state.background.color[0],
				ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine(80);
			bool cbg0 = ImGui::ColorEdit3("Lines##Background",
				&_state.background.linesColor[0],
				ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine(160);
			bool cbg1 =
				ImGui::ColorEdit3("Text##Background", &_state.background.textColor[0],
					ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine(240);
			bool cbg2 =
				ImGui::ColorEdit3("Keys##Background", &_state.background.keysColor[0],
					ImGuiColorEditFlags_NoInputs);

			bool m2 = ImGui::Checkbox("Horizontal lines", &_state.background.hLines);
			ImGui::SameLine(160);
			bool m3 = ImGui::Checkbox("Vertical lines", &_state.background.vLines);
			bool m4 = ImGui::Checkbox("Keyboard", &_state.background.keys);
			ImGui::SameLine(160);
			bool m1 = ImGui::Checkbox("Digits", &_state.background.digits);
			ImGui::PushItemWidth(145);
			smw1 =
				ImGui::InputFloat("Minor keys size", &_state.background.minorsWidth,
					0.1f, 1.0f, "%.2f");
			ImGui::PopItemWidth();

			if (m1 || m2 || m3 || m4) {
				_background->setDisplay(
					_state.background.digits, _state.background.hLines,
					_state.background.vLines);
			}

			if (cbg0 || cbg1 || cbg2) {
				_background->setColors(_state.background.linesColor,
					_state.background.textColor,
					_state.background.keysColor);
			}
		}

		if (_state.showParticles) {
			if (ImGui::CollapsingHeader("Particles##HEADER")) {

				ImGui::PushID("ParticlesSettings");

				ImGui::PushItemWidth(100);
				if (ImGui::SliderInt("Count", &_state.particles.count, 1, 512)) {
					_state.particles.count =
						std::min(std::max(_state.particles.count, 1), 512);
				}
				ImGui::SameLine(160);
				if (ImGui::InputFloat("Size", &_state.particles.scale, 1.0f, 10.0f)) {
					_state.particles.scale = std::max(1.0f, _state.particles.scale);
				}

				const bool mp0 =
					ImGui::InputFloat("Speed", &_state.particles.speed, 0.001f, 1.0f);
				ImGui::SameLine(160);
				const bool mp1 = ImGui::InputFloat(
					"Expansion", &_state.particles.expansion, 0.1f, 5.0f);
				if (mp1 || mp0) {
					_scene->setParticlesParameters(_state.particles.speed,
						_state.particles.expansion);
				}

				if (ImGui::Button("Default image")) {
					// Use a white square particle appearance by default.
					const GLuint blankID = ResourcesManager::getTextureFor("blankarray");
					_state.particles.tex = blankID;
					_state.particles.texCount = 1;
					_state.particles.scale = 1.0f;
				}
				ImGui::SameLine(160);
				if (ImGui::Button("Load images...")) {
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
					ImGui::TextUnformatted(
						"You can select multiple images (PNG or JPEG). They should be "
						"square and greyscale, where black is transparent, white opaque.");
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}

				ImGui::PopID();
				ImGui::PopItemWidth();
			}
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
			_state.background.minorsWidth =
				std::min(std::max(_state.background.minorsWidth, 0.1f), 1.0f);
			_scene->setScaleAndMinorWidth(_state.scale,
				_state.background.minorsWidth);
			_background->setScaleAndMinorWidth(_state.scale,
				_state.background.minorsWidth);
		}

		// Keep the colors in sync if needed.
		if (_state.lockParticleColor &&
			(colNotesEdit || colPartsEdit || colMinorsEdit || colFlashesEdit)) {
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
			_state.baseColor = _state.particles.color = _state.minorColor =
				_state.flashColor = refColor;
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
			ImGui::Text("%.1f FPS / %.1f ms", ImGui::GetIO().Framerate,
				ImGui::GetIO().DeltaTime * 1000.0f);
			if (ImGui::Button("Print MIDI content to console")) {
				_scene->midiFile().printTracks();
			}
		}
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
	GLubyte *data =
		new GLubyte[_finalFramebuffer->_width * _finalFramebuffer->_height * 3];
	// Generate and save frames.
	int framesCount =
		int(std::ceil((_scene->duration() + 10.0f + _state.prerollTime) * frameRate));
	int targetSize = int(std::to_string(framesCount).size());

	// Start by clearing up the blur and particles buffers.
	resize(int(_camera._screenSize[0]), int(_camera._screenSize[1]));

	std::cout << "[EXPORT]: Will export " << framesCount << " frames to \""
		<< outputDirPath << "\"." << std::endl;
	for (size_t fid = 0; fid < framesCount; ++fid) {
		std::cout << "\r[EXPORT]: Processing frame " << (fid + 1) << "/"
			<< framesCount << "." << std::flush;
		// Render.
		draw(_timer);
		glFinish();
		glFlush();
		// Readback.
		_finalFramebuffer->bind();
		glReadPixels(0, 0, (GLsizei)_finalFramebuffer->_width,
			(GLsizei)_finalFramebuffer->_height, GL_RGB, GL_UNSIGNED_BYTE,
			&data[0]);
		_finalFramebuffer->unbind();
		// Write to disk.
		std::string intString = std::to_string(fid);
		while (intString.size() < targetSize) {
			intString = "0" + intString;
		}
		const std::string outputFilePath =
			outputDirPath + "/output_" + intString + ".png";

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
		unsigned error = lodepng_encode_file(
			outputFilePath.c_str(), data, _finalFramebuffer->_width,
			_finalFramebuffer->_height, LCT_RGB, 8);

		/*if there's an error, display it*/
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
	_background->setScaleAndMinorWidth(_state.scale,
		_state.background.minorsWidth);
	_scene->setParticlesParameters(_state.particles.speed,
		_state.particles.expansion);
	_background->setDisplay(_state.background.digits, _state.background.hLines,
		_state.background.vLines);
	_background->setColors(_state.background.linesColor,
		_state.background.textColor,
		_state.background.keysColor);

	// Reset buffers.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	_particlesFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_particlesFramebuffer->unbind();
	_blurFramebuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	_blurFramebuffer->unbind();
	glUseProgram(_blurringScreen.programId());
	GLuint id2 =
		glGetUniformLocation(_blurringScreen.programId(), "attenuationFactor");
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
	_background->clean();
	_blurringScreen.clean();
	_blurryScreen.clean();
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
	_particlesFramebuffer->resize(currentQuality.particlesResolution *
		_camera._screenSize);
	_blurFramebuffer->resize(currentQuality.blurResolution * _camera._screenSize);
	_finalFramebuffer->resize(currentQuality.finalResolution *
		_camera._screenSize);
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
