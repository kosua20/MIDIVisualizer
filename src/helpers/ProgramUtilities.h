#ifndef ProgramUtilities_h
#define ProgramUtilities_h

#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

/// This macro is used to check for OpenGL errors with access to the file and line number where the error is detected.
#define checkGLError() _checkGLError(__FILE__, __LINE__)

/// Converts a GLenum error number into a human-readable string.
std::string getGLErrorString(GLenum error);

/// Check if any OpenGL error has been detected and log it.
int _checkGLError(const char *file, int line);

/// Load a shader of the given type from a string
GLuint loadShader(const std::string & prog, GLuint type);

/// Create a GLProgram using the hader code contained in the given files.
GLuint createGLProgram(const std::string & vertexPath, const std::string & fragmentPath, const std::string & geometryPath = "");

GLuint createGLProgramFromStrings(const std::string & vertexContent, const std::string & fragmentContent, const std::string & geometryContent = "");

// Texture loading.

// 2D texture.

GLuint loadTexture(const std::string& path, unsigned int channels, bool sRGB);

//GLuint loadTexture(const std::string& path, const GLuint program, const GLuint textureSlot, const std::string& uniformName, bool sRGB = false);

GLuint loadTexture( unsigned char* image, unsigned imwidth, unsigned imheight, unsigned int channels, bool sRGB);

GLuint loadTextureArray(const std::vector<std::string>& paths, bool sRGB, int & layers);

GLuint loadTextureArray(const std::vector<unsigned char*>& images, const std::vector<glm::ivec2> & sizes, unsigned int channels, bool sRGB);

std::vector<GLuint> generate2DViewsOfArray(GLuint tex, unsigned int maxSize);

// Cubemap texture.
//GLuint loadTextureCubeMap(const std::string& pathBase, bool sRGB);

//GLuint loadTextureCubeMap(const std::string& pathBase, const GLuint program, const GLuint textureSlot, const std::string& uniformName, bool sRGB = false);


#endif
