#ifndef State_h
#define State_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <map>


#define MIDIVIZ_VERSION_MAJOR 3
#define MIDIVIZ_VERSION_MINOR 5


	
struct Quality {
	enum Level : int {
		LOW_RES = 0,
		LOW,
		MEDIUM,
		HIGH,
		HIGH_RES
	};
	
	static const std::map<std::string, Level> names;
	static const std::map<Level, Quality> availables;
	
	std::string name = "MEDIUM";
	float particlesResolution = 0.5f;
	float blurResolution = 1.0f;
	float finalResolution = 1.0f;
	
	Quality() = default;
	
	Quality(const Quality::Level & alevel, const float partRes, const float blurRes, const float finRes);
};
	


class State {
public:
	struct BackgroundState {
		glm::vec3 color;
		glm::vec3 linesColor;
		glm::vec3 textColor;
		glm::vec3 keysColor;
		float minorsWidth;
		bool hLines;
		bool vLines;
		bool digits;
		bool keys;
	};
	
	
	struct ParticlesState {
		glm::vec3 color ;
		GLuint tex;
		int texCount;
		float speed;
		float expansion;
		float scale;
		int count;
	};
	
	BackgroundState background;
	ParticlesState particles;
	Quality::Level quality;
	glm::vec3 baseColor;
	glm::vec3 minorColor;
	glm::vec3 flashColor;
	float scale;
	float attenuation;
	float flashSize;
	float prerollTime;
	bool showParticles;
	bool showFlashes;
	bool showBlur;
	bool showBlurNotes;
	bool lockParticleColor;
	bool showNotes;
	bool showScore;
	
	void load(const std::string & path);
	
	void save(const std::string & path);
	
	void reset();
	
};




#endif
