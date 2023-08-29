#include <gl3w/gl3w.h> // to load OpenGL extensions at runtime
#include <GLFW/glfw3.h> // to set up the OpenGL context and manage window lifecycle and inputs
#include "helpers/ProgramUtilities.h"
#include "helpers/Configuration.h"
#include "helpers/ResourcesManager.h"
#include "helpers/ImGuiStyle.h"
#include "helpers/System.h"

#include "rendering/Viewer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <sr_gui.h>
#include <iostream>
#include <algorithm>

/// Callbacks

void resize_callback(GLFWwindow* window, int width, int height){
	Viewer *viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
	viewer->resize(width, height);
}

void rescale_callback(GLFWwindow* window, float xscale, float yscale){
	Viewer *viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
	// Assume only one of the two for now.
	viewer->rescale(xscale);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(!ImGui::GetIO().WantCaptureKeyboard){
		// Get pointer to the viewer.
		Viewer *viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
		viewer->keyPressed(key, action);
	}
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void drop_callback(GLFWwindow* window, int count, const char** paths){
	if(count == 0){
		return;
	}

	Viewer *viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
	bool loadedMIDI = false;
	bool loadedConfig = false;
	for(int i = 0; i < count; ++i){
		std::string path(paths[i]);
		if(path.empty()){
			continue;
		}
		const std::string::size_type pos = path.rfind('.');
		if(pos == std::string::npos){
			continue;
		}
		if(pos == path.size() - 1){
			continue;
		}
		const std::string extension = path.substr(pos + 1);
		// Determine path file type.
		const bool isMIDI = extension == "mid" || extension == "midi";
		const bool isConfig = extension == "ini" || extension == "config";
		// Attempt to load MIDI if not already loaded.
		if(!loadedMIDI && isMIDI){
			loadedMIDI = viewer->loadFile(path);
		}
		// Attempt to load state if not already loaded.
		if(!loadedConfig && isConfig){
			State newState;
			loadedConfig = newState.load(path);
			if(loadedConfig){
				viewer->setState(newState);
			}
		}
	}
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
			// This is for recording, to go as fast as possible on the GPU side.
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

const char* getGlfwErrorType(int type){
	switch (type) {
		case GLFW_NO_ERROR: 			return "NO_ERROR";
		case GLFW_NOT_INITIALIZED: 		return "NOT_INITIALIZED";
		case GLFW_NO_CURRENT_CONTEXT: 	return "NO_CURRENT_CONTEXT";
		case GLFW_INVALID_ENUM: 		return "INVALID_ENUM";
		case GLFW_INVALID_VALUE: 		return "INVALID_VALUE";
		case GLFW_OUT_OF_MEMORY: 		return "OUT_OF_MEMORY";
		case GLFW_API_UNAVAILABLE: 		return "API_UNAVAILABLE";
		case GLFW_VERSION_UNAVAILABLE: 	return "VERSION_UNAVAILABLE";
		case GLFW_PLATFORM_ERROR: 		return "PLATFORM_ERROR";
		case GLFW_FORMAT_UNAVAILABLE: 	return "FORMAT_UNAVAILABLE";
		case GLFW_NO_WINDOW_CONTEXT: 	return "NO_WINDOW_CONTEXT";
		default:						break;
	}
	return "UNKNOWN";
}

void logLastGlfwError(){
	const char* msg = nullptr;
	const int code = glfwGetError(&msg);
	if(code == GLFW_NO_ERROR){
		return;
	}
	std::cerr << "[ERROR]: GLFW error: " << getGlfwErrorType(code) << " \"" << (msg ? msg : "") << "\"" << std::endl;
}

/// The main function
int main( int argc, char** argv) {

	sr_gui_init();

	// Initialize glfw, which will create and setup an OpenGL context.
	if (!glfwInit()) {
		std::cerr << "[ERROR]: could not init GLFW3" << std::endl;
		logLastGlfwError();
		sr_gui_show_message("Unable to start MIDIVisualizer", "Could not init GLFW", SR_GUI_MESSAGE_LEVEL_ERROR);
		sr_gui_cleanup();
		return 2;
	}
	// Ensure we are using the C locale.
	System::forceLocale();

	// Retrieve the settings directory for all applications.
	std::string applicationDataPath = System::getApplicationDataDirectory();
	// If this is not empty (ie the working directory), be a good citizen
	// and save config in a subdirectory belonging to MIDIVisualizer.
	if(!applicationDataPath.empty()){
		// We also need to make sure the directory exist.
		System::createDirectory(applicationDataPath);
		// And create a subdirectory for MIDIVisualizer.
		applicationDataPath += "MIDIVisualizer/";
		// Idem.
		System::createDirectory(applicationDataPath);
	}
	const std::string internalConfigPath = applicationDataPath + Configuration::defaultName();

	// This has to be called after glfwInit for the working dir to be OK on macOS.
	Configuration config(internalConfigPath, std::vector<std::string>(argv, argv+argc));

	if(config.showHelp){
		Configuration::printHelp();
		glfwTerminate();
		sr_gui_cleanup();
		return 0;
	}
	if(config.showVersion){
		Configuration::printVersion();
		glfwTerminate();
		sr_gui_cleanup();
		return 0;
	}
	
	// On OS X, the correct OpenGL profile and version to use have to be explicitely defined.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window visiblity and transparency.
	glfwWindowHint(GLFW_VISIBLE, config.hideWindow ? GLFW_FALSE : GLFW_TRUE);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, config.preventTransparency ? GLFW_FALSE : GLFW_TRUE);

	// Create a window with a given size. Width and height are macros as we will need them again.
	GLFWwindow* window = glfwCreateWindow(config.windowSize[0], config.windowSize[1], "MIDI Visualizer", NULL, NULL);
	if (!window) {
		std::cerr << "[ERROR]: Could not open window with GLFW3" << std::endl;
		logLastGlfwError();
		glfwTerminate();
		sr_gui_show_message("Unable to start MIDIVisualizer", "Could not open window with GLFW3", SR_GUI_MESSAGE_LEVEL_ERROR);
		sr_gui_cleanup();
		return 2;
	}
	// Set window position.
	glfwSetWindowPos(window, config.windowPos[0], config.windowPos[1]);
	// Check if transparency was successfully enabled.
	config.preventTransparency = glfwGetWindowAttrib(window, GLFW_TRANSPARENT_FRAMEBUFFER) == GLFW_FALSE;

	// Bind the OpenGL context and the new window.
	glfwMakeContextCurrent(window);

	if(gl3wInit()){
		std::cerr << "[ERROR]: Failed to initialize OpenGL" << std::endl;
		logLastGlfwError();
		glfwTerminate();
		sr_gui_show_message("Unable to start MIDIVisualizer", "Failed to initialize OpenGL", SR_GUI_MESSAGE_LEVEL_ERROR);
		sr_gui_cleanup();
		return -1;
	}
	if(!gl3wIsSupported(3, 2)){
		std::cerr << "[ERROR]: OpenGL 3.2 not supported" << std::endl;
		logLastGlfwError();
		glfwTerminate();
		sr_gui_show_message("Unable to start MIDIVisualizer", "OpenGL 3.2 not supported", SR_GUI_MESSAGE_LEVEL_ERROR);
		sr_gui_cleanup();
		return -1;
	}

	// The font should be maintained alive until the atlas is built.
	ImFontConfig font;
	// We need a scope to ensure the viewer is deleted before the OpenGL context is destroyed.
	{

		// Setup resources.
		ResourcesManager::loadResources();
		// Create the viewer (passing options to display them)
		Viewer viewer(config);

		// Setup ImGui for interface.
		ImGui::CreateContext();

		ImGui::configureFont(font);
		ImGui::configureStyle();
		
		ImGui_ImplGlfw_InitForOpenGL(window, false);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Load midi file if specified.
		if(!config.lastMidiPath.empty()){
			viewer.loadFile(config.lastMidiPath);
		}
		// Apply custom state.
		State state;
		if(!config.lastConfigPath.empty()){
			state.load(config.lastConfigPath);
		}
		// Apply any extra display argument on top of the existing config.
		state.load(config.args());
		viewer.setState(state);

		// Connect to MIDI device if specified. We do it after setting the state because there are constraints on the scroll direction when recording.
		// But we don't want to force reverse-scroll when playing back a recorded liveplay.
		if(!config.lastMidiDevice.empty()){
			viewer.connectDevice(config.lastMidiDevice);
		}

		// Define utility pointer for callbacks (can be obtained back from inside the callbacks).
		glfwSetWindowUserPointer(window, &viewer);
		glfwSetFramebufferSizeCallback(window, resize_callback);
		glfwSetKeyCallback(window,key_callback);
		glfwSetScrollCallback(window,scroll_callback);
		glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
		glfwSetDropCallback(window, drop_callback);
		glfwSwapInterval(1);

		// On HiDPI screens, we might have to initially resize the framebuffers size.
		glm::ivec4 frame(0);
		glfwGetWindowPos(window, &frame[0], &frame[1]);
		glfwGetWindowSize(window, &frame[2], &frame[3]);
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		const float scale = float(width) / float((std::max)(frame[2], 1));
		viewer.resizeAndRescale(width, height, scale);

		// Scale the GUI based on options. This one has to be done late, after ImGui initialisation.
		viewer.setGUIScale(config.guiScale);

		// Direct export.
		const bool directRecord = !config.exporting.path.empty();
		if(directRecord){
			const bool success = viewer.startDirectRecording(config.exporting, config.windowSize);
			if(!success){
				// Quit.
				performAction(SystemAction::QUIT, window, frame);
			}
		}

		if(config.fullscreen){
			performAction(SystemAction::FULLSCREEN, window, frame);
		}

		// Start the display/interaction loop.
		while (!glfwWindowShouldClose(window)) {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Update the content of the window.
			SystemAction action = viewer.draw(DEBUG_SPEED * float(glfwGetTime()));

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
		// Refresh and save global settings.
		viewer.updateConfiguration(config);
		glfwGetWindowPos(window, &config.windowPos[0], &config.windowPos[1]);
		glfwGetWindowSize(window, &config.windowSize[0], &config.windowSize[1]);
		config.save(internalConfigPath);

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		viewer.clean();
	}

	// Remove the window.
	glfwDestroyWindow(window);
	// Clean other resources
	// Close GL context and any other GLFW resources.
	glfwTerminate();
	sr_gui_cleanup();
	return 0;
}


