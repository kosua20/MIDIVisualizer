#ifndef Configuration_h
#define Configuration_h

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

typedef std::unordered_map<std::string, std::vector<std::string>> Arguments;

// Helper to trim characters from both ends of a string.
std::string trim(const std::string & str, const std::string & del);

std::string join(const std::vector<std::string>& strs, const std::string& delim);

struct Export {

	enum class Format : int {
		   PNG = 0, MPEG2 = 1, MPEG4 = 2, PRORES = 3
	};

	std::string path;
	Format format = Format::PNG;
	float postroll = 10.0f;
	int framerate = 60;
	int bitrate = 40;
	bool fixPremultiply = false;
	bool alphaBackground = false;

};

class Configuration {

public:

	Configuration(const std::string& path, const std::vector<std::string>& argv);

	void save(const std::string& path);

	const Arguments& args() const { return _args; }

	static Arguments parseArguments(std::istream & configFile);

	/** Will filter arguments without values. */
	static Arguments parseArguments(const std::vector<std::string> & argv);

	static bool parseBool(const std::string & str);

	static int parseInt(const std::string & str);

	static float parseFloat(const std::string & str);

	static glm::vec3 parseVec3(const std::vector<std::string> & strs);

	static void printVersion();

	static void printHelp();

	static std::string defaultName();
	
public:

	// General settings (will be saved)
	std::string lastMidiPath;
	std::string lastMidiDevice;
	std::string lastConfigPath;
	glm::ivec2 windowSize = { 1280, 600 };
	glm::ivec2 windowPos = {100, 100};
	float guiScale = 1.0f;
	bool fullscreen = false;
	bool hideWindow = false;
	bool preventTransparency = false;
	bool useTransparency = false;
	bool showVersion = false;
	bool showHelp = false;

	// Export settings (won't be saved)
	Export exporting;

private:

	Arguments _args;

};

#endif
