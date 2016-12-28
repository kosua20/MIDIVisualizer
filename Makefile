#Compiler: g++
CXX = g++

#Include directories (for headers): standard include dirs in /usr and /usr/local, and our helper directory.
INCLUDEDIR = -I/usr/include/ -I/usr/local/include/ -Isrc/helpers/
#Libraries needed: OpenGL, GLEW and glfw3. glfw3 requires Cocoa, IOKit and CoreVideo.
LIBS =  -framework OpenGL -lGLEW -lglfw3 -framework Cocoa -framework IOKit -framework CoreVideo

#Compiler flags: compile, we use the C++11 standard, and display 'all' warnings.
COMPFLAGS = -c -std=c++11 -Wall

#Build directory
BUILDDIR = build
#Source directory
SRCDIR = src

#Paths to the source files
SOURCES =  midi/MIDIUtils.cpp midi/MIDITrack.cpp midi/MIDIFile.cpp main.cpp rendering/Renderer.cpp rendering/Framebuffer.cpp rendering/ScreenQuad.cpp rendering/Background.cpp rendering/MIDIScene.cpp  rendering/camera/Camera.cpp rendering/camera/Keyboard.cpp helpers/ProgramUtilities.cpp helpers/MeshUtilities.cpp helpers/ResourcesManager.cpp helpers/lodepng/lodepng.cpp
#Paths to the objects files (generated from the sources)
OBJECTS = $(SOURCES:%.cpp=$(BUILDDIR)/%.o)

#Executable name
EXECNAME = midiviz

#Re-create the build dir if needed, compile and link.
all: dirs $(OBJECTS) $(EXECNAME)

#Linking phase: combine all objects files to generate the executable
$(EXECNAME): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $(BUILDDIR)/$(EXECNAME)
	@echo "\nCompilation finished."

#Compiling phase: generate the object files from the source files
$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(COMPFLAGS) $(INCLUDEDIR)  $< -o $@

#Run the executable
run:
	@./$(BUILDDIR)/$(EXECNAME)

#Create the build directory and its subdirectories
dirs:
	@mkdir -p $(BUILDDIR)/helpers/lodepng
	@mkdir -p $(BUILDDIR)/midi
	@mkdir -p $(BUILDDIR)/rendering/camera

#Remove the whole build directory
.PHONY: clean
clean :
	rm -r $(BUILDDIR)
