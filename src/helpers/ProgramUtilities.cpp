#include "ProgramUtilities.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image/stb_image_resize.h>


std::string getGLErrorString(GLenum error) {
	std::string msg;
	switch (error) {
		case GL_INVALID_ENUM:
			msg = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			msg = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			msg = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_NO_ERROR:
			msg = "GL_NO_ERROR";
			break;
		case GL_OUT_OF_MEMORY:
			msg = "GL_OUT_OF_MEMORY";
			break;
		default:
			msg = "UNKNOWN_GL_ERROR";
	}
	return msg;
}

int _checkGLError(const char *file, int line){
	GLenum glErr = glGetError();
	if (glErr != GL_NO_ERROR){
		std::cerr << "[GL]: Error in " << file << " (" << line << ") : " << getGLErrorString(glErr) << std::endl;
		return 1;
	}
	return 0;
}

std::string loadStringFromFile(const std::string & filename) {
	std::ifstream in;
	// Open a stream to the file.
	in.open(filename.c_str());
	if (!in) {
		std::cerr << "[ERROR]: " << filename + " is not a valid file." << std::endl;
		return "";
	}
	std::stringstream buffer;
	// Read the stream in a buffer.
	buffer << in.rdbuf();
	// Create a string based on the content of the buffer.
	std::string line = buffer.str();
	in.close();
	return line;
}

GLuint loadShader(const std::string & prog, GLuint type){
	GLuint id;
	// Create shader object.
	id = glCreateShader(type);
	checkGLError();
	// Setup string as source.
	const char * shaderProg = prog.c_str();
	glShaderSource(id,1,&shaderProg,(const GLint*)NULL);
	// Compile the shader on the GPU.
	glCompileShader(id);
	checkGLError();

	GLint success;
	glGetShaderiv(id,GL_COMPILE_STATUS, &success);
	
	// If compilation failed, get information and display it.
	if (success != GL_TRUE) {
		GLint infoLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> infoLog((std::max)(infoLogLength, int(1)));
		glGetShaderInfoLog(id, infoLogLength, NULL, &infoLog[0]);

		std::cerr << std::endl 
					<< "*--- " 
					<< (type == GL_VERTEX_SHADER ? "Vertex" : (type == GL_FRAGMENT_SHADER ? "Fragment" : "Geometry (or tess.)")) 
					<< " shader failed to compile ---*" 
					<< std::endl
					<< &infoLog[0]
					<< "*---------------------------------*" 
					<< std::endl << std::endl;
	}
	// Return the id to the successfuly compiled  shader program.
	return id;
}

GLuint createGLProgram(const std::string & vertexPath, const std::string & fragmentPath, const std::string & geometryPath){
	
	std::string vertexCode = loadStringFromFile(vertexPath);
	std::string fragmentCode = loadStringFromFile(fragmentPath);
	std::string geometryCode = "";
	
	if(!geometryPath.empty()) {
		geometryCode = loadStringFromFile(geometryPath);
	}
	
	return createGLProgramFromStrings(vertexCode, fragmentCode, geometryCode);
}

GLuint createGLProgramFromStrings(const std::string & vertexContent, const std::string & fragmentContent, const std::string & geometryContent){
	GLuint vp(0), fp(0), gp(0), id(0);
	id = glCreateProgram();
	checkGLError();
	std::string vertexCode = vertexContent;
	std::string fragmentCode = fragmentContent;
	
	// If vertex program code is given, compile it.
	if (!vertexCode.empty()) {
		vp = loadShader(vertexCode,GL_VERTEX_SHADER);
		glAttachShader(id,vp);
	}
	// If fragment program code is given, compile it.
	if (!fragmentCode.empty()) {
		fp = loadShader(fragmentCode,GL_FRAGMENT_SHADER);
		glAttachShader(id,fp);
	}
	// If geometry program filepath exists, load it and compile it.
	std::string geometryCode = geometryContent;
	if (!geometryCode.empty()) {
		gp = loadShader(geometryCode,GL_GEOMETRY_SHADER);
		glAttachShader(id,gp);
	}
	
	
	// Link everything
	glLinkProgram(id);
	checkGLError();
	//Check linking status.
	GLint success = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	
	// If linking failed, query info and display it.
	if(!success) {
		GLint infoLogLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> infoLog((std::max)(infoLogLength, int(1)));
		glGetProgramInfoLog(id, infoLogLength, NULL, &infoLog[0]);
		
		std::cerr << "[GL]: Failed loading program: " << &infoLog[0] << std::endl;
		return 0;
	}
	// We can now clean the shaders objects, by first detaching them
	if (vp != 0) {
		glDetachShader(id,vp);
	}
	if (fp != 0) {
		glDetachShader(id,fp);
	}
	if (gp != 0) {
		glDetachShader(id,gp);
	}
	checkGLError();
	//And deleting them
	glDeleteShader(vp);
	glDeleteShader(fp);
	glDeleteShader(gp);
	
	glUseProgram(id);
	checkGLError();
	// Return the id to the succesfuly linked GLProgram.
	return id;
}

GLuint loadTexture(const std::string& path, unsigned int channels, bool sRGB){

	// Load and upload the texture.
	int imwidth, imheight, nChans;
	stbi_set_flip_vertically_on_load(true);
	unsigned char * image = stbi_load(path.c_str(), &imwidth, &imheight, &nChans, channels);
	if(image == NULL){
		std::cerr << "[GL]: Unable to load the texture at path " << path << "." << std::endl;
		return 0;
	}
	stbi_set_flip_vertically_on_load(false);
	GLuint textureId = loadTexture(image, imwidth, imheight, channels, sRGB);
	stbi_image_free(image);
	return textureId;
}

GLuint loadTexture( unsigned char* image, unsigned imwidth, unsigned imheight, unsigned int channels, bool sRGB){
	
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	const GLenum format = channels == 1 ? GL_RED : (channels == 3 ? GL_RGB : GL_RGBA);
	const GLenum typedFormat = (channels == 4 && sRGB) ? GL_SRGB8_ALPHA8 : format;
	glTexImage2D(GL_TEXTURE_2D, 0, typedFormat, imwidth , imheight, 0, format, GL_UNSIGNED_BYTE, &(image[0]));
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	return textureId;
}

GLuint loadTextureArray(const std::vector<std::string>& paths, bool sRGB, int & layers) {

	std::vector<unsigned char *> images;
	std::vector <glm::ivec2> sizes;

	stbi_set_flip_vertically_on_load(true);

	for (size_t i = 0; i < paths.size(); ++i) {
		const auto & path = paths[i];
		int nChans;
		glm::ivec2 size(0);
		unsigned char * image = stbi_load(path.c_str(), &size[0], &size[1], &nChans, 1);
		if (image == NULL) {
			// Skip non existent file.
			std::cerr << "[GL]: " << "Unable to load the texture at path " << path << "." << std::endl;
			continue;
		}
		images.push_back(image);
		sizes.push_back(size);
	}
	stbi_set_flip_vertically_on_load(false);
	layers = int(images.size());
	GLuint textureId = loadTextureArray(images, sizes, 1, sRGB);
	// Free images data.
	for (int i = 0; i < int(images.size()); ++i) {
		stbi_image_free(images[i]);
	}
	return textureId;
}

GLuint loadTextureArray(const std::vector<unsigned char*>& images, const std::vector<glm::ivec2>& sizes, unsigned int channels, bool sRGB){
	// Cmopute max size.
	glm::ivec2 maxSize(0);
	for (int i = 0; i < int(images.size()); ++i) {
		maxSize = glm::max(maxSize, sizes[i]);
	}
	const GLenum format = channels == 1 ? GL_RED : (channels == 3 ? GL_RGB : GL_RGBA);
	const GLenum typedFormat = (channels == 4 && sRGB) ? GL_SRGB8_ALPHA8 : format;
	
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureId);
	// Allocate.
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, typedFormat, maxSize[0], maxSize[1], GLsizei(images.size()), 0, format, GL_UNSIGNED_BYTE, nullptr);
	// Upload each layer, resizing it first.
	for (int i = 0; i < int(images.size()); ++i) {
		unsigned char * resizedImage = images[i];
		const bool resize = sizes[i][0] != maxSize[0] || sizes[i][1] != maxSize[1];
		if (resize) {
			// Upscale.
			resizedImage = new unsigned char[maxSize[0] * maxSize[1] * channels];
			stbir_resize_uint8(images[i], sizes[i][0], sizes[i][1], 0, resizedImage, maxSize[0], maxSize[1], 0, channels);
		}
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, maxSize[0], maxSize[1], 1, format, GL_UNSIGNED_BYTE, resizedImage);
		if (resize) {
			delete[] resizedImage;
		}
	}
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureId;
}
