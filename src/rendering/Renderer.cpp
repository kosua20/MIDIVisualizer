#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>
#include <nfd/nfd.h>
#include <imgui/imgui.h>
#include "../helpers/ProgramUtilities.h"
#include "../helpers/ResourcesManager.h"

#include "Renderer.h"
#include <algorithm>


Renderer::Renderer(){ }

Renderer::~Renderer(){}

void Renderer::init(int width, int height){
	_showGUI = true;
	
	ResourcesManager::loadResources();
	
	// GL options
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Setup projection matrix.
	_camera.screen(width, height);
	
	// Setup framebuffers.
	_particlesFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(_camera._screenSize[0],_camera._screenSize[1], GL_RGBA,GL_UNSIGNED_BYTE,GL_LINEAR,GL_CLAMP_TO_EDGE));
	_blurFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(_camera._screenSize[0],_camera._screenSize[1], GL_RGBA,GL_UNSIGNED_BYTE,GL_LINEAR,GL_CLAMP_TO_EDGE));
	
	_blurringScreen.init(_particlesFramebuffer->textureId(), ResourcesManager::getStringForShader("particlesblur_frag"));
	_blurryScreen.init(_blurFramebuffer->textureId(), ResourcesManager::getStringForShader("screenquad_frag"));
	const GLuint blankID = ResourcesManager::getTextureFor("blank");
	_state.particles.texs = std::vector<GLuint>(PARTICLES_TEXTURE_COUNT, blankID);
	// Check setup errors.
	checkGLError();
}

void Renderer::setColorAndScale(const glm::vec3 & baseColor, const float scale){
	_state.scale = scale;
	_state.baseColor = baseColor;
	_state.particles.color = _state.baseColor;
}

void Renderer::loadFile(const std::string & midiFilePath){
	// Player.
	_timer = 0.0;
	_shouldPlay = false;

	// Init objects.
	_scene = std::make_shared<MIDIScene>(midiFilePath);
	_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_scene->setParticlesParameters(_state.particles.speed, _state.particles.expansion);
	
	_background = std::make_shared<Background>(_scene->midiFile().tracks[0].secondsPerMeasure);
	_background->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
	_background->setDisplay(_state.background.digits, _state.background.hLines, _state.background.vLines, _state.background.keys);
}


void Renderer::draw(){
	
	// Compute the time elapsed since last frame, or keep the same value if playback is disabled.
	_timer = _shouldPlay ? (glfwGetTime() - _timerStart) : _timer;
	
	// Update active notes listing (for particles).
	_scene->updatesActiveNotes(_timer);
	
	// Set viewport.
	glViewport(0,0,_camera._screenSize[0],_camera._screenSize[1]);
	
	if(_state.showBlur){
		// Bind particles buffer.
		_particlesFramebuffer->bind();
		
		// Draw blurred particles from previous frames.
		_blurryScreen.draw(_timer, 1.0f/ _camera._screenSize);
		if(_state.showParticles){
			// Draw the new particles.
			_scene->drawParticles(_timer, 1.0f/ _camera._screenSize, _state.particles.color, _state.particles.scale, _state.particles.texs, _state.particles.count, true);
		}
		if(_state.showBlurNotes){
			// Draw the notes.
			_scene->draw(_timer, 1.0f/ _camera._screenSize, _state.baseColor, true);
		}
		
		_particlesFramebuffer->unbind();
	
	
		// Bind blur framebuffer.
		_blurFramebuffer->bind();
		// Perform box blur on result from particles pass.
		_blurringScreen.draw(_timer, 1.0f/ _camera._screenSize);
		_blurFramebuffer->unbind();
	}
	
	// Final pass (directly on screen).
	glClear(GL_COLOR_BUFFER_BIT);
	// Draw the blurred particles.
	if(_state.showBlur){
		_blurryScreen.draw(_timer, 1.0f/ _camera._screenSize);
	}
	// Draw the particles.
	if(_state.showParticles){
		_scene->drawParticles(_timer, 1.0f/ _camera._screenSize, _state.particles.color, _state.particles.scale, _state.particles.texs, _state.particles.count, false);
	}
	// Draw the keys, grid, and measure numbers.
	_background->draw(_timer, 1.0f/ _camera._screenSize);
	// Draw the notes.
	_scene->draw(_timer, 1.0f/ _camera._screenSize, _state.baseColor, false);
	
	if(_state.showFlashes){
		// Draw the flashes.
		_scene->drawFlashes(_timer, 1.0f/ _camera._screenSize, _state.baseColor);
	}
	
	if(_showGUI){
		drawGUI();
	}
}

void Renderer::drawGUI(){
	//ImGui::ShowTestWindow();
	
	if(ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize)){
		
		if(ImGui::Button(_shouldPlay ? "Pause" : "Play")){
			_shouldPlay = !_shouldPlay;
			_timerStart = glfwGetTime() - _timer;
		}
		ImGui::SameLine();
		if(ImGui::Button("Reset")){
			_timer = 0;
			_timerStart = glfwGetTime();
		}
		
		ImGui::SameLine();
		ImGui::Button("Keys");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::Text("Keys:");
			ImGui::TextUnformatted("\tp: start/stop playing");
			ImGui::TextUnformatted("\tr: reset track");
			ImGui::Text("\ti: hide this panel");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		ImGui::Button("Infos");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("MIDIVisualizer v2.3");
			ImGui::TextUnformatted("Created by S. Rodriguez (kosua20)");
			ImGui::TextUnformatted("github.com/kosua20/MIDIVisualizer");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
		
		ImGui::Separator();
		
		if(ImGui::Button("Load MIDI file...")){
			// Read arguments.
			std::string midiFilePath;
			nfdchar_t *outPath = NULL;
			nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
			if(result == NFD_OKAY){
				midiFilePath = std::string(outPath);
				loadFile(midiFilePath);
			}
		}
		ImGui::SameLine(160);
		ImGui::PushItemWidth(100);
		
		const bool smw0 = ImGui::InputFloat("Scale", &_state.scale, 0.01f, 0.1f);
		ImGui::PopItemWidth();
		
		
		bool colNotesEdit = ImGui::ColorEdit3("Notes", &_state.baseColor[0], ImGuiColorEditFlags_NoInputs); ImGui::SameLine(80);
		bool colPartsEdit = ImGui::ColorEdit3("Effects", &_state.particles.color[0], ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine(160);
		if(ImGui::Checkbox("Lock colors", &_state.lockParticleColor)){
			// If we enable the lock, make sure the colors are synched.
			colNotesEdit = true;
		}
		
		ImGui::Checkbox("Particles", &_state.showParticles); ImGui::SameLine(160);
		ImGui::Checkbox("Flashes", &_state.showFlashes);
		
		ImGui::Checkbox("Blur", &_state.showBlur);
		if(_state.showBlur) {
			ImGui::SameLine(160);
			ImGui::Checkbox("Blur notes", &_state.showBlurNotes);
		}
		
		bool smw1 = false;
		
		if (ImGui::CollapsingHeader("Background##HEADER", ImGuiTreeNodeFlags_DefaultOpen)) {
			if(ImGui::ColorEdit3("Color##Background", &_state.background.color[0], ImGuiColorEditFlags_NoInputs)){
				glClearColor(_state.background.color[0],_state.background.color[1],_state.background.color[2], 1.0f);
				_particlesFramebuffer->bind();
				glClear(GL_COLOR_BUFFER_BIT);
				_particlesFramebuffer->unbind();
				_blurFramebuffer->bind();
				glClear(GL_COLOR_BUFFER_BIT);
				_blurFramebuffer->unbind();
				glUseProgram(_blurringScreen.programId());
				GLuint id1 = glGetUniformLocation(_blurringScreen.programId(), "backgroundColor");
				glUniform3fv(id1, 1, &_state.background.color[0]);
				glUseProgram(0);
			}
			ImGui::SameLine(120);
			ImGui::PushItemWidth(80);
			smw1 = ImGui::InputFloat("Minor keys size", &_state.background.minorsWidth, 0.1f, 1.0f, "%.2f");
			ImGui::PopItemWidth();
			bool m2 = ImGui::Checkbox("Horizontal lines", &_state.background.hLines);  ImGui::SameLine(160);
			bool m3 = ImGui::Checkbox("Vertical lines", &_state.background.vLines);
			bool m4 = ImGui::Checkbox("Keyboard", &_state.background.keys); ImGui::SameLine(160);
			bool m1 = ImGui::Checkbox("Digits", &_state.background.digits);
			
			
			if(m1 || m2 || m3 || m4){
				_background->setDisplay(_state.background.digits, _state.background.hLines, _state.background.vLines, _state.background.keys);
			}
		}
		
		if(_state.showParticles){
			if (ImGui::CollapsingHeader("Particles##HEADER")) {
			
			
				ImGui::PushID("ParticlesSettings");
				
				
				ImGui::PushItemWidth(100);
				ImGui::SliderInt("Count", &_state.particles.count, 1, 512);
				ImGui::SameLine(160);
				if(ImGui::InputFloat("Size", &_state.particles.scale, 1.0f, 10.0f)){
					_state.particles.scale = std::max(1.0f, _state.particles.scale);
				}
				
				const bool mp0 = ImGui::InputFloat("Speed", &_state.particles.speed, 0.001f, 1.0f);
				ImGui::SameLine(160);
				const bool mp1 = ImGui::InputFloat("Expansion", &_state.particles.expansion, 0.1f, 5.0f);
				if(mp1 || mp0){
					_scene->setParticlesParameters(_state.particles.speed, _state.particles.expansion);
				}
				
				if(ImGui::Button("Default image")){
					// Use a white square particle appearance by default.
					const GLuint blankID =  ResourcesManager::getTextureFor("blank");
					_state.particles.texs = std::vector<GLuint>(PARTICLES_TEXTURE_COUNT, blankID);
					_state.particles.scale = 1.0f;
				}
				ImGui::SameLine(160);
				if(ImGui::Button("Load images...")){
					// Read arguments.
					nfdpathset_t outPaths;
					nfdresult_t result = NFD_OpenDialogMultiple("png;jpg,jpeg;", NULL, &outPaths);
					
					if(result == NFD_OKAY){
						_state.particles.texs.clear();
						for(size_t i = 0; i < NFD_PathSet_GetCount(&outPaths); ++i) {
							nfdchar_t *outPath = NFD_PathSet_GetPath(&outPaths, i);
							const std::string imageFilePath = std::string(outPath);
							const GLuint tid = loadTexture(imageFilePath, false);
							_state.particles.texs.push_back(tid);
						}
						NFD_PathSet_Free(&outPaths);
						// Keep filling until we have four texture IDs.
						int id = 0;
						const int initCount = _state.particles.texs.size();
						while (_state.particles.texs.size() < PARTICLES_TEXTURE_COUNT) {
							_state.particles.texs.push_back(_state.particles.texs[id]);
							id = (id+1)%initCount;
						}
						
						if(_state.particles.scale <= 9.0f){
							_state.particles.scale = 10.0f;
						}
						
					}
				}
				ImGui::SameLine();
				ImGui::TextDisabled("(?)");
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted("You can select up to 4 images (PNG or JPEG). They should be square and B&W, where black is transparent, white opaque.");
					ImGui::PopTextWrapPos();
					ImGui::EndTooltip();
				}
				
				ImGui::PopID();
				ImGui::PopItemWidth();
			}
		}
		
		
		
		
		
		
		
		if(smw0 || smw1){
			_state.scale = std::max(_state.scale, 0.01f);
			_state.background.minorsWidth = std::min(std::max(_state.background.minorsWidth, 0.1f), 1.0f);
			_scene->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
			_background->setScaleAndMinorWidth(_state.scale, _state.background.minorsWidth);
		}
		
		// Keep the colors in sync if needed.
		if(_state.lockParticleColor){
			if(colNotesEdit){
				_state.particles.color = _state.baseColor;
			} else if(colPartsEdit){
				_state.baseColor = _state.particles.color;
			}
		}
		
	}
	ImGui::End();
}


void Renderer::clean(){
	
	// Clean objects.
	_scene->clean();
	_background->clean();
	_blurringScreen.clean();
	_blurryScreen.clean();
	_particlesFramebuffer->clean();
	_blurFramebuffer->clean();
	
}


void Renderer::resize(int width, int height){
	
	//Update the size of the viewport.
	glViewport(0, 0, width, height);
	// Update the projection matrix.
	_camera.screen(width, height);
	// Resize the framebuffers.
	_particlesFramebuffer->resize(_camera._screenSize);
	
	_blurFramebuffer->resize(_camera._screenSize);
	
}

void Renderer::keyPressed(int key, int action){
	
	if(action == GLFW_PRESS){
		if(key == GLFW_KEY_P){
			_shouldPlay = !_shouldPlay;
			_timerStart = glfwGetTime() - _timer;
		} else if (key == GLFW_KEY_R){
			_timer = 0;
			_timerStart = glfwGetTime();
		} else if (key == GLFW_KEY_L){
			
		} else if (key == GLFW_KEY_I){
			_showGUI = !_showGUI;
		}
	}
	
}


