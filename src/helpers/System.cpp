#include "System.h"
#include <sr_gui.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <clocale>

#include <GLFW/glfw3.h>

#ifdef _WIN32
#undef APIENTRY
#define NOMINMAX
#include <windows.h>
#include <shlobj.h>
#else
#include <sys/stat.h>
#endif

#ifdef __APPLE__
#include <sysdir.h>
#endif

// On Windows, we can notify both AMD and Nvidia drivers that we prefer discrete GPUs.
#ifdef _WIN32
extern "C" {
	// See https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
	_declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
	// See https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

void fixPathEnding(std::string& path){
	if(path.empty()){
		return;
	}
	if((path[path.size()-1] != '/') && (path[path.size()-1] != '\\')){
		path.append("/");
	}
}

void System::ping() {
	std::cout << '\a' << std::endl;
}

double System::time(){
	return glfwGetTime();
}

std::string System::timestamp(){
	const auto time = std::time(nullptr);
#ifdef _WIN32
	tm ltime = { 0,0,0,0,0,0,0,0,0 };
	localtime_s(&ltime, &time);
#else
	const tm ltime = *(std::localtime(&time));
#endif
	std::stringstream str;
	str << std::put_time(&ltime, "%Y_%m_%d_%H_%M_%S");
	return str.str();
}

#ifdef _WIN32

wchar_t * widen(const std::string & str) {
	const int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	WCHAR * arr	= new WCHAR[size];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, static_cast<LPWSTR>(arr), size);
	return arr;
}

std::string narrow(wchar_t * str) {
	const int size = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);
	std::string res(size - 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, &res[0], size, nullptr, nullptr);
	return res;
}

bool System::createDirectory(const std::string & directory) {
	wchar_t* str = widen(directory);
	const bool success = CreateDirectoryW(str, nullptr) != 0;
	delete[] str;
	return success;
}

std::ifstream System::openInputFile(const std::string& path, bool binary){
	wchar_t* str = widen(path);
	auto flags = binary ? std::ios::binary|std::ios::in : std::ios::in;
	std::ifstream file(str, flags);
	delete[] str;
	return file;
}

std::ofstream System::openOutputFile(const std::string& path, bool binary){
	wchar_t* str = widen(path);
	auto flags = binary ? std::ios::binary|std::ios::out : std::ios::out;
	std::ofstream file(str, flags);
	delete[] str;
	return file;
}

#else

bool System::createDirectory(const std::string & directory) {
	return mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
}

std::ifstream System::openInputFile(const std::string& path, bool binary){
	auto flags = binary ? std::ios::binary|std::ios::in : std::ios::in;
	std::ifstream file(path.c_str(), flags);
	return file;
}

std::ofstream System::openOutputFile(const std::string& path, bool binary){
	auto flags = binary ? std::ios::binary|std::ios::out : std::ios::out;
	std::ofstream file(path.c_str(), flags);
	return file;
}

#endif

std::string System::getApplicationDataDirectory(){
	char* path = nullptr;
	std::string pathStr;
	if( sr_gui_get_app_data_path( &path ) == SR_GUI_VALIDATED){
		if( path ){
			pathStr = std::string( path );
		}
	}
	if( path ){
		free( path );
	}
	return pathStr;
}

std::string System::loadStringFromFile(const std::string& path){
	std::ifstream file = System::openInputFile(path, false);
	if(!file.is_open()){
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	const std::string str = buffer.str();
	file.close();
	return str;
}

void System::writeStringToFile(const std::string& path, const std::string& content){
	std::ofstream file = System::openOutputFile(path, false);
	if(file.is_open()){
		file << content << std::endl;
		file.close();
	}
}

void System::forceLocale(){
	// Helps managing configuration files loading/saving.
	std::setlocale(LC_ALL, "C");
}
