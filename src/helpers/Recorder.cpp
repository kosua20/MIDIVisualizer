#include "Recorder.h"
#include "../rendering/State.h"

#include <imgui/imgui.h>
#include <nfd.h>
#include <lodepng/lodepng.h>

#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

#ifdef MIDIVIZ_SUPPORT_VIDEO
extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavformat/avio.h>
	#include <libswscale/swscale.h>
	#include <libavutil/opt.h>
}
#endif

Recorder::Recorder(){
	_formats = {
		 {"PNG", "png", Recorder::Format::PNG},
	#ifdef MIDIVIZ_SUPPORT_VIDEO
		{"MPEG2", "mp4", Recorder::Format::MPEG2},
		{"MPEG4", "mp4", Recorder::Format::MPEG4},
		{"PRORES", "mov", Recorder::Format::PRORES}
	#endif
	};

	#ifdef MIDIVIZ_SUPPORT_VIDEO
		// Some linux distributions (eg Ubuntu 18 LTS) use an old version of ffmpeg/libavformat
		// that needs to be initialized to detect available codecs and formats.
		#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
			av_register_all();
			avcodec_register_all();
		#endif
	#endif
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
		std::cerr << "[EXPORT]: Unexpected frame size while recording. Stopping." << std::endl;
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

	// Cancel alpha premultiplication if requested.
	if(_exportNoBackground && _cancelPremultiply){
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				const int baseInd = (y * width + x) * 4;
				const float a = float(_buffer[baseInd + 3]) / 255.0f;
				if(a == 0.0){
					continue;
				}
				float r = float(_buffer[baseInd + 0]) / 255.0f;
				float g = float(_buffer[baseInd + 1]) / 255.0f;
				float b = float(_buffer[baseInd + 2]) / 255.0f;
				r = glm::clamp(r / a, 0.0f, 1.0f);
				g = glm::clamp(g / a, 0.0f, 1.0f);
				b = glm::clamp(b / a, 0.0f, 1.0f);
				_buffer[baseInd + 0] = GLubyte(255.0f * r);
				_buffer[baseInd + 1] = GLubyte(255.0f * g);
				_buffer[baseInd + 2] = GLubyte(255.0f * b);
			}
		}
	}

	// Erase alpha channel if exporting opaque image.
	if(!_exportNoBackground){
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				const int baseInd = (y * width + x) * 4;
				_buffer[baseInd + 3] = 255;
			}
		}
	}


	if(_outFormat == Format::PNG){
		// Write to disk.
		std::string intString = std::to_string(_currentFrame);
		while (intString.size() < std::ceil(std::log10(float(_framesCount)))) {
			intString = "0" + intString;
		}
		const std::string outputFilePath = _exportPath + "/output_" + intString + ".png";
		// LodePNG encoding settings.
		LodePNGState state;
		lodepng_state_init(&state);
		state.info_raw.colortype = LCT_RGBA;
		state.info_raw.bitdepth = 8;
		state.info_png.color.colortype = _exportNoBackground ? LCT_RGBA : LCT_RGB;
		state.info_png.color.bitdepth = 8;

		// Encode
		unsigned char* outBuffer = nullptr;
		size_t outBufferSize = 0;
		lodepng_encode(&outBuffer, &outBufferSize, _buffer.data(), width, height, &state);
		unsigned int error = state.error;
		lodepng_state_cleanup(&state);

		// Save
		if(!error){
			error = lodepng_save_file(outBuffer, outBufferSize, outputFilePath.c_str());
		}
		free(outBuffer);

		if(error){
			std::cerr << "[EXPORT]: PNG error " << error << ": " << lodepng_error_text(error) << std::endl;
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
		const auto endTime	 = std::chrono::high_resolution_clock::now();
		const long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - _startTime).count();
		std::cout << std::endl;
		std::cout << "[EXPORT]: Export took " << (float(duration) / 1000.0f) << "s." << std::endl;
	}

	_currentTime += (1.0f / float(_exportFramerate));
	++_currentFrame;
}

bool Recorder::drawGUI(float scale){
	bool shouldStart = false;


	if(ImGui::BeginPopupModal("Export", nullptr, ImGuiWindowFlags_AlwaysAutoResize)){
		ImGui::PushItemWidth(scale * 100);
		const float scaledColumn = scale * EXPORT_COLUMN_SIZE;

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
		ImGui::SameLine(scaledColumn);
		ImGui::InputInt("Framerate", &_exportFramerate);

		if(ImGui::InputInt2("Export size", &_size[0])){
			_size[0] += _size[0] % 2;
			_size[1] += _size[1] % 2;
		}

		ImGui::SameLine(scaledColumn);

		ImGui::InputFloat("Postroll", &_postroll, 0.1f, 1.0f, "%.1fs");

		bool lineStarted = false;
		if(_outFormat == Format::PNG || _outFormat == Format::PRORES){
			ImGui::Checkbox("Transparent bg.", &_exportNoBackground);
			lineStarted = true;
		}

		if(_outFormat != Format::PNG){
			if(lineStarted){
				ImGui::SameLine(scaledColumn);
			}
			ImGui::InputInt("Rate (Mbps)", &_bitRate);
		}
		if((_outFormat == Format::PNG || _outFormat == Format::PRORES) && _exportNoBackground){
			ImGui::Checkbox("Fix premultiply", &_cancelPremultiply);
		}

		ImGui::PopItemWidth();


		// Pick directory/file.
		const ImVec2 buttonSize(scaledColumn - scale * 20.0f, 0.0f);
		if(ImGui::Button("Cancel##videpopup", buttonSize)){
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::SameLine(scaledColumn);
		const std::string exportType = _outFormat == Format::PNG ? "images" : "video";
		const std::string exportButtonName = "Save " + exportType + " to...";

		if (ImGui::Button(exportButtonName.c_str(), buttonSize)) {
			// Read arguments.
			nfdchar_t *outPath = NULL;

			if(_outFormat == Format::PNG){
				nfdresult_t result = NFD_PickFolder(NULL, &outPath);
				if(result == NFD_OKAY) {
					_exportPath = std::string(outPath);
					shouldStart = true;
					ImGui::CloseCurrentPopup();
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
					ImGui::CloseCurrentPopup();
				}
			}

		}
		ImGui::EndPopup();
	}

	return shouldStart;
}

void Recorder::prepare(float preroll, float duration, float speed){
	_currentTime = -preroll;
	_framesCount = int(std::ceil((duration + _postroll + preroll) * _exportFramerate / speed));
	_currentFrame = _framesCount;
	_sceneDuration = duration;
	// Image writing setup.
	_buffer.resize(_size[0] * _size[1] * 4);

}

void Recorder::start(bool verbose) {
	_currentFrame = 0;

	if (_outFormat != Format::PNG) {
		initVideo(_exportPath, _outFormat, verbose);
	}
	_startTime = std::chrono::high_resolution_clock::now();
}

void Recorder::drawProgress(){
	if(_currentFrame == 1){
		ImGui::OpenPopup("Exporting...");
	}
	if(ImGui::BeginPopupModal("Exporting...", NULL, ImGuiWindowFlags_AlwaysAutoResize)){

		ImGui::Text("Scene duration: %ds.", int(std::round(_sceneDuration)));
		ImGui::Text("Framerate: %d fps.", _exportFramerate);
		ImGui::Text("Destination path: %s", _exportPath.c_str());

		ImGui::Text("Exporting %zu frames at resolution %dx%d...", _framesCount, _size[0], _size[1]);

		const std::string currProg = std::to_string(_currentFrame + 1) + "/" + std::to_string(_framesCount);
		ImGui::ProgressBar(float(_currentFrame + 1) / float(_framesCount), ImVec2(-1.0f, 0.0f), currProg.c_str());
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
	_size[0] += _size[0]%2;
	_size[1] += _size[1]%2;
}

bool Recorder::setParameters(const std::string & path, Format format, int framerate, int bitrate, float postroll, bool skipBackground, bool fixPremultiply){
	// Check if the format is supported.
	if(int(format) >= _formats.size()){
		std::cerr << "[EXPORT]: The requested output format is not supported by this executable. If this is a video format, make sure MIDIVisualizer has been compiled with ffmpeg enabled by checking the output of ./MIDIVisualizer --version" << std::endl;
		return false;
	}

	_exportPath = path;
	_outFormat = format;
	_exportFramerate = framerate;
	_bitRate = bitrate;
	_exportNoBackground = skipBackground;
	_cancelPremultiply = fixPremultiply;
	_postroll = postroll;

	if(_outFormat != Format::PNG){
		// Check that the export path is valid.
		const std::string & ext = _formats.at(int(_outFormat)).ext;
		const std::string fullExt = "." + ext;
		// Append extension if needed.
		if(_exportPath.size() < 5 || (_exportPath.substr(_exportPath.size()-4) != fullExt)){
			_exportPath.append(fullExt);
		}
	}
	return true;
}

bool Recorder::videoExportSupported(){
#ifdef MIDIVIZ_SUPPORT_VIDEO
	return true;
#else
	return false;
#endif
}

bool Recorder::initVideo(const std::string & path, Format format, bool verbose){
#ifdef MIDIVIZ_SUPPORT_VIDEO
	if(format == Format::PNG){
		std::cerr << "[EXPORT]: Unable to use PNG format for video." << std::endl;
		return false;
	}

	if (verbose) {
		std::cout << "[VIDEO]: Attempting export at " << _size[0] << " x " << _size[1] << std::endl;
	}

	av_log_set_level(verbose ? AV_LOG_VERBOSE : AV_LOG_ERROR);

	// Allocate general context.
	if(avformat_alloc_output_context2(&_formatCtx, nullptr, nullptr, path.c_str()) < 0 || !_formatCtx){
		std::cerr << "[VIDEO]: Unable to create format context." << std::endl;
		return false;
	}
	if(_formatCtx->oformat->flags & AVFMT_NOFILE){
		std::cerr << "[VIDEO]: Format not associated to a file." << std::endl;
		return false;
	}

	struct InternalCodecOpts {
		AVCodecID avid;
		AVPixelFormat avformat;
	};
	static const std::unordered_map<Format, InternalCodecOpts> opts = {
		{Format::MPEG2, {AV_CODEC_ID_MPEG2VIDEO, AV_PIX_FMT_YUV422P}},
		{Format::MPEG4, {AV_CODEC_ID_MPEG4, AV_PIX_FMT_YUV420P}},
		{Format::PRORES, {AV_CODEC_ID_PRORES, AV_PIX_FMT_YUVA444P10}},
	};

	// Setup codec.
	const auto & outFormat = opts.at(format);
	auto _codec = avcodec_find_encoder(outFormat.avid);
	if(!_codec){
		std::cerr << "[VIDEO]: Unable to find encoder." << std::endl;
		return false;
	}

	// Setup codec context and parameters.
	_codecCtx = avcodec_alloc_context3(_codec);
	if(!_codecCtx){
		std::cerr << "[VIDEO]: Unable to create encoder context." << std::endl;
		return false;
	}
	const int tgtW = _size[0] - _size[0]%2;
	const int tgtH = _size[1] - _size[1]%2;
	_codecCtx->codec_id = outFormat.avid;
	_codecCtx->width = tgtW;
	_codecCtx->height = tgtH;
	_codecCtx->time_base = {1, _exportFramerate };
	_codecCtx->framerate = { _exportFramerate, 1};
	_codecCtx->gop_size = 10;
	_codecCtx->pix_fmt = outFormat.avformat;
	_codecCtx->bit_rate = _bitRate*1000000;
	if(_formatCtx->oformat->flags & AVFMT_GLOBALHEADER){
		_codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
	}

	// For PRORES, try to be as fast as possible.
	if(format == Format::PRORES){
		// Just in case the slowest prores codec is picked, try to configure it for maximum speed
		// as described in doc (https://ffmpeg.org/ffmpeg-codecs.html#toc-Speed-considerations)
		//_codecCtx->flags |= AV_CODEC_FLAG_QSCALE;
		//_codecCtx->global_quality = FF_QP2LAMBDA * 4;
		av_opt_set_int(_codecCtx->priv_data, "qscale", 4, 0);
		av_opt_set_int(_codecCtx->priv_data, "bits_per_mb", 8000, 0);
	}

	AVDictionary * codecParams = nullptr;
	if(avcodec_open2(_codecCtx, _codec, &codecParams) < 0){
		std::cerr << "[VIDEO]: Unable to open encoder." << std::endl;
		return false;
	}

	// Setup stream.
	_stream = avformat_new_stream(_formatCtx, _codec);
	if(!_stream){
		std::cerr << "[VIDEO]: Unable to create stream." << std::endl;
		return false;
	}
	_stream->id = _formatCtx->nb_streams - 1;
	_stream->time_base = {1, _exportFramerate };
	// Sync parameters.
	av_dict_free(&codecParams);
	if(avcodec_parameters_from_context(_stream->codecpar, _codecCtx) < 0){
		std::cerr << "[VIDEO]: Unable to transfer parameters from encoder to stream." << std::endl;
		return false;
	}

	// Allocate frame.
	_frame = av_frame_alloc();
	if(!_frame){
		std::cerr << "[VIDEO]: Unable to allocate frame." << std::endl;
		return false;
	}
	_frame->format = _codecCtx->pix_fmt;
	_frame->width = _codecCtx->width;
	_frame->height = _codecCtx->height;
	_frame->pts = 0;
	if(av_frame_get_buffer(_frame, 0) < 0){
		std::cerr << "[VIDEO]: Unable to create frame buffer." << std::endl;
		return false;
	}

	// Open file, write header.
	if(avio_open(&_formatCtx->pb, path.c_str(), AVIO_FLAG_WRITE) < 0){
		std::cerr << "[VIDEO]: Unable to open IO file." << std::endl;
		return false;
	}
	if(avformat_write_header(_formatCtx, nullptr) < 0){
		std::cerr << "[VIDEO]: Unable to write header." << std::endl;
		return false;
	}
	
	// Create scaling/conversion context.
	_swsContext = sws_getContext(_size[0], _size[1], AV_PIX_FMT_RGBA, _codecCtx->width, _codecCtx->height, _codecCtx->pix_fmt, SWS_POINT, nullptr, nullptr, nullptr);
	if(!_swsContext){
		std::cerr << "[VIDEO]: Unable to create processing context." << std::endl;
		return false;
	}

	// Debug log.
	if (verbose) {
		std::cout << "[VIDEO]: Context infos: " << std::endl;
		av_dump_format(_formatCtx, 0, path.c_str(), 1);
		std::cout << std::endl;
	}
	return true;
#else
	return false;
#endif

}

bool Recorder::addFrameToVideo(GLubyte * data){
#ifdef MIDIVIZ_SUPPORT_VIDEO
	unsigned char * srcs[AV_NUM_DATA_POINTERS] = {0};
	int strides[AV_NUM_DATA_POINTERS] = {0};
	srcs[0] = (unsigned char *)data;
	strides[0] = int(_size[0] * 4);
	// Rescale and convert to the proper output layout.
	sws_scale(_swsContext, srcs, strides, 0, _size[1], _frame->data, _frame->linesize);
	// Send frame.
	const int res = avcodec_send_frame(_codecCtx, _frame);
	if(res == AVERROR(EAGAIN)){
		// Unavailable right now, should flush and retry.
		if(flush()){
			avcodec_send_frame(_codecCtx, _frame);
		}
	} else if(res < 0){
		std::cerr << "[VIDEO]: Unable to send frame." << std::endl;
		return false;
	}
	_frame->pts++;
	return true;
#else
	return false;
#endif
}

void Recorder::endVideo(){
#ifdef MIDIVIZ_SUPPORT_VIDEO
	avcodec_send_frame(_codecCtx, nullptr);
	flush();
	av_write_trailer(_formatCtx);
	avio_closep(&_formatCtx->pb);
	avcodec_free_context(&_codecCtx);
	av_frame_free(&_frame);
	sws_freeContext(_swsContext);
	avformat_free_context(_formatCtx);

	_formatCtx = nullptr;
	_codec= nullptr;
	_codecCtx= nullptr;
	_stream= nullptr;
	_frame= nullptr;
	_swsContext= nullptr;
#endif
}

bool Recorder::flush(){
#ifdef MIDIVIZ_SUPPORT_VIDEO
	// Keep flushing.
	while(true){
		AVPacket packet = {0};
		av_init_packet(&packet);
		// Get packet.
		int res = avcodec_receive_packet(_codecCtx, &packet);
		if(res == AVERROR(EAGAIN) || res == AVERROR_EOF){
			return true;
		} else if(res < 0){
			std::cerr << "[VIDEO]: Unable to retrieve packet." << std::endl;
			return false;
		}
		// Adjust timing for output.
		av_packet_rescale_ts(&packet, _codecCtx->time_base, _stream->time_base);
		packet.stream_index = _stream->index;
		// Write packet.
		res = av_interleaved_write_frame(_formatCtx, &packet);
		if(res < 0){
			std::cerr << "[VIDEO]: Unable to write frame to file." << std::endl;
			return false;
		}
	}
	return true;
#else
	return false;
#endif
}
