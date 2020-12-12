#ifndef Configuration_h
#define Configuration_h

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>

typedef std::map<std::string, std::vector<std::string>> Arguments;

// Helper to trim characters from both ends of a string.
std::string trim(const std::string & str, const std::string & del);

class Configuration {

public:

	static Arguments parseArguments(std::istream & configFile);

	/** Will filter arguments without values. */
	static Arguments parseArguments(const std::vector<std::string> & argv, bool & showHelp, bool & showVersion);

	static bool parseBool(const std::string & str);

	static int parseInt(const std::string & str);

	static float parseFloat(const std::string & str);

	static glm::vec3 parseVec3(const std::vector<std::string> & strs);

private:


};

#endif
