#ifndef State_h
#define State_h

#include "../helpers/Configuration.h"
//#include "../midi/MIDIUtils.h"
#include "SetOptions.h"

#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <array>

#define MIDIVIZ_VERSION_MAJOR 7
#define MIDIVIZ_VERSION_MINOR 0

#define COLUMN_SIZE 170
#define EXPORT_COLUMN_SIZE 200

typedef std::array<glm::vec3, CHANNELS_COUNT> ColorArray;

struct Quality {
	enum Level : int {
		LOW_RES = 0,
		LOW,
		MEDIUM,
		HIGH,
		HIGH_RES
	};
	
	static const std::unordered_map<std::string, Level> names;
	static const std::unordered_map<Level, Quality> availables;
	
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
		std::string imagePath; ///< Path to an image on disk.
		glm::vec3 color; ///< Background color.
		glm::vec3 linesColor; ///< Score lines color.
		glm::vec3 textColor; ///< Score text color.
		glm::vec3 keysColor; ///< Black keys color.
		float minorsWidth; ///< Minor keys and notes width.
		bool hLines; ///< Show horizontal score lines.
		bool vLines; ///< Show vertical score lines.
		bool digits; ///< Show score text.
		bool image; ///< Use background image.
		bool imageBehindKeyboard; ///< Should image pass behind keyboard.
		float imageAlpha; ///< Background alpha.
		GLuint tex;
	};
	
	
	struct ParticlesState {
		std::string imagePaths; ///< List of paths to images on disk.
		ColorArray colors; ///< Particles color.
		GLuint tex;
		int texCount;
		float speed; ///< Particles speed.
		float expansion; ///< Expansion factor.
		float scale; ///< Particles scale.
		int count; ///< Number of particles.
	};

	struct KeyboardState {
		ColorArray majorColor; ///< Major key pressed color.
		ColorArray minorColor; ///< Minor key pressed color.
		float size; ///< Size on screen, starting from the bottom.
		bool highlightKeys; ///< Highlight pressed keys.
		bool customKeyColors; ///< Use the custom colors above instead of the color of the notes.
	};

	struct PedalsState {

		enum Location : int {
			TOPLEFT = 0, BOTTOMLEFT = 1, TOPRIGHT = 2, BOTTOMRIGHT = 3
		};

		glm::vec3 color;
		Location location;
		float size;
		float opacity;
		bool merge;
	};

	struct WaveState {
		glm::vec3 color;
		float opacity;
		float spread;
		float amplitude;
		float frequency;
	};
	
	BackgroundState background;
	ParticlesState particles;
	KeyboardState keyboard;
	SetOptions setOptions;
	PedalsState pedals;
	WaveState waves;
	
	Quality::Level quality;
	ColorArray baseColors; ///< Major notes color.
	ColorArray minorColors; ///< Minor notes color.
	ColorArray flashColors; ///< Flashes color.
	float scale; ///< Display vertical scale.
	float attenuation; ///< Blur attenuation.
	float flashSize; ///< Size of flashes.
	float prerollTime; ///< Preroll time.
	float scrollSpeed; ///< Playback speed.
	float notesFadeOut; ///< Notes fade out at the top.

	int minKey; ///< The lowest key to display.
	int maxKey; ///< The highest key to display.

	bool showParticles;
	bool showFlashes;
	bool showBlur;
	bool showBlurNotes;
	bool lockParticleColor;
	bool showNotes;
	bool showScore;
	bool showKeyboard;
	bool perChannelColors;
	bool showPedal;
	bool showWave;
	bool applyAA;
	bool reverseScroll;
	bool horizontalScroll;

	std::array<int, 16> layersMap; ///< Location of each layer.

	State();

	bool load(const std::string & path);

	void load(const Arguments & configArgs);

	void save(const std::string & path);
	
	void reset();

	void synchronizeChannels();

	const std::string& filePath() const;

	static size_t helpText(std::string & configOpts, std::string & setsOpts);

private:

	static void defineOptions();

	void updateOptions();

	// Legacy loading.
	void load(std::istream & configFile, int majVersion, int minVersion);

	struct OptionInfos {

		enum class Type {
			BOOLEAN, INTEGER, FLOAT, COLOR, OTHER, KEY, PATH
		};

		enum class Category {
			DEFAULT, SETS
		};

		std::string description;
		std::string values;
		Type type;
		std::array<float, 2> range;
		Category category = Category::DEFAULT;

		OptionInfos();

		OptionInfos(const std::string & adesc, Type atype, const std::array<float, 2> & arange = {0.0f, 0.0f});
	};

	static std::unordered_map<std::string, OptionInfos> _sharedInfos;
	std::unordered_map<std::string, bool*> _boolInfos;
	std::unordered_map<std::string, int*> _intInfos;
	std::unordered_map<std::string, float*> _floatInfos;
	std::unordered_map<std::string, glm::vec3*> _vecInfos;
	std::unordered_map<std::string, std::string*> _stringInfos;
	std::string _filePath;

};




#endif
