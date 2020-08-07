#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <lodepng/lodepng.h>

void printHelp(){
	std::cout << "---- Infos ---- MIDIVisualizer Packager --------" << std::endl
	<< "Generate header and source files for resources images and shaders." << std::endl
	<< "Usage: packager path/to/repo/root/dir/" << std::endl
	<< "--------------------------------------------" << std::endl;

}

void flipImage(std::vector<unsigned char> & image, const int width, const int height) {
	// Compute the number of components per pixel.
	int components = int(image.size()) / (width * height);
	// The width in bytes.
	int widthInBytes = width * components;
	int halfHeight = height / 2;

	// For each line of the first half, we swap it with the mirroring line, starting from the end of the image.
	for (int h = 0; h < halfHeight; h++) {
		std::swap_ranges(image.begin() + h * widthInBytes, image.begin() + (h + 1) * widthInBytes, image.begin() + (height - h - 1) * widthInBytes);
	}
}

int main( int argc, char** argv) {
	
	if (argc < 2) {
		printHelp();
		return 0;
	}
	const std::string baseDir(argv[1]);

	const std::string resourcesDir = baseDir + "/resources/";
	const std::string outputDir = baseDir + "/src/resources/";
	
	std::vector<std::string> imagesToLoad = { "flash", "font", "particles"};
	std::vector<std::string> shadersToLoad = { "background", "flashes", "notes", "particles", "particlesblur", "screenquad", "keys", "backgroundtexture", "pedal", "wave", "fxaa"};
	
	// Header file.
	std::ofstream headerFile(outputDir + "data.h");
	if(!headerFile.is_open()){
		std::cerr << "Unable to open handle to header file." << std::endl;
		return 1;
	}
	
	headerFile << "#ifndef DATA_RESOURCES_H" << "\n"
			   << "#define DATA_RESOURCES_H" << "\n" << "\n"
			   << "#include <string>" << "\n"
			   << "#include <map>" << "\n"
			   << "#include <vector>" << "\n" << "\n"
			   << "extern const std::map<std::string, std::string> shaders;" << "\n" << "\n";
	
	
	// Each image has its own cpp + a line in the header file.
	for(const auto & imageName : imagesToLoad){
		const std::string imagePath = resourcesDir + imageName + ".png";
		const std::string outputPath = outputDir + imageName + "_image.cpp";
		
		// Load the image.
		std::vector<unsigned char> image;
		unsigned imwidth, imheight;
		unsigned error = lodepng::decode(image, imwidth, imheight, imagePath);
		if(error != 0){
			std::cerr << "Unable to load the image at path " << imagePath << "." << std::endl;
			continue;
		}
		flipImage(image, imwidth, imheight);

		// Definition in the header.
		headerFile << "extern  unsigned char " << imageName << "_image[" << (imwidth*imheight*4) << "];" << "\n";
		
		// Write the values in the cpp file
		std::ofstream outputFile(outputPath);
		if(outputFile.is_open()){
			outputFile << "#include \"data.h\"" << "\n";
			outputFile << "unsigned char " << imageName << "_image[] = { ";
			for(size_t pid = 0; pid < image.size(); ++pid){
				outputFile << (pid == 0 ? "" : ", ") << int(image[pid]);
			}
			outputFile << "};" << "\n";
			outputFile.close();
		} else {
			std::cerr << "Unable to open handle to source file for " << imageName << "." << std::endl;
		}
	}
	
	headerFile << "\n#endif\n";
	headerFile.close();
	
	// Now the shaders.
	std::ofstream shadersOutput(outputDir + "shaders.cpp");
	if(!shadersOutput.is_open()){
		std::cerr << "Unable to open handle to shaders output file." << std::endl;
		return 1;
	}
	shadersOutput << "#include \"data.h\"" << "\n"
				<< "const std::map<std::string, std::string> shaders = {\n";
	
	for(size_t sid = 0; sid < shadersToLoad.size(); ++sid){
		const auto & shaderName = shadersToLoad[sid];
		const std::string shaderBasePath = resourcesDir + "shaders/" + shaderName;
		std::ifstream vertShader(shaderBasePath + ".vert");
		std::ifstream fragShader(shaderBasePath + ".frag");
		if(!vertShader.is_open() || !fragShader.is_open()){
			std::cerr << "Unable to open handle to shaders input file for " << shaderName << "." << std::endl;
			continue;
		}
		std::string buffLine;
		
		// Vertex shader content.
		shadersOutput << "{ \"" << shaderName << "_" << "vert" << "\", \"";
		while (std::getline(vertShader, buffLine)) {
			if(buffLine.empty()){
				continue;
			}
			shadersOutput << buffLine << "\\n ";
		}
		shadersOutput << "\"}, " << "\n";
		
		// Fragment shader content.
		shadersOutput << "{ \"" << shaderName << "_" << "frag" << "\", \"";
		while (std::getline(fragShader, buffLine)) {
			if(buffLine.empty()){
				continue;
			}
			shadersOutput << buffLine << "\\n ";
		}
		shadersOutput << "\"}" << (sid == shadersToLoad.size()-1 ? "" : "," ) << "\n";
		
	}
	
	shadersOutput << "};" << "\n";
	shadersOutput.close();
	return 0;
}


