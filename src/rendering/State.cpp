
#include "State.h"
#include "scene/MIDIScene.h"
#include "../helpers/ResourcesManager.h"
#include "../helpers/System.h"
#include "../resources/strings.h"

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
	_sharedInfos[s_lock_colors_key]						= {Category::GENERAL, s_lock_colors_dsc, Type::BOOLEAN};
	_sharedInfos[s_quality_key] 						= {Category::GENERAL, s_quality_dsc, Type::OTHER};
	_sharedInfos[s_quality_key].values 					= "values: LOW_RES, LOW, MEDIUM, HIGH, HIGH_RES";
	_sharedInfos[s_sets_separator_control_points_key] 	= {Category::GENERAL, s_sets_separator_control_points_dsc, Type::OTHER};
	_sharedInfos[s_sets_separator_control_points_key].values = "values: space-separated triplets time,key,set";
	_sharedInfos[s_filter_hide_channels_key] 			= {Category::GENERAL, s_filter_hide_channels_dsc, Type::OTHER };
	_sharedInfos[s_filter_hide_channels_key].values 	= "space-separated channels indices, between 0 and 15";
	_sharedInfos[s_filter_show_channels_key] 			= {Category::GENERAL, s_filter_show_channels_dsc, Type::OTHER };
	_sharedInfos[s_filter_show_channels_key].values 	= "space-separated channels indices, between 0 and 15";
	_sharedInfos[s_filter_hide_tracks_key] 				= {Category::GENERAL, s_filter_hide_tracks_dsc, Type::OTHER };
	_sharedInfos[s_filter_hide_tracks_key].values 		= "space-separated track indices, starting from 0";
	_sharedInfos[s_filter_show_tracks_key] 				= {Category::GENERAL, s_filter_show_tracks_dsc, Type::OTHER };
	_sharedInfos[s_filter_show_tracks_key].values 		= "space-separated track indices, starting from 0";
	_sharedInfos[s_colors_per_set_key] 					= {Category::GENERAL, s_colors_per_set_dsc, Type::BOOLEAN};
	_sharedInfos[s_sets_mode_key] 						= {Category::GENERAL, s_sets_mode_dsc, Type::OTHER};
	_sharedInfos[s_sets_mode_key].values 				= "per-channel: 0, per-track: 1, split based on a key separator: 2, per-key: 3, split based on list: 4, chromatic keys: 5";
	_sharedInfos[s_sets_separator_key_key] 				= {Category::GENERAL, s_sets_separator_key_dsc, Type::KEY, {0.0f, 127.0f}};
	_sharedInfos[s_min_key_key] 						= {Category::GENERAL, s_min_key_dsc, Type::KEY, {0.0f, 127.0f}};
	_sharedInfos[s_max_key_key] 						= {Category::GENERAL, s_max_key_dsc, Type::KEY, {0.0f, 127.0f}};
	_sharedInfos[s_smooth_key] 							= {Category::GENERAL, s_smooth_dsc, Type::BOOLEAN};

	// Playback
	_sharedInfos[s_time_scale_key] 			= {Category::PLAYBACK, s_time_scale_dsc, Type::FLOAT};
	_sharedInfos[s_preroll_key] 			= {Category::PLAYBACK, s_preroll_dsc, Type::FLOAT};
	_sharedInfos[s_scroll_speed_key] 		= {Category::PLAYBACK, s_scroll_speed_dsc, Type::FLOAT};
	_sharedInfos[s_scroll_reverse_key] 		= {Category::PLAYBACK, s_scroll_reverse_dsc, Type::BOOLEAN};
	_sharedInfos[s_scroll_horizontal_key] 	= {Category::PLAYBACK, s_scroll_horizontal_dsc, Type::BOOLEAN};

	// Effects
	_sharedInfos[s_layers_key] 			= {Category::EFFECTS, s_layers_dsc, Type::OTHER};
	_sharedInfos[s_layers_key].values 	= "values: bg-color: 0, bg-texture: 1, blur: 2, score: 3, keyboard: 4, particles: 5, notes: 6, flashes: 7, pedal: 8, wave: 9";
	_sharedInfos[s_show_particles_key] 	= {Category::EFFECTS, s_show_particles_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_flashes_key] 	= {Category::EFFECTS, s_show_flashes_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_blur_key] 		= {Category::EFFECTS, s_show_blur_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_keyboard_key] 	= {Category::EFFECTS, s_show_keyboard_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_score_key] 		= {Category::EFFECTS, s_show_score_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_notes_key] 		= {Category::EFFECTS, s_show_notes_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_pedal_key] 		= {Category::EFFECTS, s_show_pedal_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_wave_key] 		= {Category::EFFECTS, s_show_wave_dsc, Type::BOOLEAN};

	// Notes
	_sharedInfos[s_notes_major_img_scroll_key] 		= {Category::NOTES, s_notes_major_img_scroll_dsc, Type::BOOLEAN};
	_sharedInfos[s_notes_minor_img_scroll_key] 		= {Category::NOTES, s_notes_minor_img_scroll_dsc, Type::BOOLEAN};
	_sharedInfos[s_minor_size_key] 					= {Category::NOTES, s_minor_size_dsc, Type::FLOAT, {0.1f, 1.0f}};
	_sharedInfos[s_fadeout_notes_key] 				= {Category::NOTES, s_fadeout_notes_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_notes_edge_width_key] 			= {Category::NOTES, s_notes_edge_width_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_notes_edge_intensity_key] 		= {Category::NOTES, s_notes_edge_intensity_dsc, Type::FLOAT, {0.0f, 100.0f}};
	_sharedInfos[s_notes_corner_radius_key]  		= {Category::NOTES, s_notes_corner_radius_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_notes_major_img_scale_key]   	= {Category::NOTES, s_notes_major_img_scale_dsc, Type::FLOAT};
	_sharedInfos[s_notes_major_img_intensity_key]	= {Category::NOTES, s_notes_major_img_intensity_dsc, Type::FLOAT, {0.0, 1.0f}};
	_sharedInfos[s_notes_minor_img_scale_key]   	= {Category::NOTES, s_notes_minor_img_scale_dsc, Type::FLOAT};
	_sharedInfos[s_notes_minor_img_intensity_key]	= {Category::NOTES, s_notes_minor_img_intensity_dsc, Type::FLOAT, {0.0, 1.0f}};
	_sharedInfos[s_notes_major_img_path_key] 		= {Category::NOTES, s_notes_major_img_path_dsc, Type::PATH};
	_sharedInfos[s_notes_minor_img_path_key] 		= {Category::NOTES, s_notes_minor_img_path_dsc, Type::PATH};

	// Flashes
	_sharedInfos[s_flashes_size_key] 			= {Category::FLASHES, s_flashes_size_dsc, Type::FLOAT, {0.1f, 3.0f}};
	_sharedInfos[s_flashes_halo_inner_key] 		= {Category::FLASHES, s_flashes_halo_inner_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_flashes_halo_outer_key] 		= {Category::FLASHES, s_flashes_halo_outer_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_flashes_halo_intensity_key] 	= {Category::FLASHES, s_flashes_halo_intensity_dsc, Type::FLOAT, {0.0f, 2.0f}};
	_sharedInfos[s_flashes_img_path_key] 		= {Category::FLASHES, s_flashes_img_path_dsc, Type::PATH};
	_sharedInfos[s_flashes_img_rows_key] 		= {Category::FLASHES, s_flashes_img_rows_dsc, Type::INTEGER};
	_sharedInfos[s_flashes_img_columns_key] 	= {Category::FLASHES, s_flashes_img_columns_dsc, Type::INTEGER};

	// Particles
	_sharedInfos[s_particles_count_key] 		= {Category::PARTICLES, s_particles_count_dsc, Type::INTEGER, {1.0f, 512.0f}};
	_sharedInfos[s_particles_size_key] 			= {Category::PARTICLES, s_particles_size_dsc, Type::FLOAT, {1.0f, 10.0f}};
	_sharedInfos[s_particles_speed_key] 		= {Category::PARTICLES, s_particles_speed_dsc, Type::FLOAT};
	_sharedInfos[s_particles_expansion_key] 	= {Category::PARTICLES, s_particles_expansion_dsc, Type::FLOAT};
	_sharedInfos[s_particles_paths_key] 		= {Category::PARTICLES, s_particles_paths_dsc, Type::PATH};
	_sharedInfos[s_particles_turbulences_key] 	= {Category::PARTICLES, s_particles_turbulences_dsc, Type::FLOAT, {0.f, 10.f}};

	// Pedal
	_sharedInfos[s_pedal_size_key] 				= {Category::PEDAL, s_pedal_size_dsc, Type::FLOAT, {0.05f, 0.5f}};
	_sharedInfos[s_pedal_opacity_key] 			= {Category::PEDAL, s_pedal_opacity_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_color_pedal_key] 			= {Category::PEDAL, s_color_pedal_dsc, Type::COLOR};
	_sharedInfos[s_color_pedal_key].legacy 		= true;
	_sharedInfos[s_color_pedal_top_key] 		= {Category::PEDAL, s_color_pedal_top_dsc, Type::COLOR};
	_sharedInfos[s_color_pedal_center_key] 		= {Category::PEDAL, s_color_pedal_center_dsc, Type::COLOR};
	_sharedInfos[s_color_pedal_left_key] 		= {Category::PEDAL, s_color_pedal_left_dsc, Type::COLOR};
	_sharedInfos[s_color_pedal_right_key] 		= {Category::PEDAL, s_color_pedal_right_dsc, Type::COLOR};
	_sharedInfos[s_pedal_merge_key] 			= {Category::PEDAL, s_pedal_merge_dsc, Type::BOOLEAN};
	_sharedInfos[s_pedal_location_key] 			= {Category::PEDAL, s_pedal_location_dsc, Type::OTHER, {0.0f, 3.0f}};
	_sharedInfos[s_pedal_location_key].values 	= "top-left: 0, bottom-left: 1, top-right: 2, bottom-right: 3";
	_sharedInfos[s_pedal_img_offset_x_key] 		= {Category::PEDAL, s_pedal_img_offset_x_dsc, Type::FLOAT, {-0.5f, 0.5f}};
	_sharedInfos[s_pedal_img_offset_y_key] 		= {Category::PEDAL, s_pedal_img_offset_y_dsc, Type::FLOAT  , {-0.5f, 0.5f}};
	_sharedInfos[s_pedal_img_mirrored_key] 		= {Category::PEDAL, s_pedal_img_mirrored_dsc, Type::BOOLEAN};
	_sharedInfos[s_pedal_top_img_path_key] 		= {Category::PEDAL, s_pedal_top_img_path_dsc, Type::PATH};
	_sharedInfos[s_pedal_center_img_path_key] 	= {Category::PEDAL, s_pedal_center_img_path_dsc, Type::PATH};
	_sharedInfos[s_pedal_side_img_paths_key] 	= {Category::PEDAL, s_pedal_side_img_paths_dsc, Type::PATH};

	// Keyboard
	_sharedInfos[s_keyboard_highlight_key] 		= {Category::KEYBOARD, s_keyboard_highlight_dsc, Type::BOOLEAN};
	_sharedInfos[s_keyboard_custom_colors_key] 	= {Category::KEYBOARD, s_keyboard_custom_colors_dsc, Type::BOOLEAN};
	_sharedInfos[s_keyboard_minor_edges_key] 	= {Category::KEYBOARD, s_keyboard_minor_edges_dsc, Type::BOOLEAN};
	_sharedInfos[s_color_keyboard_major_key] 	= {Category::KEYBOARD, s_color_keyboard_major_dsc, Type::COLOR};
	_sharedInfos[s_color_keyboard_minor_key] 	= {Category::KEYBOARD, s_color_keyboard_minor_dsc, Type::COLOR};
	_sharedInfos[s_color_keyboard_key] 			= {Category::KEYBOARD, s_color_keyboard_dsc, Type::COLOR};
	_sharedInfos[s_keyboard_size_key] 			= {Category::KEYBOARD, s_keyboard_size_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_keyboard_minor_height_key] 	= {Category::KEYBOARD, s_keyboard_minor_height_dsc, Type::FLOAT, {0.0f, 1.0f}};

	// Score
	_sharedInfos[s_show_horiz_lines_key] 			= {Category::SCORE, s_show_horiz_lines_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_vert_lines_key] 			= {Category::SCORE, s_show_vert_lines_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_numbers_key] 				= {Category::SCORE, s_show_numbers_dsc, Type::BOOLEAN};
	_sharedInfos[s_color_lines_key] 				= {Category::SCORE, s_color_lines_dsc, Type::COLOR};
	_sharedInfos[s_color_lines_key].legacy 			= true;
	_sharedInfos[s_color_lines_vertical_key] 		= {Category::SCORE, s_color_lines_vertical_dsc, Type::COLOR};
	_sharedInfos[s_color_lines_horizontal_key] 		= {Category::SCORE, s_color_lines_horizontal_dsc, Type::COLOR};
	_sharedInfos[s_color_numbers_key] 				= {Category::SCORE, s_color_numbers_dsc, Type::COLOR};
	_sharedInfos[s_score_lines_vertical_width_key] 	= {Category::SCORE, s_score_lines_vertical_width_dsc, Type::FLOAT};
	_sharedInfos[s_score_lines_horizontal_width_key]= {Category::SCORE, s_score_lines_horizontal_width_dsc, Type::FLOAT};
	_sharedInfos[s_score_digits_size_key] 			= {Category::SCORE, s_score_digits_size_dsc, Type::FLOAT};
	_sharedInfos[s_score_digits_offset_x_key] 		= {Category::SCORE, s_score_digits_offset_x_dsc, Type::FLOAT};
	_sharedInfos[s_score_digits_offset_y_key] 		= {Category::SCORE, s_score_digits_offset_y_dsc, Type::FLOAT};

	// Wave
	_sharedInfos[s_wave_size_key] 				= {Category::WAVE, s_wave_size_dsc, Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos[s_wave_opacity_key] 			= {Category::WAVE, s_wave_opacity_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_wave_amplitude_key] 			= {Category::WAVE, s_wave_amplitude_dsc, Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos[s_wave_frequency_key] 			= {Category::WAVE, s_wave_frequency_dsc, Type::FLOAT, {0.0f, 5.0f}};
	_sharedInfos[s_color_wave_key] 				= {Category::WAVE, s_color_wave_dsc, Type::COLOR};
	_sharedInfos[s_wave_speed_key] 				= {Category::WAVE, s_wave_speed_dsc, Type::FLOAT };
	_sharedInfos[s_wave_noise_intensity_key] 	= {Category::WAVE, s_wave_noise_intensity_dsc, Type::FLOAT };
	_sharedInfos[s_wave_noise_extent_key] 		= {Category::WAVE, s_wave_noise_extent_dsc, Type::FLOAT };

	// Background
	_sharedInfos[s_show_blur_notes_key] 		= {Category::BACKGROUND, s_show_blur_notes_dsc, Type::BOOLEAN};
	_sharedInfos[s_bg_img_behind_keyboard_key]	= {Category::BACKGROUND, s_bg_img_behind_keyboard_dsc, Type::BOOLEAN};
	_sharedInfos[s_show_bg_img_key] 			= {Category::BACKGROUND, s_show_bg_img_dsc, Type::BOOLEAN};
	_sharedInfos[s_blur_attenuation_key] 		= {Category::BACKGROUND, s_blur_attenuation_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_bg_img_opacity_key] 			= {Category::BACKGROUND, s_bg_img_opacity_dsc, Type::FLOAT, {0.0f, 1.0f}};
	_sharedInfos[s_color_bg_key] 				= {Category::BACKGROUND, s_color_bg_dsc, Type::COLOR};
	_sharedInfos[s_bg_img_path_key] 			= {Category::BACKGROUND, s_bg_img_path_dsc, Type::PATH};

	// Per sets
	_sharedInfos[s_color_major_key] 			= {Category::PER_SETS, s_color_major_dsc, Type::COLOR};
	_sharedInfos[s_color_particles_key] 		= {Category::PER_SETS, s_color_particles_dsc, Type::COLOR};
	_sharedInfos[s_color_minor_key] 			= {Category::PER_SETS, s_color_minor_dsc, Type::COLOR};
	_sharedInfos[s_color_flashes_key] 			= {Category::PER_SETS, s_color_flashes_dsc, Type::COLOR};

	assert(sizeof(s_color_major_dscs) / sizeof(s_color_major_dscs[0]) == SETS_COUNT);
	assert(sizeof(s_color_minor_dscs) / sizeof(s_color_minor_dscs[0]) == SETS_COUNT);
	assert(sizeof(s_color_particles_dscs) / sizeof(s_color_particles_dscs[0]) == SETS_COUNT);
	assert(sizeof(s_color_flashes_dscs) / sizeof(s_color_flashes_dscs[0]) == SETS_COUNT);

	for(size_t cid = 1; cid < SETS_COUNT; ++cid){
		_sharedInfos[s_color_major_keys[cid]] 		= {Category::PER_SETS, s_color_major_dscs[cid], Type::COLOR};
		_sharedInfos[s_color_minor_keys[cid]] 		= {Category::PER_SETS, s_color_minor_dscs[cid], Type::COLOR};
		_sharedInfos[s_color_particles_keys[cid]] 	= {Category::PER_SETS, s_color_particles_dscs[cid], Type::COLOR};
		_sharedInfos[s_color_flashes_keys[cid]] 	= {Category::PER_SETS, s_color_flashes_dscs[cid], Type::COLOR};
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

	_intInfos[s_particles_count_key] = &particles.count;
	_boolInfos[s_show_particles_key] = &showParticles;
	_boolInfos[s_show_flashes_key] = &showFlashes;
	_boolInfos[s_show_blur_key] = &showBlur;
	_boolInfos[s_show_blur_notes_key] = &showBlurNotes;
	_boolInfos[s_lock_colors_key] = &lockParticleColor;
	_boolInfos[s_colors_per_set_key] = &perSetColors;
	_boolInfos[s_show_horiz_lines_key] = &score.hLines;
	_boolInfos[s_show_vert_lines_key] = &score.vLines;
	_boolInfos[s_show_numbers_key] = &score.digits;
	_boolInfos[s_show_keyboard_key] = &showKeyboard;
	_boolInfos[s_bg_img_behind_keyboard_key] = &background.imageBehindKeyboard;
	_boolInfos[s_keyboard_highlight_key] = &keyboard.highlightKeys;
	_boolInfos[s_keyboard_custom_colors_key] = &keyboard.customKeyColors;
	_boolInfos[s_keyboard_minor_edges_key] = &keyboard.minorEdges;
	_boolInfos[s_show_score_key] = &showScore;
	_boolInfos[s_show_bg_img_key] = &background.image;
	_boolInfos[s_show_notes_key] = &showNotes;
	_boolInfos[s_notes_major_img_scroll_key] = &notes.majorTexScroll;
	_boolInfos[s_notes_minor_img_scroll_key] = &notes.minorTexScroll;

	_floatInfos[s_time_scale_key] = &scale;
	_floatInfos[s_minor_size_key] = &background.minorsWidth;
	_floatInfos[s_particles_size_key] = &particles.scale;
	_floatInfos[s_particles_speed_key] = &particles.speed;
	_floatInfos[s_particles_expansion_key] = &particles.expansion;
	_floatInfos[s_particles_turbulences_key] = &particles.turbulenceStrength;
	_floatInfos[s_blur_attenuation_key] = &attenuation;
	_floatInfos[s_flashes_size_key] = &flashes.size;
	_floatInfos[s_flashes_halo_inner_key] = &flashes.haloInnerRadius;
	_floatInfos[s_flashes_halo_outer_key] = &flashes.haloOuterRadius;
	_floatInfos[s_flashes_halo_intensity_key] = &flashes.haloIntensity;
	_floatInfos[s_preroll_key] = &prerollTime;
	_floatInfos[s_scroll_speed_key] = &scrollSpeed;
	_floatInfos[s_bg_img_opacity_key] = &background.imageAlpha;
	_floatInfos[s_fadeout_notes_key] = &notes.fadeOut;
	_floatInfos[s_notes_edge_width_key] = &notes.edgeWidth;
	_floatInfos[s_notes_edge_intensity_key] = &notes.edgeBrightness;
	_floatInfos[s_notes_corner_radius_key] = &notes.cornerRadius;
	_floatInfos[s_notes_major_img_scale_key] = &notes.majorTexScale;
	_floatInfos[s_notes_major_img_intensity_key] = &notes.majorTexAlpha;
	_floatInfos[s_notes_minor_img_scale_key] = &notes.minorTexScale;
	_floatInfos[s_notes_minor_img_intensity_key] = &notes.minorTexAlpha;

	_vecInfos[s_color_bg_key] = &background.color;
	_vecInfos[s_color_keyboard_major_key] = &keyboard.majorColor[0];
	_vecInfos[s_color_keyboard_minor_key] = &keyboard.minorColor[0];
	_vecInfos[s_color_keyboard_key] = &keyboard.edgeColor;
	_vecInfos[s_color_lines_key] = &score.vLinesColor; // Retro compat
	_vecInfos[s_color_lines_vertical_key] = &score.vLinesColor;
	_vecInfos[s_color_lines_horizontal_key] = &score.hLinesColor;
	_vecInfos[s_color_numbers_key] = &score.digitsColor;

	_vecInfos[s_color_major_key] = &notes.majorColors[0];
	_vecInfos[s_color_particles_key] = &particles.colors[0];
	_vecInfos[s_color_minor_key] = &notes.minorColors[0];
	_vecInfos[s_color_flashes_key] = &flashes.colors[0];

	for(size_t cid = 1; cid < notes.majorColors.size(); ++cid){
		_vecInfos[s_color_major_keys[cid]] = &notes.majorColors[cid];
		_vecInfos[s_color_particles_keys[cid]] = &particles.colors[cid];
		_vecInfos[s_color_minor_keys[cid]] = &notes.minorColors[cid];
		_vecInfos[s_color_flashes_keys[cid]] = &flashes.colors[cid];
	}

	_intInfos[s_sets_mode_key] = (int*)&setOptions.mode;
	_intInfos[s_sets_separator_key_key] = &setOptions.key;
	_floatInfos[s_keyboard_size_key] = &keyboard.size;
	_floatInfos[s_keyboard_minor_height_key] = &keyboard.minorHeight;

	_intInfos[s_min_key_key] = &minKey;
	_intInfos[s_max_key_key] = &maxKey;

	_boolInfos[s_show_pedal_key] = &showPedal;
	_floatInfos[s_pedal_size_key] = &pedals.size;
	_floatInfos[s_pedal_opacity_key] = &pedals.opacity;
	_vecInfos[s_color_pedal_key] = &pedals.centerColor; // Retro_compat
	_vecInfos[s_color_pedal_center_key] = &pedals.centerColor;
	_vecInfos[s_color_pedal_top_key] = &pedals.topColor;
	_vecInfos[s_color_pedal_right_key] = &pedals.rightColor;
	_vecInfos[s_color_pedal_left_key] = &pedals.leftColor;
	_boolInfos[s_pedal_merge_key] = &pedals.merge;
	_intInfos[s_pedal_location_key] = (int*)&pedals.location;
	_floatInfos[s_pedal_img_offset_x_key] = &pedals.margin[0];
	_floatInfos[s_pedal_img_offset_y_key] = &pedals.margin[1];
	_boolInfos[s_pedal_img_mirrored_key] = &pedals.mirror;

	_pathInfos[s_pedal_top_img_path_key] = &pedals.topImagePath;
	_pathInfos[s_pedal_center_img_path_key] = &pedals.centerImagePath;
	_pathInfos[s_pedal_side_img_paths_key] = &pedals.sideImagePaths;

	_boolInfos[s_show_wave_key] = &showWave;
	_floatInfos[s_wave_size_key] = &waves.spread;
	_floatInfos[s_wave_opacity_key] = &waves.opacity;
	_floatInfos[s_wave_amplitude_key] =  &waves.amplitude;
	_floatInfos[s_wave_frequency_key] =  &waves.frequency;
	_floatInfos[s_wave_noise_intensity_key] =  &waves.noiseIntensity;
	_floatInfos[s_wave_noise_extent_key] =  &waves.noiseSize;
	_floatInfos[s_wave_speed_key] =  &waves.speed;
	_vecInfos[s_color_wave_key] = &waves.color;

	_boolInfos[s_smooth_key] = &applyAA;
	_boolInfos[s_scroll_reverse_key] = &reverseScroll;
	_boolInfos[s_scroll_horizontal_key] = &horizontalScroll;

	_pathInfos[s_bg_img_path_key] = &background.imagePath;
	_pathInfos[s_particles_paths_key] = &particles.imagePaths;

	_pathInfos[s_notes_major_img_path_key] = &notes.majorImagePath;
	_pathInfos[s_notes_minor_img_path_key] = &notes.minorImagePath;
	_pathInfos[s_flashes_img_path_key] = &flashes.imagePath;
	_intInfos[s_flashes_img_rows_key] = &flashes.texRowCount;
	_intInfos[s_flashes_img_columns_key] = &flashes.texColCount;

	_floatInfos[s_score_lines_vertical_width_key] = &score.hLinesWidth;
	_floatInfos[s_score_lines_horizontal_width_key] = &score.vLinesWidth;
	_floatInfos[s_score_digits_size_key] = &score.digitsScale;
	_floatInfos[s_score_digits_offset_x_key] = &score.digitsOffset[0];
	_floatInfos[s_score_digits_offset_y_key] = &score.digitsOffset[1];
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

	configFile << std::endl << "# " << _sharedInfos[s_quality_key].description << " (";
	configFile << _sharedInfos[s_quality_key].values << ")" << std::endl;
	configFile << s_quality_key << ": " << Quality::availables.at(quality).name << std::endl;

	configFile << std::endl << "# " << _sharedInfos[s_layers_key].description << " (";
	configFile << _sharedInfos[s_layers_key].values << ")" << std::endl;
	configFile << s_layers_key << ": ";
	for (int i = 0; i < layersMap.size(); ++i) {
		configFile << layersMap[i] << (i != (layersMap.size() - 1) ? " " : "");
	}
	configFile << std::endl;

	configFile << std::endl << "# " << _sharedInfos[s_sets_separator_control_points_key].description << " (";
	configFile << _sharedInfos[s_sets_separator_control_points_key].values << ")" << std::endl;
	configFile << s_sets_separator_control_points_key << ": " << setOptions.toKeysString(" ") << std::endl;

	configFile << std::endl << "# " << _sharedInfos[s_filter_hide_channels_key].description << " (";
	configFile << _sharedInfos[s_filter_hide_channels_key].values << ")" << std::endl;
	configFile << s_filter_hide_channels_key <<": " << filter.toHiddenChannelsString() << std::endl;

	configFile << std::endl << "# " << _sharedInfos[s_filter_hide_tracks_key].description << " (";
	configFile << _sharedInfos[s_filter_hide_tracks_key].values << ")" << std::endl;
	configFile << s_filter_hide_tracks_key << ": " << filter.toHiddenTracksString() << std::endl;

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

		if(key == s_quality_key){
			const std::string & qualityName = arg.second[0];
			if(Quality::names.count(qualityName) > 0){
				quality = Quality::names.at(qualityName);
			}
			continue;
		}

		if(key == s_layers_key){
			const size_t bound = (std::min)(layersMap.size(), arg.second.size());
			for(size_t id = 0; id < bound; ++id){
				layersMap[id] = Configuration::parseInt(arg.second[id]);
			}
			continue;
		}

		if(key == s_sets_separator_control_points_key){
			std::string str = arg.second[0];
			for(size_t tid = 1; tid < arg.second.size(); ++tid){
				str.append(" ");
				str.append(arg.second[tid]);
			}
			setOptions.fromKeysString(str);
			continue;
		}

		if( key == s_filter_hide_channels_key ){
			filter.fillChannelsFromTokens( arg.second, false);
			continue;
		}
		if( key == s_filter_show_channels_key ){
			filter.fillChannelsFromTokens( arg.second, true );
			continue;
		}
		if( key == s_filter_hide_tracks_key ){
			filter.fillTracksFromTokens( arg.second, false );
			continue;
		}
		if( key ==s_filter_show_tracks_key ){
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
	if(configArgs.count(s_color_pedal_key) != 0){
		pedals.topColor = pedals.centerColor;
		pedals.leftColor = pedals.centerColor;
		pedals.rightColor = pedals.centerColor;
	}
	// Legacy setting for score lines.
	if(configArgs.count(s_color_lines_key) != 0){
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

	particles.turbulenceStrength = 1.0;
	particles.turbulenceScale = 1.0;

	
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
