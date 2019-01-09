#ifndef State_h
#define State_h
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <map>


#define MIDIVIZ_VERSION_MAJOR 3
#define MIDIVIZ_VERSION_MINOR 2


	
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
		float minorsWidth;
		bool hLines;
		bool vLines;
		bool digits;
		bool keys;
	};
	
	
	struct ParticlesState {
		glm::vec3 color ;
		std::vector<GLuint> texs;
		float speed;
		float expansion ;
		float scale;
		int count;
	};
	
	BackgroundState background;
	ParticlesState particles;
	Quality::Level quality;
	glm::vec3 baseColor;
	float scale;
	float attenuation;
	bool showParticles;
	bool showFlashes;
	bool showBlur;
	bool showBlurNotes;
	bool lockParticleColor;
	
	void load(const std::string & path);
	
	void save(const std::string & path);
	
	void reset();
	
};




#endif
