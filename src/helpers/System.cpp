#include "System.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

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
#include <CoreFoundation/CoreFoundation.h>
extern "C" CFStringRef NSHomeDirectory( void );
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

#ifdef _WIN32

std::string System::getApplicationDataDirectory(){
	// %APPDATA%, ie D:/Users/name/AppData/
	PWSTR pathData = nullptr;
	HRESULT res = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &pathData);

	std::string path;
	if(res == S_OK){
		path = narrow(pathData);
	}
	CoTaskMemFree(pathData);
	// If the call above failed, this will return an empty path, the working directory.
	fixPathEnding(path);
	return path;
}

#elif defined(__APPLE__)

std::string System::getApplicationDataDirectory(){
	// /Users/name/Library/Application Support/
	// Retrieve the user application support path, relative to the home directory.
	sysdir_search_path_enumeration_state state = sysdir_start_search_path_enumeration(SYSDIR_DIRECTORY_APPLICATION_SUPPORT, SYSDIR_DOMAIN_MASK_USER);
	std::string path;
	char pathBuffer[PATH_MAX];
	while((state = sysdir_get_next_search_path_enumeration(state, pathBuffer))){
		path = std::string(pathBuffer);
		if(!path.empty()){
			// We found a valid one!
			break;
		}
	}
	// If nothing found, return the working directory.
	if(path.empty()){
		return "";
	}
	// Retrieve home directory
	CFStringGetCString((CFStringRef)NSHomeDirectory(), pathBuffer, sizeof(pathBuffer), kCFStringEncodingUTF8);
	std::string homePath(pathBuffer);
	fixPathEnding(homePath);
	// Expand ~ if present in the path.
	if(path[0] == '~'){
		path = homePath.substr(0, homePath.size()-1) + path.substr(1);
	}
	fixPathEnding(path);
	return path;
}

#else

std::string System::getApplicationDataDirectory(){
	// /home/name/.config/ or other, based on $HOME
	const char* envHome = std::getenv("HOME");
	if(envHome){
		std::string path = std::string(envHome);
		fixPathEnding(path);
		return path + ".config/";
	}
	// If nothing found, return the working directory.
	return "";
}

#endif

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
