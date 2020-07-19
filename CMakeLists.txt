cmake_minimum_required(VERSION 3.8)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

project(MIDIVisualizer)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Release")
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
# Need to look for local FindFFMPEG.cmake script, as none are provided by default.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/libs")


## Dependencies

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw/)

# Add NativeFileDialog
add_subdirectory(libs/nfd/)

# Add OpenGL
find_package(OpenGL REQUIRED)

# Add FFMPEG if available
find_package(FFMPEG)

## Projects

# Helper packager

set(PackagerSources 
	"src/libs/lodepng/lodepng.h" 
	"src/libs/lodepng/lodepng.cpp" 
	"src/packager.cpp" )

add_executable(Packager ${PackagerSources})
target_include_directories(Packager PRIVATE src/libs/)

# Target for packaging data in source files.
add_custom_target(Packaging
	COMMAND $<TARGET_FILE_DIR:Packager>/$<TARGET_FILE_NAME:Packager> ${PROJECT_SOURCE_DIR}
    DEPENDS Packager)


# MIDIVisualizer

file(GLOB GlmSources 
	"src/libs/glm/*.hpp" 
	"src/libs/glm/*/*.hpp" 
	"src/libs/glm/*/*.inl"
)

set(LibSources 
	"src/libs/gl3w/gl3w.cpp"
	"src/libs/gl3w/gl3w.h"
	"src/libs/gl3w/glcorearb.h"
	"src/libs/imgui/imconfig.h"
	"src/libs/imgui/imgui.cpp"
	"src/libs/imgui/imgui.h"
	"src/libs/imgui/imgui_demo.cpp"
	"src/libs/imgui/imgui_draw.cpp"
	"src/libs/imgui/imgui_impl_glfw.cpp"
	"src/libs/imgui/imgui_impl_glfw.h"
	"src/libs/imgui/imgui_impl_opengl3.cpp"
	"src/libs/imgui/imgui_impl_opengl3.h"
	"src/libs/imgui/imgui_internal.h"
	"src/libs/imgui/imgui_widgets.cpp"
	"src/libs/imgui/imstb_rectpack.h"
	"src/libs/imgui/imstb_textedit.h"
	"src/libs/imgui/imstb_truetype.h"
	"src/libs/lodepng/lodepng.h"
	"src/libs/lodepng/lodepng.cpp"
	"src/libs/stb_image/stb_image.h"
	"src/libs/stb_image/stb_image_resize.h"
	"src/libs/stb_image_write/stb_image_write.h" 
	${GlmSources}
)

set(Sources
	"src/helpers/MeshUtilities.cpp"
	"src/helpers/MeshUtilities.h"
	"src/helpers/ProgramUtilities.cpp"
	"src/helpers/ProgramUtilities.h"
	"src/helpers/ResourcesManager.cpp"
	"src/helpers/ResourcesManager.h"
	"src/helpers/Recorder.cpp"
	"src/helpers/Recorder.h"
	"src/helpers/Configuration.cpp"
	"src/helpers/Configuration.h"
	"src/midi/MIDIFile.cpp"
	"src/midi/MIDIFile.h"
	"src/midi/MIDITrack.cpp"
	"src/midi/MIDITrack.h"
	"src/midi/MIDIUtils.cpp"
	"src/midi/MIDIUtils.h"
	"src/rendering/Score.cpp"
	"src/rendering/Score.h"
	"src/rendering/Framebuffer.cpp"
	"src/rendering/Framebuffer.h"
	"src/rendering/MIDIScene.cpp"
	"src/rendering/MIDIScene.h"
	"src/rendering/Renderer.cpp"
	"src/rendering/Renderer.h"
	"src/rendering/ScreenQuad.cpp"
	"src/rendering/ScreenQuad.h"
	"src/rendering/State.cpp"
	"src/rendering/State.h"
	"src/rendering/camera/Camera.cpp"
	"src/rendering/camera/Camera.h"
	"src/rendering/camera/Keyboard.cpp"
	"src/rendering/camera/Keyboard.h"
	"src/resources/data.h"
	"src/resources/flash_image.cpp"
	"src/resources/font_image.cpp"
	"src/resources/particles_image.cpp"
	"src/resources/shaders.cpp"
	"src/main.cpp")

file(GLOB Shaders "resources/shaders/*.vert" "resources/shaders/*.frag")

source_group(TREE ${PROJECT_SOURCE_DIR}/src FILES ${Sources} ${LibSources})
source_group(TREE ${PROJECT_SOURCE_DIR}/resources FILES ${Shaders})

add_executable(MIDIVisualizer ${LibSources} ${Sources} ${Shaders})

target_include_directories(MIDIVisualizer PRIVATE src/libs/ src/helpers/)
target_link_libraries(MIDIVisualizer PRIVATE nfd glfw ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY})
add_dependencies(MIDIVisualizer Packaging)

# Add dependency to FFmpeg if available.
if(FFMPEG_FOUND)
	message(STATUS "FFmpeg found, enabling video export.")
	target_compile_definitions(MIDIVisualizer PRIVATE MIDIVIZ_SUPPORT_VIDEO)
	target_include_directories(MIDIVisualizer PRIVATE ${FFMPEG_INCLUDE_DIRS})
	target_link_libraries(MIDIVisualizer PRIVATE ${FFMPEG_LIBRARIES})
	target_compile_definitions(MIDIVisualizer PRIVATE ${FFMPEG_DEFINITIONS})
endif()
