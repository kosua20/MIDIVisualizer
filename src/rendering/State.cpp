
#include "State.h"
#include "scene/MIDIScene.h"
#include "../helpers/ResourcesManager.h"
#include "../helpers/System.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


#define MAX_TRACK_COUNT 4096

std::unordered_map<std::string, State::OptionInfos> State::_sharedInfos;

// Quality names.
const std::unordered_map<std::string, Quality::Level> Quality::names = {
	{ "LOW_RES", Quality::LOW_RES },
	{ "LOW", Quality::LOW },
	{ "MEDIUM", Quality::MEDIUM },
	{ "HIGH", Quality::HIGH },
	{ "HIGH_RES", Quality::HIGH_RES }
};

const std::unordered_map<Quality::Level, Quality> Quality::availables = {
	{ Quality::LOW_RES, { Quality::LOW_RES, 0.5f, 0.5f, 0.5f}},
	{ Quality::LOW, { Quality::LOW, 0.5f, 0.5f, 1.0f}},
	{ Quality::MEDIUM, { Quality::MEDIUM, 0.5f, 1.0f, 1.0f}},
	{ Quality::HIGH, { Quality::HIGH, 1.0f, 1.0f, 1.0f}},
	{ Quality::HIGH_RES, { Quality::HIGH_RES, 1.0f, 2.0f, 2.0f}}
};

Quality::Quality(const Quality::Level & alevel, const float partRes, const float blurRes, const float finRes){
	for(const auto & kv : names){
		if(kv.second == alevel){
			name = kv.first;
			break;
		}
	}
	particlesResolution = partRes; blurResolution = blurRes; finalResolution = finRes;
}

FilterOptions::FilterOptions(){
	channels.fill( true );
}

void FilterOptions::fillChannelsFromTokens( const std::vector<std::string>& list, bool enabled ){
	for( const std::string& token : list ){
		if( token.empty() ){
			continue;
		}
		int index = Configuration::parseInt( token );
		if( index < 0 || index >= channels.size() ){
			continue;
		}
		channels[ index ] = enabled;
	}
}

void FilterOptions::fillTracksFromTokens( const std::vector<std::string>& list, bool enabled ){
	for( const std::string& token : list ){
		if( token.empty() ){
			continue;
		}
		int index = Configuration::parseInt( token );
		if( index < 0 || index >= MAX_TRACK_COUNT ){
			continue;
		}
		const size_t newMaxTrack = ( std::max )( tracks.size(), size_t( index ) + 1 );
		// All new tracks are assumed visible.
		tracks.resize( newMaxTrack, true );
		tracks[ index ] = enabled;
	}
}

std::string FilterOptions::toHiddenChannelsString(){
	std::vector<unsigned int> hiddenChannels;
	hiddenChannels.reserve( channels.size() );
	for( unsigned int i = 0; i < channels.size(); ++i )
		if( !channels[ i ] )
			hiddenChannels.push_back( i );

	std::stringstream str;
	for( unsigned int chan : hiddenChannels ){
		str << chan << " ";
	}
	return str.str();
}

std::string FilterOptions::toHiddenTracksString(){
	std::vector<unsigned int> hiddenTracks;
	hiddenTracks.reserve( tracks.size() );
	for( unsigned int i = 0; i < tracks.size(); ++i )
		if( !tracks[ i ] )
			hiddenTracks.push_back( i );

	std::stringstream str;
	for( unsigned int track : hiddenTracks ){
		str << track << " ";
	}
	return str.str();
}

bool FilterOptions::accepts( int track, int channel ) const {
	return channels[ channel ] && ( ( track >= tracks.size()) || tracks[ track ] );
}

State::OptionInfos::OptionInfos(){
	description = "";
	type = Type::OTHER;
	range = {0.0f, 0.0f};
	values = "";
	category = Category::GENERAL;
}

State::OptionInfos::OptionInfos(Category acategory, const std::string & adesc, State::OptionInfos::Type atype, const std::array<float, 2> & arange){
	description = adesc;
	type = atype;
	range = arange;
	category = acategory;
	// Generate value description based on type and range.
	switch(type){
		case Type::BOOLEAN:
			values = "1 or 0 to enable/disable";
			break;
		case Type::INTEGER:
			values = "integer";
			if(range[0] != range[1]){
				values.append(" in [" + std::to_string(int(range[0])) + "," + std::to_string(int(range[1])) + "]");
			}
			break;
		case Type::FLOAT:
			values = "number";
			if(range[0] != range[1]){
				values.append(" in [" + std::to_string(range[0]) + "," + std::to_string(range[1]) + "]");
			}
			break;
		case Type::COLOR:
			values = "R G B in [0.0, 1.0]";
			break;
		case Type::KEY:
			values = "key index in [0,127]: 0 is C-1, 1 is C-1#, etc.";
			break;
		case Type::PATH:
			values = "absolute path to file on disk";
			break;
		default:
			break;
	}
}

void State::defineOptions(){
	using Category = OptionInfos::Category;
	using Type = OptionInfos::Type;

	// General
	_sharedInfos["lock-colors"]						= {Category::GENERAL, "Should the keys and all effects use the same color", Type::BOOLEAN};
	_sharedInfos["quality"] 						= {Category::GENERAL, "Rendering quality", Type::OTHER};
	_sharedInfos["quality"].values 					= "values: LOW_RES, LOW, MEDIUM, HIGH, HIGH_RES";
	_sharedInfos["sets-separator-control-points"] 	= {Category::GENERAL, "Sets of control points for dynamic set assignment", Type::OTHER};
	_sharedInfos["sets-separator-control-points"].values = "values: space-separated triplets time,key,set";
	_sharedInfos["filter-hide-channels"] 			= {Category::GENERAL, "Hidden channels", Type::OTHER };
	_sharedInfos["filter-hide-channels"].values 	= "space-separated channels indices, between 0 and 15";
	_sharedInfos["filter-show-channels"] 			= {Category::GENERAL, "Enabled channels", Type::OTHER };
	_sharedInfos["filter-show-channels"].values 	= "space-separated channels indices, between 0 and 15";
	_sharedInfos["filter-hide-tracks"] 				= {Category::GENERAL, "Hidden tracks", Type::OTHER };
	_sharedInfos["filter-hide-tracks"].values 		= "space-separated track indices, starting from 0";
	_sharedInfos["filter-show-tracks"] 				= {Category::GENERAL, "Enabled tracks", Type::OTHER };
	_sharedInfos["filter-show-tracks"].values 		= "space-separated track indices, starting from 0";
	_sharedInfos["colors-per-set"] 					= {Category::GENERAL, "Should each notes set use its own key/effects colors", Type::BOOLEAN};
	_sharedInfos["sets-mode"] 						= {Category::GENERAL, "How should notes be grouped into sets", Type::OTHER};
	_sharedInfos["sets-mode"].values 				= "per-channel: 0, per-track: 1, split based on a key separator: 2, per-key: 3, split based on list: 4, chromatic: 5";
	_sharedInfos["sets-separator-key"] 				= {Category::GENERAL, "If notes are grouped in two sets, defines the key where the split should happen", Type::KEY, {0.0f, 127.0f}};
	_sharedInfos["min-key"] 						= {Category::GENERAL, "Lowest key to display", Type::KEY, {0.0f, 127.0f}};
	_sharedInfos["max-key"] 						= {Category::GENERAL, "Highest key to display", Type::KEY, {0.0f, 127.0f}};
	_sharedInfos["smooth"] 							= {Category::GENERAL, "Apply anti-aliasing to smooth all lines", Type::BOOLEAN};

	// Playback
	_sharedInfos["time-scale"] 			= {Category::PLAYBACK, "Vertical display scale", Type::FLOAT};
	_sharedInfos["preroll"] 			= {Category::PLAYBACK, "Preroll time in seconds before starting to play", Type::FLOAT};
	_sharedInfos["scroll-speed"] 		= {Category::PLAYBACK, "Playback speed", Type::FLOAT};
	_sharedInfos["scroll-reverse"] 		= {Category::PLAYBACK, "Notes scroll from bottom to top instead of the opposite", Type::BOOLEAN};
	_sharedInfos["scroll-horizontal"] 	= {Category::PLAYBACK, "Notes scroll from right to left when enabled", Type::BOOLEAN};

	// Effects
	_sharedInfos["layers"] 			= {Category::EFFECTS, "Active layers indices, from background to foreground", Type::OTHER};
	_sharedInfos["layers"].values 	= "values: bg-color: 0, bg-texture: 1, blur: 2, score: 3, keyboard: 4, particles: 5, notes: 6, flashes: 7, pedal: 8, wave: 9";
	_sharedInfos["show-particles"] 	= {Category::EFFECTS, "Should particles be shown", Type::BOOLEAN};
	_sharedInfos["show-flashes"] 	= {Category::EFFECTS, "Should flashes be shown", Type::BOOLEAN};
	_sharedInfos["show-blur"] 		= {Category::EFFECTS, "Should the blur be visible", Type::BOOLEAN};
	_sharedInfos["show-keyboard"] 	= {Category::EFFECTS, "Should the keyboard be shown", Type::BOOLEAN};
	_sharedInfos["show-score"] 		= {Category::EFFECTS, "Should the score (lines, numbers) be shown", Type::BOOLEAN};
	_sharedInfos["show-notes"] 		= {Category::EFFECTS, "Should the notes be shown", Type::BOOLEAN};
	_sharedInfos["show-pedal"] 		= {Category::EFFECTS, "Display the pedals indicator", Type::BOOLEAN};
	_sharedInfos["show-wave"] 		= {Category::EFFECTS, "Display the wave effect along the top of the keyboard", Type::BOOLEAN};

	// Notes
	_sharedInfos["notes-major-img-scroll"] 		= {Category::NOTES, "Should the major notes texture scroll with the notes", Type::BOOLEAN};
	_sharedInfos["notes-minor-img-scroll"] 		= {Category::NOTES, "Should the minor notes texture scroll with the notes", Type::BOOLEAN};
	_sharedInfos["minor-size"] 					= {Category::NOTES, "Relative size of the minor keys and notes", Type::FLOAT, {0.1f, 1.0f}};
	_sharedInfos["fadeout-notes"] 				= {Category::NOTES, "Notes fade-out at the edge of the screen", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["notes-edge-width"] 			= {Category::NOTES, "Control the width of the edge around notes", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["notes-edge-intensity"] 		= {Category::NOTES, "Control the intensity of the edge around notes", Type::FLOAT, {0.0f, 100.0f}};
	_sharedInfos["notes-corner-radius"]  		= {Category::NOTES, "Rounding radius of the notes corners", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["notes-major-img-scale"]   	= {Category::NOTES, "Scale of the texture applied to major notes", Type::FLOAT};
	_sharedInfos["notes-major-img-intensity"]	= {Category::NOTES, "Intensity of the texture applied to major notes", Type::FLOAT, {0.0, 1.0f}};
	_sharedInfos["notes-minor-img-scale"]   	= {Category::NOTES, "Scale of the texture applied to minor notes", Type::FLOAT};
	_sharedInfos["notes-minor-img-intensity"]	= {Category::NOTES, "Intensity of the texture applied to minor notes", Type::FLOAT, {0.0, 1.0f}};
	_sharedInfos["notes-major-img-path"] 		= {Category::NOTES, "Path to an image on disk to use as texture for the major notes", Type::PATH};
	_sharedInfos["notes-minor-img-path"] 		= {Category::NOTES, "Path to an image on disk to use as texture for the minor notes", Type::PATH};

	// Flashes
	_sharedInfos["flashes-size"] 			= {Category::FLASHES, "Flash effect size", Type::FLOAT, {0.1f, 3.0f}};
	_sharedInfos["flashes-halo-inner"] 		= {Category::FLASHES, "Flash inner halo size", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["flashes-halo-outer"] 		= {Category::FLASHES, "Flash outer halo size", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["flashes-halo-intensity"] 	= {Category::FLASHES, "Flash halo brightness", Type::FLOAT, {0.0f, 2.0f}};
	_sharedInfos["flashes-img-path"] 		= {Category::FLASHES, "Path to an image on disk to use as the flash flipbook", Type::PATH};
	_sharedInfos["flashes-img-rows"] 		= {Category::FLASHES, "Number of rows in the flipbook image", Type::INTEGER};
	_sharedInfos["flashes-img-columns"] 	= {Category::FLASHES, "Number of columns in the flipbook image", Type::INTEGER};

	// Particles
	_sharedInfos["particles-count"] 		= {Category::PARTICLES, "Particles count", Type::INTEGER, {1.0f, 512.0f}};
	_sharedInfos["particles-size"] 			= {Category::PARTICLES, "Size of the particles", Type::FLOAT, {1.0f, 10.0f}};
	_sharedInfos["particles-speed"] 		= {Category::PARTICLES, "Particles speed", Type::FLOAT};
	_sharedInfos["particles-expansion"] 	= {Category::PARTICLES, "Particles expansion factor", Type::FLOAT};
	_sharedInfos["particles-paths"] 		= {Category::PARTICLES, "Set of paths (separated by spaces) to black and white images on disk to use as particles", Type::PATH};

	// Pedal
	_sharedInfos["pedal-size"] 				= {Category::PEDAL, "Pedal indicator size", Type::FLOAT, {0.05f, 0.5f}};
	_sharedInfos["pedal-opacity"] 			= {Category::PEDAL, "Pedal indicator opacity when not pressed", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["color-pedal"] 			= {Category::PEDAL, "All pedals color when pressed", Type::COLOR};
	_sharedInfos["color-pedal"].legacy 		= true;
	_sharedInfos["color-pedal-top"] 		= {Category::PEDAL, "Top pedal color when pressed", Type::COLOR};
	_sharedInfos["color-pedal-center"] 		= {Category::PEDAL, "Center pedal color when pressed", Type::COLOR};
	_sharedInfos["color-pedal-left"] 		= {Category::PEDAL, "Left color when pressed", Type::COLOR};
	_sharedInfos["color-pedal-right"] 		= {Category::PEDAL, "Right pedal color when pressed", Type::COLOR};
	_sharedInfos["pedal-merge"] 			= {Category::PEDAL, "Display only one pedal", Type::BOOLEAN};
	_sharedInfos["pedal-location"] 			= {Category::PEDAL, "Pedal location on screen", Type::OTHER, {0.0f, 3.0f}};
	_sharedInfos["pedal-location"].values 	= "top-left: 0, bottom-left: 1, top-right: 2, bottom-right: 3";
	_sharedInfos["pedal-img-offset-x"] 		= {Category::PEDAL, "Horizontal overlap offset between pedals", Type::FLOAT, {-0.5f, 0.5f}};
	_sharedInfos["pedal-img-offset-y"] 		= {Category::PEDAL, "Vertical overlap offset between pedals", Type::FLOAT  , {-0.5f, 0.5f}};
	_sharedInfos["pedal-img-mirrored"] 		= {Category::PEDAL, "Should the right pedal be mirrored horizontally", Type::BOOLEAN};
	_sharedInfos["pedal-top-img-path"] 		= {Category::PEDAL, "Path to an image on disk to use as texture for the top pedal", Type::PATH};
	_sharedInfos["pedal-center-img-path"] 	= {Category::PEDAL, "Path to an image on disk to use as texture for the center pedal", Type::PATH};
	_sharedInfos["pedal-side-img-paths"] 	= {Category::PEDAL, "Paths to one or two images on disk to use as textures for the side pedals", Type::PATH};

	// Keyboard
	_sharedInfos["keyboard-highlight"] 		= {Category::KEYBOARD, "Should the pressed keys be highlighted", Type::BOOLEAN};
	_sharedInfos["keyboard-custom-colors"] 	= {Category::KEYBOARD, "Override notes color for pressed keys", Type::BOOLEAN};
	_sharedInfos["keyboard-minor-edges"] 	= {Category::KEYBOARD, "Show edges around minor keys", Type::BOOLEAN};
	_sharedInfos["color-keyboard-major"] 	= {Category::KEYBOARD, "Custom color for pressed major keys", Type::COLOR};
	_sharedInfos["color-keyboard-minor"] 	= {Category::KEYBOARD, "Custom color for pressed minor keys", Type::COLOR};
	_sharedInfos["color-keyboard"] 			= {Category::KEYBOARD, "Keyboard black keys color", Type::COLOR};
	_sharedInfos["keyboard-size"] 			= {Category::KEYBOARD, "Vertical size of the keyboard", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["keyboard-minor-height"] 	= {Category::KEYBOARD, "Vertical fraction of the keyboard taken by minor keys", Type::FLOAT, {0.0f, 1.0f}};

	// Score
	_sharedInfos["show-horiz-lines"] 			= {Category::SCORE, "Should horizontal score lines be showed", Type::BOOLEAN};
	_sharedInfos["show-vert-lines"] 			= {Category::SCORE, "Should vertical score lines be shown", Type::BOOLEAN};
	_sharedInfos["show-numbers"] 				= {Category::SCORE, "Should measure numbers be shown", Type::BOOLEAN};
	_sharedInfos["color-lines"] 				= {Category::SCORE, "Score lines color", Type::COLOR};
	_sharedInfos["color-lines"].legacy 			= true;
	_sharedInfos["color-lines-vertical"] 		= {Category::SCORE, "Score vertical lines color", Type::COLOR};
	_sharedInfos["color-lines-horizontal"] 		= {Category::SCORE, "Score horizontal lines color", Type::COLOR};
	_sharedInfos["color-numbers"] 				= {Category::SCORE, "Score measure numbers color", Type::COLOR};
	_sharedInfos["score-lines-vertical-width"] 	= {Category::SCORE, "Score vertical lines height, in pixels", Type::FLOAT};
	_sharedInfos["score-lines-horizontal-width"]= {Category::SCORE, "Score horizontal lines width, in pixels", Type::FLOAT};
	_sharedInfos["score-digits-size"] 			= {Category::SCORE, "Score digits size", Type::FLOAT};
	_sharedInfos["score-digits-offset-x"] 		= {Category::SCORE, "Score digits horizontal offset, as a fraction of a digit", Type::FLOAT};
	_sharedInfos["score-digits-offset-y"] 		= {Category::SCORE, "Score digits vertical offset, as a fraction of a digit", Type::FLOAT};

	// Wave
	_sharedInfos["wave-size"] 				= {Category::WAVE, "Wave effect size", Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos["wave-opacity"] 			= {Category::WAVE, "Wave effect opacity", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["wave-amplitude"] 			= {Category::WAVE, "Wave effect amplitude", Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos["wave-frequency"] 			= {Category::WAVE, "Wave effect frequency", Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos["color-wave"] 				= {Category::WAVE, "Wave effect color", Type::COLOR};
	_sharedInfos["wave-speed"] 				= {Category::WAVE, "Wave scrolling speed", Type::FLOAT };
	_sharedInfos["wave-noise-intensity"] 	= {Category::WAVE, "Wave noise intensity", Type::FLOAT };
	_sharedInfos["wave-noise-extent"] 		= {Category::WAVE, "Wave noise extent on screen", Type::FLOAT };

	// Background
	_sharedInfos["show-blur-notes"] 		= {Category::BACKGROUND, "Should the notes be part of the blur", Type::BOOLEAN};
	_sharedInfos["bg-img-behind-keyboard"]	= {Category::BACKGROUND, "Should the background image extend behind the keyboard", Type::BOOLEAN};
	_sharedInfos["show-bg-img"] 			= {Category::BACKGROUND, "Use a background texture", Type::BOOLEAN};
	_sharedInfos["blur-attenuation"] 		= {Category::BACKGROUND, "Blur attenuation speed", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["bg-img-opacity"] 			= {Category::BACKGROUND, "Background opacity", Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos["color-bg"] 				= {Category::BACKGROUND, "Background color", Type::COLOR};
	_sharedInfos["bg-img-path"] 			= {Category::BACKGROUND, "Path to an image on disk to use as background", Type::PATH};

	// Per sets
	_sharedInfos["color-major"] 			= {Category::PER_SETS,"Major notes color", Type::COLOR};
	_sharedInfos["color-particles"] 		= {Category::PER_SETS, "Particles color", Type::COLOR};
	_sharedInfos["color-minor"] 			= {Category::PER_SETS,"Minor notes color", Type::COLOR};
	_sharedInfos["color-flashes"] 			= {Category::PER_SETS,"Flash effect color", Type::COLOR};

	for(size_t cid = 1; cid < SETS_COUNT; ++cid){
		const std::string num = std::to_string(cid);
		_sharedInfos["color-major-" + num] 		= {Category::PER_SETS, "Major notes color for set " + num, Type::COLOR};
		_sharedInfos["color-particles-" + num] 	= {Category::PER_SETS, "Particles color for set " + num, Type::COLOR};
		_sharedInfos["color-minor-" + num] 		= {Category::PER_SETS, "Minor notes color for set " + num, Type::COLOR};
		_sharedInfos["color-flashes-" + num] 	= {Category::PER_SETS, "Flash effect color for set " + num, Type::COLOR};

	}

}

size_t State::helpText(std::string & configOpts){
	if(_sharedInfos.empty()){
		defineOptions();
	}
	size_t maxLength = 0;
	for(const auto & param : _sharedInfos){
		maxLength = (std::max)(maxLength, param.first.size());
	}

	std::array<std::vector<std::string>, size_t(OptionInfos::Category::COUNT)> linesPerCategory;
	static const std::array<std::string, size_t(OptionInfos::Category::COUNT)> categoryNames = {
		"General", "Playback", "Effect layers", "Notes", "Flashes", "Particles", "Keyboard", "Pedals", "Wave", "Score", "Background and blur", "Per-set settings",
	};

	for(const auto & header : categoryNames){
		maxLength = (std::max)(maxLength, header.size());
	}
	const size_t alignSize = maxLength + 4;
	for(const auto & param : _sharedInfos){
		// Don't list legacy settings.
		if(param.second.legacy){
			continue;
		}
		const std::string padString(alignSize - param.first.size(), ' ');
		const std::string line = "--" + param.first + padString + param.second.description + " (" + param.second.values + ")\n";

		linesPerCategory[size_t(param.second.category)].push_back(line);
	}

	std::stringstream messageStr;
	for(size_t i = 0; i < size_t(OptionInfos::Category::COUNT); ++i){
		auto& list = linesPerCategory[i];
		std::sort(list.begin(), list.end());
		// Insert sub header
		messageStr << "\n# " << categoryNames[i] << "\n";
		for(const auto& str : list){
			messageStr << str;
		}
	}

	configOpts = messageStr.str();
	return alignSize;
}

State::State(){
	reset();
	updateOptions();
}

void State::updateOptions(){
	if(_sharedInfos.empty()){
		defineOptions();
	}

	_intInfos["particles-count"] = &particles.count;
	_boolInfos["show-particles"] = &showParticles;
	_boolInfos["show-flashes"] = &showFlashes;
	_boolInfos["show-blur"] = &showBlur;
	_boolInfos["show-blur-notes"] = &showBlurNotes;
	_boolInfos["lock-colors"] = &lockParticleColor;
	_boolInfos["colors-per-set"] = &perSetColors;
	_boolInfos["show-horiz-lines"] = &score.hLines;
	_boolInfos["show-vert-lines"] = &score.vLines;
	_boolInfos["show-numbers"] = &score.digits;
	_boolInfos["show-keyboard"] = &showKeyboard;
	_boolInfos["bg-img-behind-keyboard"] = &background.imageBehindKeyboard;
	_boolInfos["keyboard-highlight"] = &keyboard.highlightKeys;
	_boolInfos["keyboard-custom-colors"] = &keyboard.customKeyColors;
	_boolInfos["keyboard-minor-edges"] = &keyboard.minorEdges;
	_boolInfos["show-score"] = &showScore;
	_boolInfos["show-bg-img"] = &background.image;
	_boolInfos["show-notes"] = &showNotes;
	_boolInfos["notes-major-img-scroll"] = &notes.majorTexScroll;
	_boolInfos["notes-minor-img-scroll"] = &notes.minorTexScroll;

	_floatInfos["time-scale"] = &scale;
	_floatInfos["minor-size"] = &background.minorsWidth;
	_floatInfos["particles-size"] = &particles.scale;
	_floatInfos["particles-speed"] = &particles.speed;
	_floatInfos["particles-expansion"] = &particles.expansion;
	_floatInfos["blur-attenuation"] = &attenuation;
	_floatInfos["flashes-size"] = &flashes.size;
	_floatInfos["flashes-halo-inner"] = &flashes.haloInnerRadius;
	_floatInfos["flashes-halo-outer"] = &flashes.haloOuterRadius;
	_floatInfos["flashes-halo-intensity"] = &flashes.haloIntensity;
	_floatInfos["preroll"] = &prerollTime;
	_floatInfos["scroll-speed"] = &scrollSpeed;
	_floatInfos["bg-img-opacity"] = &background.imageAlpha;
	_floatInfos["fadeout-notes"] = &notes.fadeOut;
	_floatInfos["notes-edge-width"] = &notes.edgeWidth;
	_floatInfos["notes-edge-intensity"] = &notes.edgeBrightness;
	_floatInfos["notes-corner-radius"] = &notes.cornerRadius;
	_floatInfos["notes-major-img-scale"] = &notes.majorTexScale;
	_floatInfos["notes-major-img-intensity"] = &notes.majorTexAlpha;
	_floatInfos["notes-minor-img-scale"] = &notes.minorTexScale;
	_floatInfos["notes-minor-img-intensity"] = &notes.minorTexAlpha;

	_vecInfos["color-bg"] = &background.color;
	_vecInfos["color-keyboard-major"] = &keyboard.majorColor[0];
	_vecInfos["color-keyboard-minor"] = &keyboard.minorColor[0];
	_vecInfos["color-keyboard"] = &keyboard.edgeColor;
	_vecInfos["color-lines"] = &score.vLinesColor; // Retro compat
	_vecInfos["color-lines-vertical"] = &score.vLinesColor;
	_vecInfos["color-lines-horizontal"] = &score.hLinesColor;
	_vecInfos["color-numbers"] = &score.digitsColor;

	_vecInfos["color-major"] = &notes.majorColors[0];
	_vecInfos["color-particles"] = &particles.colors[0];
	_vecInfos["color-minor"] = &notes.minorColors[0];
	_vecInfos["color-flashes"] = &flashes.colors[0];

	for(size_t cid = 1; cid < notes.majorColors.size(); ++cid){
		const std::string num = std::to_string(cid);
		_vecInfos["color-major-" + num] = &notes.majorColors[cid];
		_vecInfos["color-particles-" + num] = &particles.colors[cid];
		_vecInfos["color-minor-" + num] = &notes.minorColors[cid];
		_vecInfos["color-flashes-" + num] = &flashes.colors[cid];
	}

	_intInfos["sets-mode"] = (int*)&setOptions.mode;
	_intInfos["sets-separator-key"] = &setOptions.key;
	_floatInfos["keyboard-size"] = &keyboard.size;
	_floatInfos["keyboard-minor-height"] = &keyboard.minorHeight;

	_intInfos["min-key"] = &minKey;
	_intInfos["max-key"] = &maxKey;

	_boolInfos["show-pedal"] = &showPedal;
	_floatInfos["pedal-size"] = &pedals.size;
	_floatInfos["pedal-opacity"] = &pedals.opacity;
	_vecInfos["color-pedal"] = &pedals.centerColor; // Retro-compat
	_vecInfos["color-pedal-center"] = &pedals.centerColor;
	_vecInfos["color-pedal-top"] = &pedals.topColor;
	_vecInfos["color-pedal-right"] = &pedals.rightColor;
	_vecInfos["color-pedal-left"] = &pedals.leftColor;
	_boolInfos["pedal-merge"] = &pedals.merge;
	_intInfos["pedal-location"] = (int*)&pedals.location;
	_floatInfos["pedal-img-offset-x"] = &pedals.margin[0];
	_floatInfos["pedal-img-offset-y"] = &pedals.margin[1];
	_boolInfos["pedal-img-mirrored"] = &pedals.mirror;

	_pathInfos["pedal-top-img-path"] = &pedals.topImagePath;
	_pathInfos["pedal-center-img-path"] = &pedals.centerImagePath;
	_pathInfos["pedal-side-img-paths"] = &pedals.sideImagePaths;

	_boolInfos["show-wave"] = &showWave;
	_floatInfos["wave-size"] = &waves.spread;
	_floatInfos["wave-opacity"] = &waves.opacity;
	_floatInfos["wave-amplitude"] =  &waves.amplitude;
	_floatInfos["wave-frequency"] =  &waves.frequency;
	_floatInfos["wave-noise-intensity"] =  &waves.noiseIntensity;
	_floatInfos["wave-noise-extent"] =  &waves.noiseSize;
	_floatInfos["wave-speed"] =  &waves.speed;
	_vecInfos["color-wave"] = &waves.color;

	_boolInfos["smooth"] = &applyAA;
	_boolInfos["scroll-reverse"] = &reverseScroll;
	_boolInfos["scroll-horizontal"] = &horizontalScroll;

	_pathInfos["bg-img-path"] = &background.imagePath;
	_pathInfos["particles-paths"] = &particles.imagePaths;

	_pathInfos["notes-major-path"] = &notes.majorImagePath;
	_pathInfos["notes-minor-path"] = &notes.minorImagePath;
	_pathInfos["flashes-img-path"] = &flashes.imagePath;
	_intInfos["flashes-img-rows"] = &flashes.texRowCount;
	_intInfos["flashes-img-columns"] = &flashes.texColCount;

	_floatInfos["score-lines-vertical-width"] = &score.hLinesWidth;
	_floatInfos["score-lines-horizontal-width"] = &score.vLinesWidth;
	_floatInfos["score-digits-size"] = &score.digitsScale;
	_floatInfos["score-digits-offset-x"] = &score.digitsOffset[0];
	_floatInfos["score-digits-offset-y"] = &score.digitsOffset[1];
}


void State::save(const std::string & path){
	std::string outputPath = path;
	if (path.size() > 4 && path.substr(path.size() - 4, 4) != ".ini") {
		outputPath += ".ini";
	}
	std::ofstream configFile = System::openOutputFile(outputPath);
	if(!configFile.is_open()){
		std::cerr << "[CONFIG]: Unable to save state to file at path " << outputPath << std::endl;
		return;
	}
	// Make sure the parameter pointers are up to date.
	updateOptions();

	configFile << "# MIDIVisualizer configuration file." << std::endl;
	configFile << "# Version (do not modify)" << std::endl;
	configFile << MIDIVIZ_VERSION_MAJOR << " " << MIDIVIZ_VERSION_MINOR << std::endl;

	// Write all options.
	for(const auto & param : _floatInfos){
		const auto& infos = _sharedInfos[param.first];
		if(infos.legacy){
			continue;
		}
		configFile << std::endl << "# " << infos.description << " (";
		configFile << infos.values << ")" << std::endl;
		configFile << param.first << ": " << *param.second << std::endl;
	}

	for(const auto & param : _intInfos){
		const auto& infos = _sharedInfos[param.first];
		if(infos.legacy){
			continue;
		}
		configFile << std::endl << "# " << infos.description << " (";
		configFile << infos.values << ")" << std::endl;
		configFile << param.first << ": " << *param.second << std::endl;
	}

	for(const auto & param : _boolInfos){
		const auto& infos = _sharedInfos[param.first];
		if(infos.legacy){
			continue;
		}
		configFile << std::endl << "# " << infos.description << " (";
		configFile << infos.values << ")" << std::endl;
		configFile << param.first << ": " << (*(param.second) ? 1 : 0) << std::endl;
	}

	for(const auto & param : _vecInfos){
		const auto& infos = _sharedInfos[param.first];
		if(infos.legacy){
			continue;
		}
		configFile << std::endl << "# " << infos.description << " (";
		configFile << infos.values << ")" << std::endl;
		const glm::vec3 & val = *param.second;
		configFile << param.first << ": " << val[0] << " " << val[1] << " " << val[2] << std::endl;
	}

	for(const auto & param : _pathInfos){
		const auto& infos = _sharedInfos[param.first];
		if(infos.legacy){
			continue;
		}

		// Skip empty path collection.
		if(param.second->empty()){
			continue;
		}

		configFile << std::endl << "# " << infos.description << " (";
		configFile << infos.values << ")" << std::endl;
		configFile << param.first << ":";
		for(const std::string& str : *(param.second)){
			configFile << " " << str;
		}
		configFile << std::endl;
	}

	configFile << std::endl << "# " << _sharedInfos["quality"].description << " (";
	configFile << _sharedInfos["quality"].values << ")" << std::endl;
	configFile << "quality: " << Quality::availables.at(quality).name << std::endl;

	configFile << std::endl << "# " << _sharedInfos["layers"].description << " (";
	configFile << _sharedInfos["layers"].values << ")" << std::endl;
	configFile << "layers: ";
	for (int i = 0; i < layersMap.size(); ++i) {
		configFile << layersMap[i] << (i != (layersMap.size() - 1) ? " " : "");
	}
	configFile << std::endl;

	configFile << std::endl << "# " << _sharedInfos["sets-separator-control-points"].description << " (";
	configFile << _sharedInfos["sets-separator-control-points"].values << ")" << std::endl;
	configFile << "sets-separator-control-points: " << setOptions.toKeysString(" ") << std::endl;

	configFile << std::endl << "# " << _sharedInfos[ "filter-hide-channels" ].description << " (";
	configFile << _sharedInfos[ "filter-hide-channels" ].values << ")" << std::endl;
	configFile << "filter-hide-channels: " << filter.toHiddenChannelsString() << std::endl;

	configFile << std::endl << "# " << _sharedInfos[ "filter-hide-tracks" ].description << " (";
	configFile << _sharedInfos[ "filter-hide-tracks" ].values << ")" << std::endl;
	configFile << "filter-hide-tracks: " << filter.toHiddenTracksString() << std::endl;

	// No need to save filter-show-tracks and filter-show-channels, they are complementary of the two above.

	configFile.close();

	_filePath = outputPath;
}

bool State::load(const std::string & path){
	std::ifstream configFileRaw = System::openInputFile(path);
	if(!configFileRaw.is_open()){
		std::cerr << "[CONFIG]: Unable to load state from file at path " << path << std::endl;
		return false;
	}

	// Now that we support comments we need to be able to skip them without large code modifications below.
	// Do a first parse of the file, filtering comments, and build a new stream from it.
	// Not the most efficient thing, but we are talking about a config file of < 100 lines.
	std::stringstream configFile;
	std::string line;
	while(std::getline(configFileRaw, line)){
		line = trim(line, "\r\t ");
		if(line.empty() || line[0] == '#'){
			continue;
		}
		configFile << line << std::endl;
	}
	configFileRaw.close();

	// Check the version number.
	int majVersion = 0; int minVersion = 0;
	configFile >> majVersion >> minVersion;
	
	if(majVersion > MIDIVIZ_VERSION_MAJOR || (majVersion == MIDIVIZ_VERSION_MAJOR && minVersion > MIDIVIZ_VERSION_MINOR)){
		std::cout << "[CONFIG]: The config is more recent, some settings might be ignored." << std::endl;
	}
	if(majVersion < MIDIVIZ_VERSION_MAJOR || (majVersion == MIDIVIZ_VERSION_MAJOR && minVersion < MIDIVIZ_VERSION_MINOR)){
		std::cout << "[CONFIG]: The config is older, some newer settings will be left as-is." << std::endl;
	}

	// Two options: if we are < 5.0, we use the old positional format.
	// Else we use the key-value format, similar to command line.
	if(majVersion < 5){
		loadLegacy(configFile, majVersion, minVersion);
		// No set options.
	} else {
		// Build arguments list.
		const Arguments args = Configuration::parseArguments(configFile);
		load(args);
	}

	_filePath = path;
	return true;
}

void State::load(const Arguments & configArgs){
	// Make sure the parameter pointers are up to date.
	updateOptions();

	for(const auto & arg : configArgs){
		const auto & key = arg.first;
		if(arg.second.empty()){
			std::cerr << "[CONFIG]: Missing values for key " << key << "." << std::endl;
			continue;
		}

		if(key == "quality"){
			const std::string & qualityName = arg.second[0];
			if(Quality::names.count(qualityName) > 0){
				quality = Quality::names.at(qualityName);
			}
			continue;
		}

		if(key == "layers"){
			const size_t bound = (std::min)(layersMap.size(), arg.second.size());
			for(size_t id = 0; id < bound; ++id){
				layersMap[id] = Configuration::parseInt(arg.second[id]);
			}
			continue;
		}

		if(key == "sets-separator-control-points"){
			std::string str = arg.second[0];
			for(size_t tid = 1; tid < arg.second.size(); ++tid){
				str.append(" ");
				str.append(arg.second[tid]);
			}
			setOptions.fromKeysString(str);
			continue;
		}

		if( key == "filter-hide-channels" ){
			filter.fillChannelsFromTokens( arg.second, false);
			continue;
		}
		if( key == "filter-show-channels" ){
			filter.fillChannelsFromTokens( arg.second, true );
			continue;
		}
		if( key == "filter-hide-tracks" ){
			filter.fillTracksFromTokens( arg.second, false );
			continue;
		}
		if( key == "filter-show-tracks" ){
			filter.fillTracksFromTokens( arg.second, true );
			continue;
		}

		const auto itf = _floatInfos.find(key);
		if(itf != _floatInfos.end()){
			const auto & opt = itf->second;
			*opt = Configuration::parseFloat(arg.second[0]);
			const auto & range = _sharedInfos.at(key).range;
			if(range[0] != range[1]){
				*opt = glm::clamp(*opt, range[0], range[1]);
			}
			continue;
		}

		const auto iti = _intInfos.find(key);
		if(iti != _intInfos.end()){
			const auto & opt = iti->second;
			*opt = Configuration::parseInt(arg.second[0]);
			const auto & range = _sharedInfos.at(key).range;
			if(range[0] != range[1]){
				*opt = glm::clamp(*opt, int(range[0]), int(range[1]));
			}
			continue;
		}

		const auto itb = _boolInfos.find(key);
		if(itb != _boolInfos.end()){
			const auto & opt = itb->second;
			*opt = Configuration::parseBool(arg.second[0]);
			continue;
		}

		const auto itv = _vecInfos.find(key);
		if(itv != _vecInfos.end()){
			const auto & opt = itv->second;
			*opt = Configuration::parseVec3(arg.second);
			continue;
		}

		const auto its = _pathInfos.find(key);
		if(its != _pathInfos.end()){
			const auto & opt = its->second;
			*opt = arg.second;
			continue;
		}
	}

	// Ensure synchronization of the keyboard override colors.
	for(size_t cid = 1; cid < keyboard.majorColor.size(); ++cid){
		keyboard.majorColor[cid] = keyboard.majorColor[0];
		keyboard.minorColor[cid] = keyboard.minorColor[0];
	}
	
	// Synchronization for pedals global/legacy setting.
	if(configArgs.count("color-pedal") != 0){
		pedals.topColor = pedals.centerColor;
		pedals.leftColor = pedals.centerColor;
		pedals.rightColor = pedals.centerColor;
	}
	// Legacy setting for score lines.
	if(configArgs.count("color-lines") != 0){
		score.hLinesColor = score.vLinesColor;
	}

	// Don't erase the file path.
	
	// Rebuild internal data.
	setOptions.rebuild();
}

void State::synchronizeSets(){
	for(size_t cid = 1; cid < SETS_COUNT; ++cid){
		notes.majorColors[cid] = notes.majorColors[0];
		notes.minorColors[cid] = notes.minorColors[0];
		flashes.colors[cid] = flashes.colors[0];
		particles.colors[cid] = particles.colors[0];
		keyboard.majorColor[cid] = keyboard.majorColor[0];
		keyboard.minorColor[cid] = keyboard.minorColor[0];
	}
}

const std::string& State::filePath() const {
	return _filePath;
}

void State::reset(){

	for(size_t cid = 0; cid < SETS_COUNT; ++cid){
		notes.majorColors[cid] = 1.35f * glm::vec3(0.57f,0.19f,0.98f);
		notes.minorColors[cid] = 0.8f * notes.majorColors[0];
		flashes.colors[cid] = notes.majorColors[0];
		particles.colors[cid] = notes.majorColors[0];
		keyboard.majorColor[cid] = notes.majorColors[0];
		keyboard.minorColor[cid] = notes.minorColors[0];
	}

	background.color = glm::vec3(0.0f, 0.0f, 0.0f);
	score.hLinesColor = glm::vec3(0.25f);
	score.vLinesColor = glm::vec3(0.75f);
	score.digitsColor = glm::vec3(1.0f, 1.0f, 1.0f);
	keyboard.edgeColor = glm::vec3(0.0f, 0.0f, 0.0f);

	scale = 0.5f ;
	attenuation = 0.99f;
	showParticles = true;
	showFlashes = true;
	showBlur = true;
	showBlurNotes = false;
	lockParticleColor = false; // Changed in 7.0
	perSetColors = false;
	showNotes = true;
	showScore = true;
	showKeyboard = true;
	flashes.size = 1.0f;
	flashes.haloIntensity = 1.0f;
	flashes.haloInnerRadius = 0.14f;
	flashes.haloOuterRadius = 1.0f;
	flashes.texRowCount = 4;
	flashes.texColCount = 2;
	flashes.imagePath.clear();
	flashes.tex = ResourcesManager::getTextureFor("flash");
	
	background.minorsWidth = 0.8f;
	background.image = false;
	background.imageAlpha = 1.0f;
	background.tex = 0;
	background.imageBehindKeyboard = false;
	background.imagePath.clear();

	score.hLines = true;
	score.vLines = true;
	score.digits = true;
	score.hLinesWidth = 2.0f;
	score.vLinesWidth = 2.0f;
	score.digitsScale = 0.08f;
	score.digitsOffset = {0, 0};

	particles.speed = 0.2f;
	particles.expansion = 1.0f;
	particles.scale = 1.0f;
	particles.count = 256;
	particles.imagePaths.clear();
	const GLuint blankID = ResourcesManager::getTextureFor("blankarray");
	particles.tex = blankID;
	particles.texCount = 1;
	
	quality = Quality::MEDIUM;
	prerollTime = 1.0f;
	scrollSpeed = 1.0f;
	keyboard.highlightKeys = true;
	keyboard.minorEdges = true;
	keyboard.customKeyColors = false;
	keyboard.size = 0.25f;
	keyboard.minorHeight = 0.6f;

	for (int i = 0; i < layersMap.size(); ++i) {
		layersMap[i] = i;
	}

	setOptions = SetOptions();

	filter = FilterOptions();

	notes.edgeBrightness = 1.05f;
	notes.edgeWidth = 0.1f;
	notes.fadeOut = 0.0f;
	notes.cornerRadius = 0.09f;
	notes.majorTexAlpha = 1.0;
	notes.majorTexScale = 5.0f;
	notes.minorTexAlpha = 1.0;
	notes.minorTexScale = 5.0f;
	notes.majorTexScroll = true;
	notes.minorTexScroll = true;
	notes.majorImagePath.clear();
	notes.minorImagePath.clear();
	notes.minorTex = 0;
	notes.majorTex = 0;
	
	minKey = 21;
	maxKey = 108;
	
	showPedal = true;
	pedals.topColor = notes.majorColors[0];
	pedals.centerColor = pedals.topColor;
	pedals.leftColor = pedals.topColor;
	pedals.rightColor = pedals.topColor;
	pedals.size = 0.2f;
	pedals.opacity = 0.4f;
	pedals.merge = false;
	pedals.margin = {0.04f, 0.0f};
	pedals.mirror = true;
	pedals.location = PedalsState::BOTTOMRIGHT;
	pedals.topImagePath.clear();
	pedals.centerImagePath.clear();
	pedals.sideImagePaths.clear();

	pedals.texCenter = ResourcesManager::getTextureFor("pedal_center");
	pedals.texTop = ResourcesManager::getTextureFor("pedal_top");
	pedals.texSides[0] = ResourcesManager::getTextureFor("pedal_side");
	pedals.texSides[1] = ResourcesManager::getTextureFor("pedal_side");

	showWave = true;
	waves.color = notes.majorColors[0];
	waves.amplitude = 1.0f;
	waves.frequency = 1.0f;
	waves.opacity = 1.0f;
	waves.spread = 1.0f;
	waves.speed = 1.0f;
	waves.noiseIntensity = 1.0f;
	waves.noiseSize = 0.03f;

	applyAA = false;
	reverseScroll = false;
	horizontalScroll = false;

	_filePath = "";
}

void State::loadLegacy(std::istream & configFile, int majVersion, int minVersion){

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 0
	// This part is always valid, as it was present when the saving system was introduced.
	// Note: we don't restore the texture IDs and scale.
	{
		configFile >> notes.majorColors[0][0] >> notes.majorColors[0][1] >> notes.majorColors[0][2] ;
		configFile >> background.color[0] >> background.color[1] >> background.color[2] ;
		configFile >> particles.colors[0][0] >> particles.colors[0][1] >> particles.colors[0][2] ;

		configFile >> scale ;
		configFile >> showParticles ;
		configFile >> showFlashes ;
		configFile >> showBlur ;
		configFile >> showBlurNotes ;
		configFile >> lockParticleColor ;

		configFile >> background.minorsWidth ;
		configFile >> score.hLines ;
		configFile >> score.vLines ;
		configFile >> score.digits ;
		configFile >> showKeyboard ;

		configFile >> particles.speed ;
		configFile >> particles.expansion ;
		configFile >> particles.count ;
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 1
	// Added quality level.
	if(majVersion > 3 || (majVersion == 3 && minVersion >= 1)){
		std::string qualityName;;
		configFile >> qualityName;
		if(Quality::names.count(qualityName) > 0){
			quality = Quality::names.at(qualityName);
		}
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 2
	// Added attenuation factor.
	if(majVersion > 3 || (majVersion == 3 && minVersion >= 2)){
		configFile >> attenuation ;
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 3
	/* minorColor = 0.8f*baseColor;
	flashColor = baseColor; */
	if(majVersion > 3 || (majVersion == 3 && minVersion >= 3)){
		configFile >> showNotes;
		configFile >> score.vLinesColor[0] >> score.vLinesColor[1] >> score.vLinesColor[2] ;
		configFile >> score.digitsColor[0] >> score.digitsColor[1] >> score.digitsColor[2] ;
		configFile >> keyboard.edgeColor[0] >> keyboard.edgeColor[1] >> keyboard.edgeColor[2] ;
		configFile >> notes.minorColors[0][0] >> notes.minorColors[0][1] >> notes.minorColors[0][2] ;
		configFile >> flashes.colors[0][0] >> flashes.colors[0][1] >> flashes.colors[0][2] ;
		configFile >> flashes.size;
	}

	// MIDIVIZ_VERSION_MAJOR == 3, MIDIVIZ_VERSION_MINOR == 5
	if (majVersion > 3 || (majVersion == 3 && minVersion >= 5)) {
		configFile >> prerollTime;
		configFile >> showScore;
		std::string layersList;
		// Get the next line.
		// Skip newline.
		std::getline(configFile, layersList);
		std::getline(configFile, layersList);
		std::stringstream sstr(layersList);
		int id = 0;
		while (!sstr.eof() && id < layersMap.size()) {
			sstr >> layersMap[id];
			++id;
		}
	}

	// MIDIVIZ_VERSION_MAJOR == 4, MIDIVIZ_VERSION_MINOR == 0
	if (majVersion > 4 || (majVersion == 4 && minVersion >= 0)) {
		configFile >> background.image;
		configFile >> background.imageAlpha;
		configFile >> background.imageBehindKeyboard;
		configFile >> keyboard.highlightKeys;
	}

	// MIDIVIZ_VERSION_MAJOR == 4, MIDIVIZ_VERSION_MINOR == 1
	if (majVersion > 4 || (majVersion == 4 && minVersion >= 1)) {
		configFile >> keyboard.customKeyColors;
		configFile >> keyboard.majorColor[0][0] >> keyboard.majorColor[0][1] >> keyboard.majorColor[0][2];
		configFile >> keyboard.minorColor[0][0] >> keyboard.minorColor[0][1] >> keyboard.minorColor[0][2];
	}

	// Ensure synchronization of all channels colors.
	synchronizeSets();

	pedals.topColor = notes.majorColors[0];
	pedals.leftColor = pedals.topColor;
	pedals.rightColor = pedals.topColor;
	pedals.centerColor = pedals.topColor;
	waves.color = notes.majorColors[0];
	score.hLinesColor = score.vLinesColor;
}
