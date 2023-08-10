#include "ProgramUtilities.h"
#include "../helpers/System.h"
#include "ResourcesManager.h"

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

void ShaderProgram::init(const std::string & vertexName, const std::string & fragmentName){
	const std::string vertexContent = ResourcesManager::getStringForShader(vertexName);
	const std::string fragmentContent = ResourcesManager::getStringForShader(fragmentName);
	_id = createGLProgramFromStrings(vertexContent, fragmentContent);
	_textures.clear();
	_uniforms.clear();
	// Get the number of active uniforms and their maximum length.
	// Note: this will also capture each attribute of each element of a uniform block (not used in MIDIViz for now)
	GLint count = 0;
	GLint size  = 0;
	glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &count);
	glGetProgramiv(_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &size);
	glUseProgram(_id);

	for(GLuint i = 0; i < GLuint(count); ++i) {
		// Get infos (name, name length, type,...) of each uniform.
		std::vector<GLchar> uname(size);
		GLenum utype;
		GLint usize		= 0;
		GLsizei ulength = 0;
		glGetActiveUniform(_id, i, size, &ulength, &usize, &utype, &uname[0]);
		std::string name(&uname[0]);
		// Skip empty or default uniforms (starting with 'gl_').
		if(usize == 0 || name.empty() || (name.size() > 3 && name.substr(0, 3) == "gl_")) {
			continue;
		}
		// If the size of the uniform is > 1, we have an array.
		if(usize > 1) {
			// Extract the array name from the 'name[0]' string.
			const std::string subname = name.substr(0, name.find_first_of('['));
			name = subname;
		}
		// Register uniform using its name.
		// /!\ the uniform location can be different from the uniform ID.
		const GLint location = glGetUniformLocation(_id, uname.data());

		// Store textures in their separate list.
		// Ignore more complex texture types for now
		if(utype == GL_SAMPLER_1D || utype == GL_SAMPLER_2D || utype == GL_SAMPLER_CUBE || utype == GL_SAMPLER_3D
		   || utype == GL_SAMPLER_1D_ARRAY || utype == GL_SAMPLER_2D_ARRAY || utype == GL_SAMPLER_CUBE_MAP_ARRAY){
			_textures[name] = location;
		} else {
			_uniforms[name] = location;
		}
		// Do we need additional info?
	}
	checkGLError();

	// Check we have no uniform blocks.
	glGetProgramiv(_id, GL_ACTIVE_UNIFORM_BLOCKS, &count);
	glGetProgramiv(_id, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &size);
	assert(count == 0);

	int textureSlot = 0;
	for(auto& texture : _textures){
		glUniform1i(texture.second, textureSlot);
		texture.second = textureSlot;
		++textureSlot;
	}

	checkGLError();
	glUseProgram(0);
}

void ShaderProgram::use(){
	glUseProgram(_id);
}

void ShaderProgram::clean(){
	glDeleteProgram(_id);
}

void ShaderProgram::texture(const std::string& name, GLuint texture, GLenum shape){
	auto tex = _textures.find(name);
	if(tex == _textures.end())
		return;
	
	const int texSlot = tex->second;
	glActiveTexture(GL_TEXTURE0 + texSlot);
	glBindTexture(shape, texture);
}

GLuint ShaderProgram::loadShader(const std::string & prog, GLuint type){
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

GLuint ShaderProgram::createGLProgramFromStrings(const std::string & vertexContent, const std::string & fragmentContent){
	GLuint vp(0), fp(0), id(0);
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
	checkGLError();
	//And deleting them
	glDeleteShader(vp);
	glDeleteShader(fp);
	
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if(channels == 1){
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_RED);
	}
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
			// Skip non existant file.
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
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if(channels == 1){
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_RED);
	}

	return textureId;
}

std::vector<GLuint> generate2DViewsOfArray(GLuint tex, unsigned int maxSize){
	glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
	int w, h, l, m, typedFormat;
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_WIDTH, &w);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_HEIGHT, &h);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_DEPTH, &l);
	glGetTexParameteriv(GL_TEXTURE_2D_ARRAY,  GL_TEXTURE_MAX_LEVEL, &m);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_INTERNAL_FORMAT, &typedFormat);
	int r,g,b,a;
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_RED_TYPE, &r);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_GREEN_TYPE, &g);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_BLUE_TYPE, &b);
	glGetTexLevelParameteriv(GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_ALPHA_TYPE, &a);
	int channelCount = (r != GL_NONE) + (g != GL_NONE) + (b != GL_NONE) + (a != GL_NONE);
	const GLint channelFormats[] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA};

	std::vector<GLuint> tex2Ds(l);
	glGenTextures(l, tex2Ds.data());

	for(unsigned int i = 0; i < l; ++i){
		glBindTexture(GL_TEXTURE_2D, tex2Ds[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if(channelCount == 1){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
		}

		// Allocate.
		glTexImage2D(GL_TEXTURE_2D, 0, typedFormat, maxSize, maxSize, 0, channelFormats[channelCount], GL_UNSIGNED_BYTE, nullptr);

		// Create two framebuffers.
		GLuint srcFb, dstFb;
		glGenFramebuffers(1, &srcFb);
		glGenFramebuffers(1, &dstFb);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, srcFb);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFb);

		glFramebufferTextureLayer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0, i);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2Ds[i], 0);

		glBlitFramebuffer(0, 0, w, h, 0, 0, maxSize, maxSize, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		// Restore the proper framebuffers from the cache.
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &srcFb);
		glDeleteFramebuffers(1, &dstFb);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	return tex2Ds;
}
