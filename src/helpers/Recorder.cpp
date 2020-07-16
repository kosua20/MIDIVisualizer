#include "Recorder.h"
#include "../rendering/State.h"

#include <imgui/imgui.h>
#include <nfd.h>
#include <lodepng/lodepng.h>

#include <iostream>
#include <stdio.h>
#include <vector>

Recorder::Recorder(){
	_formats = {
		 {"PNG", "png", Recorder::Format::PNG},
	#ifdef MIDIVIZ_SUPPORT_VIDEO
		{"MPEG2", "mp4", Recorder::Format::MPEG2},
		{"MPEG4", "mp4", Recorder::Format::MPEG4}
	#endif
	};
}

Recorder::~Recorder(){

}

void Recorder::record(const std::shared_ptr<Framebuffer> & frame){

	std::cout << "\r[EXPORT]: Processing frame " << (_currentFrame + 1) << "/" << _framesCount << "." << std::flush;

	// Make sure rendering is complete.
	glFinish();
	glFlush();

	if(frame->_width != _size[0] || frame->_height != _size[1]){
		std::cout << std::endl;
		std::cerr << "Unexpected frame size while recording. Stopping." << std::endl;
		_currentFrame = _framesCount;
		return;
	}

	// Readback.
	frame->bind();
	glReadPixels(0, 0, (GLsizei)_size[0], (GLsizei)_size[1], GL_RGBA, GL_UNSIGNED_BYTE, &_buffer[0]);
	frame->unbind();

	// Copy and flip rows.
	const int width = _size[0];
	const int height = _size[1];
	for (int y = 0; y < height / 2; ++y) {
		const int top = y * width * 4;
		const int topNext = (y+1) * width * 4;
		const int bottom = (height - y - 1) * width * 4;
		std::swap_ranges(_buffer.begin() + top, _buffer.begin() + topNext,
						 _buffer.begin() + bottom);
	}

	if(_outFormat == Format::PNG){
	// Write to disk.
	std::string intString = std::to_string(_currentFrame);
	while (intString.size() < std::ceil(std::log10(float(_framesCount)))) {
		intString = "0" + intString;
	}
	const std::string outputFilePath = _exportPath + "/output_" + intString + ".png";
	unsigned error = lodepng_encode_file( outputFilePath.c_str(), _buffer.data(), width, height, LCT_RGBA, 8);
	if (error) {
		std::cerr << "LodePNG error: " << error << ": " << lodepng_error_text(error) << std::endl;
	}
	} else {
		// This will do nothing (and is unreachable) if the video module is not present.
		addFrameToVideo(_buffer.data());
		if(_currentFrame + 1 == _framesCount){
			endVideo();
		}
	}

	// Flush log.
	if(_currentFrame + 1 == _framesCount){
		std::cout << std::endl;
	}

	_currentTime += (1.0f / float(_exportFramerate));
	++_currentFrame;
}

bool Recorder::drawGUI(){
	bool shouldStart = false;

	// Pick directory/file.
	if (ImGui::Button("Render offline...")) {
		// Read arguments.
		nfdchar_t *outPath = NULL;

		if(_outFormat == Format::PNG){
		nfdresult_t result = NFD_PickFolder(NULL, &outPath);
			if(result == NFD_OKAY) {
			_exportPath = std::string(outPath);
			shouldStart = true;
		}
		} else {
			const std::string & ext = _formats.at(int(_outFormat)).ext;
			nfdresult_t result = NFD_SaveDialog(ext.c_str(), NULL, &outPath);
			if(result == NFD_OKAY) {
				_exportPath = std::string(outPath);
				const std::string fullExt = "." + ext;
				// Append extension if needed.
				if(_exportPath.size() < 5 || (_exportPath.substr(_exportPath.size()-4) != fullExt)){
					_exportPath.append(fullExt);
	}
				shouldStart = true;
			}
		}

	}
	ImGui::SameLine(COLUMN_SIZE);
	ImGui::PushItemWidth(100);
	ImGui::InputInt("Rate", &_exportFramerate);

	// Dropdown list.
	if(ImGui::BeginCombo("Format", _formats[int(_outFormat)].name.c_str())){
		for(size_t i = 0; i < _formats.size(); ++i){
			ImGui::PushID((void*)(intptr_t)i);

			const auto & form = _formats[i];
			const bool selected = form.format == _outFormat;
			if(ImGui::Selectable(form.name.c_str(), selected)){
				_outFormat = form.format;
			}
			if(selected){
				ImGui::SetItemDefaultFocus();
			}
			ImGui::PopID();
		}
		ImGui::EndCombo();
	}

	// Extra options.
	ImGui::SameLine(COLUMN_SIZE);
	if(_outFormat == Format::PNG){
		ImGui::Checkbox("Transparent bg.", &_exportNoBackground);
	} else {
		ImGui::InputInt("Bitrate (Mbps)", &_bitRate);
	}
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(COLUMN_SIZE-10);
	ImGui::InputInt2("Export size", &_size[0]);
	ImGui::PopItemWidth();

	return shouldStart;
}

void Recorder::start(float preroll, float duration){
	_currentFrame = 0;
	_currentTime = -preroll;
	_framesCount = int(std::ceil((duration + 10.0f + preroll) * _exportFramerate));
	_sceneDuration = duration;
	// Image writing setup.
	_buffer.resize(_size[0] * _size[1] * 4);

	if(_outFormat != Format::PNG){
		initVideo(_exportPath, _outFormat);
	}
}

bool Recorder::drawProgress(){
	if(_currentFrame == 1){
		ImGui::OpenPopup("Exporting...");
	}
	if(ImGui::BeginPopupModal("Exporting...", NULL, ImGuiWindowFlags_AlwaysAutoResize)){

		ImGui::Text("Scene duration: %ds. (+10s. buffer).", int(std::round(_sceneDuration)));
		ImGui::Text("Framerate: %d fps.", _exportFramerate);
		ImGui::Text("Destination directory: %s", _exportPath.c_str());
		ImGui::Text("Exporting %zu frames at resolution %dx%d...", _framesCount, _size[0], _size[1]);

		const std::string currProg = std::to_string(_currentFrame + 1) + "/" + std::to_string(_framesCount);
		ImGui::ProgressBar(float(_currentFrame + 1) / float(_framesCount), ImVec2(400.0f, 0.0f), currProg.c_str());
		ImGui::EndPopup();
	}
}

bool Recorder::isRecording() const {
	return _currentFrame < _framesCount;
}

bool Recorder::isTransparent() const {
	return _exportNoBackground;
}

float Recorder::currentTime() const {
	return _currentTime;
}

size_t Recorder::currentFrame() const {
	return _currentFrame;
}

size_t Recorder::framesCount() const {
	return _framesCount;
}

const glm::ivec2 & Recorder::requiredSize() const {
	return _size;
}

void Recorder::setSize(const glm::ivec2 & size){
	_size = size;
}

bool Recorder::initVideo(const std::string & path, Format format){
	return false;
}

bool Recorder::addFrameToVideo(GLubyte * data){
	return false;
}

void Recorder::endVideo(){
}
