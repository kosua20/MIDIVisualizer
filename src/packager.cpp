#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <lodepng/lodepng.h>

void printHelp(){
	std::cout << "---- Infos ---- MIDIVisualizer Packager --------" << std::endl
	<< "Generate header and source files for resources images." << std::endl
	<< "Usage: run it from the root directory of the project." << std::endl
	<< "--------------------------------------------" << std::endl;

}

int main( int argc, char** argv) {
	
	const std::string resourcesDir = "ressources/";
	const std::string outputDir = "src/ressources/";
	
	std::vector<std::string> imagesToLoad = { "flash", "font", "particles"};
	
	// Header file.
	std::ofstream headerFile(outputDir + "data.h");
	if(!headerFile.is_open()){
		std::cerr << "Unable to open handle to header file." << std::endl;
		return 1;
	}
	
	headerFile << "#ifndef DATA_RESSOURCES_H" << std::endl
			   << "#define DATA_RESSOURCES_H" << std::endl << std::endl
			   << "#include <string>" << std::endl
			   << "#include <map>" << std::endl
			   << "#include <vector>" << std::endl << std::endl
			   << "extern const std::map<std::string, std::string> shaders;" << std::endl << std::endl;
	
	

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
		
		// Definition in the header.
		headerFile << "extern  unsigned char " << imageName << "_image[" << (imwidth*imheight*4) << "];" << std::endl;
		
		// Write the values in the cpp file
		std::ofstream outputFile(outputPath);
		if(outputFile.is_open()){
			outputFile << "#include \"data.h\"" << std::endl;
			outputFile << "unsigned char " << imageName << "_image[] = { ";
			for(size_t pid = 0; pid < image.size(); ++pid){
				outputFile << (pid == 0 ? "" : ", ") << int(image[pid]);
			}
			outputFile << "};" << std::endl;
			outputFile.close();
		} else {
			std::cerr << "Unable to open handle to source file for " << imageName << "." << std::endl;
		}
	}
	
	headerFile << "\n#endif\n";
	headerFile.close();
	
	
	return 0;
}


