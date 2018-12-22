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
	
	// Setup projection matrix.
	_camera.screen(width, height);
	
	// Setup framebuffers.
	_particlesFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(_camera._screenSize[0],_camera._screenSize[1], GL_RGBA,GL_UNSIGNED_BYTE,GL_LINEAR,GL_CLAMP_TO_EDGE));
	_blurFramebuffer = std::shared_ptr<Framebuffer>(new Framebuffer(_camera._screenSize[0],_camera._screenSize[1], GL_RGBA,GL_UNSIGNED_BYTE,GL_LINEAR,GL_CLAMP_TO_EDGE));
	
	_blurringScreen.init(_particlesFramebuffer->textureId(), ResourcesManager::getStringForShader("particlesblur_frag"));
	_blurryScreen.init(_blurFramebuffer->textureId(), ResourcesManager::getStringForShader("screenquad_frag"));
	
	_state.lookParticles = ResourcesManager::getTextureFor("blank");
	//_state.baseColor = 1.35f*glm::vec3(0.57f,0.19f,0.98f);
	//_state.particlesColor=  _state.baseColor;
	// Check setup errors.
	checkGLError();
}

void Renderer::setColorAndScale(const glm::vec3 & baseColor, const float scale){
	_state.scale = scale;
	_state.baseColor = baseColor;
	_state.particlesColor=  _state.baseColor;
}

void Renderer::loadFile(const std::string & midiFilePath){
	// Player.
	_timer = 0.0;
	_shouldPlay = false;

	// Init objects.
	_scene = std::make_shared<MIDIScene>(midiFilePath);
	_scene->setScaleAndMinorWidth(_state.scale, _state.minorsWidth);
	_scene->setParticlesParameters(_state.particlesSpeed, _state.particlesExpansion);
	
	_background = std::make_shared<Background>(_scene->midiFile().tracks[0].secondsPerMeasure);
	_background->setScaleAndMinorWidth(_state.scale, _state.minorsWidth);
	_background->setDisplay(_state.showDigits, _state.showHLines, _state.showVLines, _state.showKeys);
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
			_scene->drawParticles(_timer, 1.0f/ _camera._screenSize, _state.particlesColor, _state.particlesScale, _state.lookParticles, _state.particlesCount, true);
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
		_scene->drawParticles(_timer, 1.0f/ _camera._screenSize, _state.particlesColor, _state.particlesScale, _state.lookParticles, _state.particlesCount, false);
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
		//ImGui::ShowDemoWindow();
		if(ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoResize)){
			ImGui::Text("Welcome in MIDIVisualizer v2.2!");
			ImGui::Text("Keys:");
			ImGui::Text("\tp: start/stop playing");
			ImGui::Text("\tr: reset track");
			ImGui::Text("\ti: hide this panel");
			
			if(ImGui::Button("Load file...")){
				// Read arguments.
				std::string midiFilePath;
				nfdchar_t *outPath = NULL;
				nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
				if(result == NFD_OKAY){
					midiFilePath = std::string(outPath);
					loadFile(midiFilePath);
				}
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(100);
			
			const bool smw0 = ImGui::InputFloat("Scale", &_state.scale, 0.01f, 0.1f);
			const bool smw1 = ImGui::InputFloat("Minor keys width", &_state.minorsWidth, 0.1f, 1.0f);
			if(smw0 || smw1){
				_state.scale = std::max(_state.scale, 0.01f);
				_state.minorsWidth = std::min(std::max(_state.minorsWidth, 0.1f), 1.0f);
				_scene->setScaleAndMinorWidth(_state.scale, _state.minorsWidth);
				_background->setScaleAndMinorWidth(_state.scale, _state.minorsWidth);
			}
			
			
			ImGui::PopItemWidth();
			ImGui::Checkbox("Particles ", &_state.showParticles);
			if(_state.showParticles){
				ImGui::SameLine();
				ImGui::PushID("ParticlesSettings");
				ImGui::PushItemWidth(100);
				ImGui::SliderInt("Count", &_state.particlesCount, 1, 512);
				
				const bool mp0 = ImGui::InputFloat("Speed", &_state.particlesSpeed, 0.001f, 1.0f);
				ImGui::SameLine();
				const bool mp1 = ImGui::InputFloat("Expansion", &_state.particlesExpansion, 0.1f, 5.0f);

				if(mp1 || mp0){
					_scene->setParticlesParameters(_state.particlesSpeed, _state.particlesExpansion);
				}
				if(ImGui::InputFloat("Size", &_state.particlesScale, 1.0f, 10.0f)){
					_state.particlesScale = std::max(1.0f, _state.particlesScale);
				}
				ImGui::Text("Particles look");
				ImGui::SameLine();
				if(ImGui::Button("Load...")){
					// Read arguments.
					std::string imageFilePath;
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog( "png", NULL, &outPath );
					if(result == NFD_OKAY){
						imageFilePath = std::string(outPath);
						const GLuint tid = loadTexture(imageFilePath, false);
						_state.lookParticles = tid;
						if(_state.particlesScale <= 9.0f){
							_state.particlesScale = 10.0f;
						}
						
					}
				}
				ImGui::SameLine();
				if(ImGui::Button("Default")){
					// Use a white square particle appearance by default.
					_state.lookParticles = ResourcesManager::getTextureFor("blank");
					_state.particlesScale = 1.0f;
				}
				ImGui::PopID();
				ImGui::PopItemWidth();
			}
			ImGui::Checkbox("Blur      ", &_state.showBlur);
			if(_state.showBlur) {
				ImGui::SameLine();
				ImGui::Checkbox("Blur notes", &_state.showBlurNotes);
			}

			ImGui::Checkbox("Flashes   ", &_state.showFlashes); ImGui::SameLine();
			bool m1 = ImGui::Checkbox("Digits", &_state.showDigits);
			bool m2 = ImGui::Checkbox("H. lines  ", &_state.showHLines);  ImGui::SameLine();
			bool m3 = ImGui::Checkbox("V. lines", &_state.showVLines);
			bool m4 = ImGui::Checkbox("Keyboard", &_state.showKeys);
			
			if(m1 || m2 || m3 || m4){
				_background->setDisplay(_state.showDigits, _state.showHLines, _state.showVLines, _state.showKeys);
			}


			bool colNotesEdit = ImGui::ColorEdit3("Notes", &_state.baseColor[0], ImGuiColorEditFlags_NoInputs); ImGui::SameLine();
			bool colPartsEdit = ImGui::ColorEdit3("Effects", &_state.particlesColor[0], ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			if(ImGui::Checkbox("Lock colors", &_state.lockParticleColor)){
				// If we enable the lock, make sure the colors are synched.
				colNotesEdit = true;
			}
			
			// Keep the colors in sync if needed.
			if(_state.lockParticleColor){
				if(colNotesEdit){
					_state.particlesColor = _state.baseColor;
				} else if(colPartsEdit){
					_state.baseColor = _state.particlesColor;
				}
			}
			
			
			ImGui::Text("Created by S. Rodriguez (kosua20)");
			ImGui::Text("github.com/kosua20/MIDIVisualizer");
		}
		ImGui::End();
		
	}
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


