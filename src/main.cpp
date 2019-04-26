#include <gl3w/gl3w.h> // to load OpenGL extensions at runtime
#include <GLFW/glfw3.h> // to set up the OpenGL context and manage window lifecycle and inputs
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <nfd/nfd.h>
#include <stdio.h>
#include <iostream>

#include "helpers/ProgramUtilities.h"

#include "rendering/Renderer.h"

#define INITIAL_SIZE_WIDTH 1280
#define INITIAL_SIZE_HEIGHT 600


void printHelp(){
	std::cout << "---- Infos ---- MIDIVisualizer v" << MIDIVIZ_VERSION_MAJOR << "." << MIDIVIZ_VERSION_MINOR << " --------" << std::endl
	<< "Visually display a midi file in realtime." << std::endl
	<< "Usage: midiviz path/to/file.mid [s] [r g b]" << std::endl
	<< "\t(where s is a scaling factor and r,g,b are float color components in [0,1])" << std::endl
	<< "Keys:\tp\tplay/pause" << std::endl
	<< "\tr\treset" << std::endl
	<< "--------------------------------------------" << std::endl;

}
/// Callbacks

void resize_callback(GLFWwindow* window, int width, int height){
	Renderer *renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	renderer->resize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	
	// Handle quitting
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){ 
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
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

/// The main function

int main( int argc, char** argv) {
	
	if(argc > 3 && argc != 5 && argc != 6){
		std::cerr << "[ERROR]: wrong number of arguments" << std::endl;
		return 1;
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

	// Create a window with a given size. Width and height are macros as we will need them again.
	GLFWwindow* window = glfwCreateWindow(INITIAL_SIZE_WIDTH, INITIAL_SIZE_HEIGHT,"MIDI Visualizer", NULL, NULL);
	if (!window) {
		std::cerr << "[ERROR]: could not open window with GLFW3" << std::endl;
		glfwTerminate();
		return 2;
	}

	// Bind the OpenGL context and the new window.
	glfwMakeContextCurrent(window);

	if (gl3wInit()) {
		std::cerr << "Failed to initialize OpenGL" << std::endl;
		return -1;
	}
	if (!gl3wIsSupported(3, 2)) {
		std::cerr << "OpenGL 3.2 not supported\n" << std::endl;
		return -1;
	}
	
	// Read arguments.
	std::string midiFilePath;
	if(argc<2){
		// We are in direct-to-gui mode.
		nfdchar_t *outPath = NULL;
		nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
		if(result == NFD_OKAY){
			midiFilePath = std::string(outPath);
		} else if(result == NFD_CANCEL){
			return 0;
		} else {
			return 10;
		}
	} else {
		// We are in command-line mode.
		printHelp();
		midiFilePath = std::string(argv[1]);
	}
	glm::vec3 baseColor = 1.35f*glm::vec3(0.57f,0.19f,0.98f);
	float scale = 0.5;
	if(argc == 3 || argc == 6 ){
		scale = std::stof(argv[2]);
	}
	if(argc == 5){
		baseColor[0] = std::stof(argv[2]);
		baseColor[1] = std::stof(argv[3]);
		baseColor[2] = std::stof(argv[4]);
	} else if(argc == 6){
		baseColor[0] = std::stof(argv[3]);
		baseColor[1] = std::stof(argv[4]);
		baseColor[2] = std::stof(argv[5]);
	}
	
	// Create the renderer.
	Renderer renderer;
	renderer.init(INITIAL_SIZE_WIDTH,INITIAL_SIZE_HEIGHT);
	renderer.setColorAndScale(baseColor, scale);
	
	try {
		// Load midi file, graphics setup.
		renderer.loadFile(midiFilePath);
	} catch (...) {
		// File not found, probably (error message handled locally).
		glfwDestroyWindow(window);
		renderer.clean();
		glfwTerminate();
		return 3;
	}
	
	// Define utility pointer for callbacks (can be obtained back from inside the callbacks).
	glfwSetWindowUserPointer(window, &renderer);
	// Callbacks.
	glfwSetFramebufferSizeCallback(window, resize_callback);	// Resizing the window
	glfwSetKeyCallback(window,key_callback);					// Pressing a key
	//glfwSetMouseButtonCallback(window,mouse_button_callback);	// Clicking the mouse buttons
	//glfwSetCursorPosCallback(window,cursor_pos_callback);		// Moving the cursor
	glfwSetScrollCallback(window,scroll_callback);				// Scrolling
	glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	glfwSwapInterval(1);
	// On HiDPI screens, we might have to initially resize the framebuffers size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	renderer.resize(width, height);
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, false);
	ImGui::StyleColorsDark();
	ImGui::GetStyle().FrameRounding = 3;
	
	io.IniFilename = NULL;
	
	// Start the display/interaction loop.
	while (!glfwWindowShouldClose(window)) {
		ImGui_ImplGlfwGL3_NewFrame();
		
		// Update the content of the window.
		renderer.draw(glfwGetTime());
		
		// Interface rendering.
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		//Display the result fo the current rendering loop.
		glfwSwapBuffers(window);
		// Update events (inputs,...).
		glfwPollEvents();
		
	}
	
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	// Remove the window.
	glfwDestroyWindow(window);
	// Clean other resources
	renderer.clean();
	// Close GL context and any other GLFW resources.
	glfwTerminate();
	return 0;
}


