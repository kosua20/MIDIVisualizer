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
#define MIDIVIZ_VERSION_MINOR 1

#define COLUMN_SIZE 170
#define EXPORT_COLUMN_SIZE 200

typedef std::array<glm::vec3, SETS_COUNT> ColorArray;
typedef std::vector<std::string> PathCollection;

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
	
struct FilterOptions
{
	std::vector<bool> tracks; // Will be resized to max track index + 1
	std::array<bool, 16> channels;

	FilterOptions();

	void fillChannelsFromTokens( const std::vector<std::string>& list, bool enabled);

	void fillTracksFromTokens( const std::vector<std::string>& list, bool enabled );

	std::string toHiddenChannelsString();

	std::string toHiddenTracksString();

	bool accepts( int track, int channel ) const;

};


class State {
public:
	struct BackgroundState {
		PathCollection imagePath; ///< Path to an image on disk (expect only one).
		glm::vec3 color; ///< Background color.
		float minorsWidth; ///< Minor keys and notes width.
		bool image; ///< Use background image.
		bool imageBehindKeyboard; ///< Should image pass behind keyboard.
		float imageAlpha; ///< Background alpha.
		GLuint tex;
	};

	struct ScoreState {
		glm::vec3 hLinesColor; ///< Score lines color.
		glm::vec3 vLinesColor; ///< Score lines color.
		glm::vec3 digitsColor; ///< Score text
		glm::vec2 digitsOffset; /// < Offset in fraction of a digit.
		float hLinesWidth; ///< Width in pixels
		float vLinesWidth; ///< Width in pixels
		float digitsScale; ///< Scale
		bool hLines; ///< Show horizontal score lines.
		bool vLines; ///< Show vertical score lines.
		bool digits; ///< Show score text.
	};
	
	struct ParticlesState {
		PathCollection imagePaths; ///< List of paths to images on disk.
		ColorArray colors; ///< Particles color.
		GLuint tex;
		int texCount; ///< Number of particle textures.
		float speed; ///< Particles speed.
		float expansion; ///< Expansion factor.
		float scale; ///< Particles scale.
		int count; ///< Number of particles.
	};

	struct KeyboardState {
		ColorArray majorColor; ///< Major key pressed color.
		ColorArray minorColor; ///< Minor key pressed color.
		glm::vec3 edgeColor; ///< Black keys color.
		float size; ///< Size on screen, starting from the bottom.
		float minorHeight; ///< Fraction of the keyboard height taken by the minor keys.
		bool highlightKeys; ///< Highlight pressed keys.
		bool customKeyColors; ///< Use the custom colors above instead of the color of the notes.
		bool minorEdges; ///< Show edges on minor keys.
	};

	struct PedalsState {

		enum Location : int {
			TOPLEFT = 0, BOTTOMLEFT = 1, TOPRIGHT = 2, BOTTOMRIGHT = 3
		};

		PathCollection centerImagePath; ///< Image for center pedal.
		PathCollection topImagePath; ///< Image for top pedal.
		PathCollection sideImagePaths; ///< Image(s) for side pedals
		GLuint texCenter;
		GLuint texTop;
		GLuint texSides[2];

		glm::vec3 centerColor; ///< Pedals color
		glm::vec3 topColor; ///< Pedals color
		glm::vec3 leftColor; ///< Pedals color
		glm::vec3 rightColor; ///< Pedals color
		glm::vec2 margin; ///< Offset between pedals
		Location location; ///< Corner location on screen
		float size; ///< Pedals size
		float opacity; ///< Pedals opacity
		bool merge; // Display only one pedal for all effects
		bool mirror; // Mirror the right pedal horizontally.
	};

	struct WaveState {
		glm::vec3 color;
		float opacity;
		float spread;
		float amplitude;
		float frequency;
		float speed;
		float noiseIntensity;
		float noiseSize;
	};

	struct NotesState {
		std::vector<std::string> majorImagePath; ///< Path to major notes background texture.
		std::vector<std::string> minorImagePath; ///< Path to minor notes background texture.
		ColorArray majorColors; ///< Major notes color.
		ColorArray minorColors; ///< Minor notes color.
		float fadeOut; ///< Notes fade out at the top.
		float edgeWidth; ///< Edges drawn around the notes.
		float edgeBrightness; ///< Factor applied around the notes edges.
		float cornerRadius; ///< Notes rounding factor.
		float majorTexScale; ///< Scale for the major texture
		float minorTexScale; ///< Scale for the minor texture
		float majorTexAlpha; ///< Intensity for the major texture
		float minorTexAlpha; ///< Intensity for the minor texture
		bool majorTexScroll; ///< Scrolling for the major texture
		bool minorTexScroll; ///< Scrolling for the minor texture
		GLuint majorTex;
		GLuint minorTex;
	};

	struct FlashesState {
		std::vector<std::string> imagePath;
		ColorArray colors; ///< Flashes color.
		float size; ///< Size of float
		float haloInnerRadius; ///< Inner halo size
		float haloOuterRadius; ///< Outer halo size
		float haloIntensity; ///< Brightness of the halo
		int texRowCount; ///< Number of rows in atlas
		int texColCount; ///< Number of columns in atlas
		GLuint tex;
	};

	BackgroundState background;
	ScoreState score;
	ParticlesState particles;
	KeyboardState keyboard;
	SetOptions setOptions;
	PedalsState pedals;
	WaveState waves;
	NotesState notes;
	FlashesState flashes;
	// TODO: (MV) Regroup common state to pass to rendering functions.
	FilterOptions filter;

	Quality::Level quality;
	float scale; ///< Display vertical scale.
	float attenuation; ///< Blur attenuation.

	float prerollTime; ///< Preroll time.
	float scrollSpeed; ///< Playback speed.

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
	bool perSetColors;
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

	void synchronizeSets();

	const std::string& filePath() const;

	static size_t helpText(std::string & configOpts);

private:

	static void defineOptions();

	void updateOptions();

	// Legacy loading.
	void loadLegacy(std::istream & configFile, int majVersion, int minVersion);

	struct OptionInfos {

		enum class Type {
			BOOLEAN, INTEGER, FLOAT, COLOR, OTHER, KEY, PATH
		};

		enum class Category {
			GENERAL = 0, EFFECTS, PLAYBACK, NOTES, FLASHES, PARTICLES, KEYBOARD, PEDAL, WAVE, SCORE, BACKGROUND, PER_SETS, COUNT
		};

		std::string description;
		std::string values;
		Type type;
		std::array<float, 2> range;
		Category category;
		bool legacy = false;

		OptionInfos();

		OptionInfos(Category acategory, const std::string & adesc, Type atype, const std::array<float, 2> & arange = {0.0f, 0.0f});
	};

	static std::unordered_map<std::string, OptionInfos> _sharedInfos;
	std::unordered_map<std::string, bool*> _boolInfos;
	std::unordered_map<std::string, int*> _intInfos;
	std::unordered_map<std::string, float*> _floatInfos;
	std::unordered_map<std::string, glm::vec3*> _vecInfos;
	std::unordered_map<std::string, std::vector<std::string>*> _pathInfos;
	std::string _filePath;

};




#endif
