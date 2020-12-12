#include <gl3w/gl3w.h> // to load OpenGL extensions at runtime
#include <GLFW/glfw3.h> // to set up the OpenGL context and manage window lifecycle and inputs
#include "helpers/ProgramUtilities.h"
#include "helpers/Configuration.h"
#include "helpers/ResourcesManager.h"

#include "rendering/Renderer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <nfd.h>
#include <iostream>
#include <algorithm>

#define INITIAL_SIZE_WIDTH 1280
#define INITIAL_SIZE_HEIGHT 600

void printVersion(){
	std::cout << "MIDIVisualizer v" << MIDIVIZ_VERSION_MAJOR << "." << MIDIVIZ_VERSION_MINOR << std::endl;
	std::cout << "* Built on " << __DATE__ << ", at " << __TIME__ << "." << std::endl;
	std::cout << "* Video export with ffmpeg is " << (Recorder::videoExportSupported() ? "enabled" : "disabled") << "." << std::endl;
}

void printHelp(){
	std::string configOpts, setsOpts;
	const size_t alignSize = State::helpText(configOpts, setsOpts);

	const std::vector<std::pair<std::string, std::string>> genOpts = {
		{"midi", "path to a MIDI file to load"},
		{"config", "path to a configuration INI file"},
		{"size", "dimensions of the window (--size W H)"},
		{"fullscreen", "start in fullscreen (1 or 0 to enabled/disable)"},
		{"gui-size", "GUI text and button scaling (number, default 1.0)"},
		{"help", "display this help message"},
		{"version", "display the executable version and configuration"},
	};

	const std::vector<std::pair<std::string, std::string>> expOpts = {
		{"export", "path to the output video (or directory for PNG)"},
		{"format", "output format (values: PNG, MPEG2, MPEG4)"},
		{"framerate", "number of frames per second to export (integer)"},
		{"bitrate", "target video bitrate in Mb (integer)"},
		{"png-alpha", "use transparent PNG background (1 or 0 to enabled/disable)"},
		{"hide-window", "do not display the window (1 or 0 to enabled/disable)"},
	};

	std::cout << "---- Infos ---- MIDIVisualizer v" << MIDIVIZ_VERSION_MAJOR << "." << MIDIVIZ_VERSION_MINOR << " --------" << std::endl
	<< "Visually display a midi file in real time." << std::endl
	<< "Created by Simon Rodriguez (https://github.com/kosua20/MIDIVisualizer)" << std::endl;

	std::cout << std::endl << "* General options: " << std::endl;
	for(const auto & opt : genOpts){
		const std::string pad(std::max(int(alignSize) - int(opt.first.size()), 0), ' ');
		std::cout << "--" << opt.first << pad << opt.second << std::endl;
	}

	std::cout << std::endl << "* Export options: (--export path is mandatory)" << std::endl;
	for(const auto & opt : expOpts){
		const std::string pad(std::max(int(alignSize) - int(opt.first.size()), 0), ' ');
		std::cout << "--" << opt.first << pad << opt.second << std::endl;
	}

	std::cout << std::endl << "* Configuration options: (will override config file)" << std::endl
	<< configOpts;

	std::cout << std::endl << "* Note-sets options: (will override config file)" << std::endl
	<< setsOpts << std::endl;

}

/// Callbacks

void resize_callback(GLFWwindow* window, int width, int height){
	Renderer *renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	renderer->resize(width, height);
}

void rescale_callback(GLFWwindow* window, float xscale, float yscale){
	Renderer *renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	// Assume only one of the two for now.
	renderer->rescale(xscale);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(!ImGui::GetIO().WantCaptureKeyboard){
		// Get pointer to the renderer.
		Renderer *renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
		renderer->keyPressed(key, action);
	}
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

/// Perform system window action.

void performAction(SystemAction action, GLFWwindow * window, glm::ivec4 & frame){
	switch (action.type) {
		case SystemAction::FULLSCREEN: {
			// Are we currently fullscreen?
			const bool fullscreen = glfwGetWindowMonitor(window) != nullptr;
			if(fullscreen) {
				// Restore the window position and size.
				glfwSetWindowMonitor(window, nullptr, frame[0], frame[1], frame[2], frame[3], 0);
				// Check the window position and size (if we are on a screen smaller than the initial size).
				glfwGetWindowPos(window, &frame[0], &frame[1]);
				glfwGetWindowSize(window, &frame[2], &frame[3]);
			} else {
				// Backup the window current frame.
				glfwGetWindowPos(window, &frame[0], &frame[1]);
				glfwGetWindowSize(window, &frame[2], &frame[3]);
				// Move to fullscreen on the primary monitor.
				GLFWmonitor * monitor	= glfwGetPrimaryMonitor();
				const GLFWvidmode * mode = glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			}

			// On some hardware, V-sync options can be lost.
			glfwSwapInterval(1);
			break;
		}
		case SystemAction::RESIZE:
			glfwSetWindowSize(window, action.data[0], action.data[1]);
			// Check the window position and size (if we are on a screen smaller than the target size).
			glfwGetWindowPos(window, &frame[0], &frame[1]);
			glfwGetWindowSize(window, &frame[2], &frame[3]);
			break;
		case SystemAction::FIX_SIZE:
			glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
			// This is for recording, to go as fast as possible on the GPU size.
			glfwSwapInterval(0);
			break;
		case SystemAction::FREE_SIZE:
			glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);
			// Restore V-sync after recording end.
			glfwSwapInterval(1);
			break;
		case SystemAction::QUIT:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		default:
			break;
	}
}

/// The main function

int main( int argc, char** argv) {

	// Parse arguments.
	bool showHelp = false;
	bool showVersion = false;
	Arguments args = Configuration::parseArguments(std::vector<std::string>(argv, argv+argc), showHelp, showVersion);

	if(showHelp){
		printHelp();
		return 0;
	}
	if(showVersion){
		printVersion();
		return 0;
	}

	// Initialize glfw, which will create and setup an OpenGL context.
	if (!glfwInit()) {
		std::cerr << "[ERROR]: could not start GLFW3" << std::endl;
		return 2;
	}
	
	// On OS X, the correct OpenGL profile and version to use have to be explicitely defined.
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int isw = INITIAL_SIZE_WIDTH;
	int ish = INITIAL_SIZE_HEIGHT;
	if(args.count("size") > 0){
		const auto & vals = args["size"];
		if(vals.size() >= 2){
			isw = Configuration::parseInt(vals[0]);
			ish = Configuration::parseInt(vals[1]);
		}
	}

	// Fullscreen at launch.
	bool fullscreen = false;
	if(args.count("fullscreen") > 0 && Configuration::parseBool(args["fullscreen"][0])){
		fullscreen = true;
	}

	// Hide window if needed.
	if(args.count("hide-window") > 0 && Configuration::parseBool(args["hide-window"][0])){
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	}

	// Create a window with a given size. Width and height are macros as we will need them again.
	GLFWwindow* window = glfwCreateWindow(isw, ish,"MIDI Visualizer", NULL, NULL);
	if (!window) {
		std::cerr << "[ERROR]: could not open window with GLFW3" << std::endl;
		glfwTerminate();
		return 2;
	}

	// Bind the OpenGL context and the new window.
	glfwMakeContextCurrent(window);

	if (gl3wInit()) {
		std::cerr << "[ERROR]: Failed to initialize OpenGL" << std::endl;
		return -1;
	}
	if (!gl3wIsSupported(3, 2)) {
		std::cerr << "[ERROR]: OpenGL 3.2 not supported\n" << std::endl;
		return -1;
	}

	// We need a scope to ensure the renderer is deleted before the OpenGL context is destroyed.
	{

		// Setup resources.
		ResourcesManager::loadResources();
		// Create the renderer.
		Renderer renderer(isw, ish, fullscreen);

		// Load midi file if specified by command-line.
		// Check if a path is given in argument.
		if(args.count("midi") > 0){
			renderer.loadFile(args["midi"][0]);
		}

		// Setup ImGui for interface.
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = NULL;
		ImGui_ImplGlfw_InitForOpenGL(window, false);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Apply custom state.
		State state;
		if(args.count("config") > 0){
			state.load(args.at("config")[0]);
		}
		// Apply any extra display argument on top of the (optional) config.
		state.load(args);
		renderer.setState(state);

		// Define utility pointer for callbacks (can be obtained back from inside the callbacks).
		glfwSetWindowUserPointer(window, &renderer);
		// Callbacks.
		glfwSetFramebufferSizeCallback(window, resize_callback);	// Resizing the window
		glfwSetKeyCallback(window,key_callback);					// Pressing a key
		glfwSetScrollCallback(window,scroll_callback);				// Scrolling
		glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
		//glfwSetWindowContentScaleCallback(window, rescale_callback);
		glfwSwapInterval(1);

		// On HiDPI screens, we might have to initially resize the framebuffers size.
		glm::ivec4 frame(0);
		glfwGetWindowPos(window, &frame[0], &frame[1]);
		glfwGetWindowSize(window, &frame[2], &frame[3]);
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		const float scale = float(width) / float((std::max)(frame[2], 1));
		renderer.resizeAndRescale(width, height, scale);

		// Scale the GUI based on options.
		float guiScale = 1.0f;
		if(args.count("gui-size") > 0){
			guiScale = Configuration::parseFloat(args["gui-size"][0]);
		}
		renderer.setGUIScale(guiScale);

		const bool directRecord = args.count("export") > 0;
		if(directRecord){
			const int framerate = args.count("framerate") > 0 ? Configuration::parseInt(args["framerate"][0]) : 60;
			const int bitrate = args.count("bitrate") > 0 ? Configuration::parseInt(args["bitrate"][0]) : 40;
			const bool pngAlpha = args.count("png-alpha") > 0 ? Configuration::parseBool(args["png-alpha"][0]) : false;
			const std::string exportPath = args["export"][0];
			Recorder::Format format = Recorder::Format::PNG;
			if(args.count("format") > 0){
				const auto & formatRaw = args["format"][0];
				if(formatRaw == "MPEG2"){
					format = Recorder::Format::MPEG2;
				} else if(formatRaw == "MPEG4"){
					format = Recorder::Format::MPEG4;
				}
			}
			const bool success = renderer.startDirectRecording(exportPath, format, framerate, bitrate, pngAlpha, glm::vec2(isw, ish));
			if(!success){
				// Quit.
				performAction(SystemAction::QUIT, window, frame);
			}
		}

		if(fullscreen){
			performAction(SystemAction::FULLSCREEN, window, frame);
		}

		// Start the display/interaction loop.
		while (!glfwWindowShouldClose(window)) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Update the content of the window.
			SystemAction action = renderer.draw(DEBUG_SPEED*float(glfwGetTime()));

			// Perform system window action if required.
			performAction(action, window, frame);

			// Interface rendering.
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			//Display the result fo the current rendering loop.
			glfwSwapBuffers(window);
			// Update events (inputs,...).
			glfwPollEvents();

		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		renderer.clean();
	}

	// Remove the window.
	glfwDestroyWindow(window);
	// Clean other resources
	// Close GL context and any other GLFW resources.
	glfwTerminate();
	return 0;
}


