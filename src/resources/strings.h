#pragma once

constexpr const char* s_lock_colors_dsc						= "Should the keys and all effects use the same color";
constexpr const char* s_quality_dsc 						= "Global rendering quality";
constexpr const char* s_sets_separator_control_points_dsc 	= "Sets of control points for dynamic set assignment";
constexpr const char* s_filter_hide_channels_dsc 			= "Hidden channels" ;
constexpr const char* s_filter_show_channels_dsc 			= "Enabled channels" ;
constexpr const char* s_filter_hide_tracks_dsc 				= "Hidden tracks" ;
constexpr const char* s_filter_show_tracks_dsc 				= "Enabled tracks" ;
constexpr const char* s_colors_per_set_dsc 					= "Should each notes set use its own key/effects colors";
constexpr const char* s_sets_mode_dsc 						= "How should notes be grouped into sets";
constexpr const char* s_sets_separator_key_dsc 				= "If notes are grouped in two sets, defines the key where the split should happen";
constexpr const char* s_min_key_dsc 						= "Lowest key to display";
constexpr const char* s_max_key_dsc 						= "Highest key to display";
constexpr const char* s_smooth_dsc 							= "Apply anti-aliasing to smooth all lines";
constexpr const char* s_time_scale_dsc 						= "Vertical display scale";
constexpr const char* s_preroll_dsc 						= "Preroll time in seconds before starting to play";
constexpr const char* s_scroll_speed_dsc 					= "Playback speed";
constexpr const char* s_scroll_reverse_dsc 					= "Notes scroll from bottom to top instead of the opposite";
constexpr const char* s_scroll_horizontal_dsc 				= "Notes scroll from right to left when enabled";
constexpr const char* s_layers_dsc 							= "Active layers indices, from background to foreground";
constexpr const char* s_show_particles_dsc 					= "Should particles be shown";
constexpr const char* s_show_flashes_dsc 					= "Should flashes be shown";
constexpr const char* s_show_blur_dsc 						= "Should the blur be visible";
constexpr const char* s_show_keyboard_dsc 					= "Should the keyboard be shown";
constexpr const char* s_show_score_dsc 						= "Should the score (lines, numbers) be shown";
constexpr const char* s_show_notes_dsc 						= "Should the notes be shown";
constexpr const char* s_show_pedal_dsc 						= "Display the pedals indicator";
constexpr const char* s_show_wave_dsc 						= "Display the wave effect along the top of the keyboard";
constexpr const char* s_notes_major_img_scroll_dsc 			= "Should the major notes texture scroll with the notes";
constexpr const char* s_notes_minor_img_scroll_dsc 			= "Should the minor notes texture scroll with the notes";
constexpr const char* s_minor_size_dsc 						= "Relative size of the minor keys and notes";
constexpr const char* s_fadeout_notes_dsc 					= "Notes fade-out at the edge of the screen";
constexpr const char* s_notes_edge_width_dsc 				= "Control the width of the edge around notes";
constexpr const char* s_notes_edge_intensity_dsc 			= "Control the intensity of the edge around notes";
constexpr const char* s_notes_corner_radius_dsc  			= "Rounding radius of the notes corners";
constexpr const char* s_notes_major_img_scale_dsc   		= "Scale of the texture applied to major notes";
constexpr const char* s_notes_major_img_intensity_dsc		= "Intensity of the texture applied to major notes";
constexpr const char* s_notes_minor_img_scale_dsc   		= "Scale of the texture applied to minor notes";
constexpr const char* s_notes_minor_img_intensity_dsc		= "Intensity of the texture applied to minor notes";
constexpr const char* s_notes_major_img_path_dsc 			= "Path to an image on disk to use as texture for the major notes";
constexpr const char* s_notes_minor_img_path_dsc 			= "Path to an image on disk to use as texture for the minor notes";
constexpr const char* s_flashes_size_dsc 					= "Flash effect size";
constexpr const char* s_flashes_halo_inner_dsc 				= "Inner size of the halo surrounding the flash";
constexpr const char* s_flashes_halo_outer_dsc 				= "Outer size of the halo surrounding the flash";
constexpr const char* s_flashes_halo_intensity_dsc 			= "Brightness of the halo surrounding the flash";
constexpr const char* s_flashes_img_path_dsc 				= "Path to an image on disk to use as the flash flipbook";
constexpr const char* s_flashes_img_rows_dsc 				= "Number of rows in the flipbook image";
constexpr const char* s_flashes_img_columns_dsc 			= "Number of columns in the flipbook image";
constexpr const char* s_particles_count_dsc 				= "Number of particles in each burst";
constexpr const char* s_particles_size_dsc 					= "Size of the particles";
constexpr const char* s_particles_speed_dsc 				= "Particles speed";
constexpr const char* s_particles_expansion_dsc 			= "Particles expansion factor";
constexpr const char* s_particles_paths_dsc 				= "Set of paths (separated by spaces) to black and white images on disk to use as particles";
constexpr const char* s_particles_turbulences_dsc 			= "Particles turbulence strength";
constexpr const char* s_pedal_size_dsc 						= "Pedals size";
constexpr const char* s_pedal_opacity_dsc 					= "Pedals opacity when not pressed";
constexpr const char* s_color_pedal_dsc 					= "Pedals color when pressed";
constexpr const char* s_color_pedal_top_dsc 				= "Top pedal color when pressed";
constexpr const char* s_color_pedal_center_dsc 				= "Center pedal color when pressed";
constexpr const char* s_color_pedal_left_dsc 				= "Left color when pressed";
constexpr const char* s_color_pedal_right_dsc 				= "Right pedal color when pressed";
constexpr const char* s_pedal_merge_dsc 					= "Display only one global pedal";
constexpr const char* s_pedal_location_dsc 					= "Pedals location on screen";
constexpr const char* s_pedal_img_offset_x_dsc 				= "Horizontal overlap offset between pedals";
constexpr const char* s_pedal_img_offset_y_dsc 				= "Vertical overlap offset between pedals";
constexpr const char* s_pedal_img_mirrored_dsc 				= "Should the right pedal be mirrored horizontally";
constexpr const char* s_pedal_top_img_path_dsc 				= "Path to an image on disk to use as texture for the top pedal";
constexpr const char* s_pedal_center_img_path_dsc 			= "Path to an image on disk to use as texture for the center pedal";
constexpr const char* s_pedal_side_img_paths_dsc 			= "Paths to one or two images on disk to use as textures for the side pedals";
constexpr const char* s_keyboard_highlight_dsc 				= "Should the pressed keys be highlighted";
constexpr const char* s_keyboard_custom_colors_dsc 			= "Override notes color for pressed keys";
constexpr const char* s_keyboard_minor_edges_dsc 			= "Show edges around pressed minor keys";
constexpr const char* s_color_keyboard_major_dsc 			= "Custom color for pressed major keys";
constexpr const char* s_color_keyboard_minor_dsc 			= "Custom color for pressed minor keys";
constexpr const char* s_color_keyboard_dsc 					= "Keyboard edges and keys color";
constexpr const char* s_keyboard_size_dsc 					= "Vertical size of the keyboard";
constexpr const char* s_keyboard_minor_height_dsc 			= "Vertical fraction of the keyboard taken by minor keys";
constexpr const char* s_show_horiz_lines_dsc 				= "Should horizontal score lines be shown";
constexpr const char* s_show_vert_lines_dsc 				= "Should vertical score lines be shown";
constexpr const char* s_show_numbers_dsc 					= "Should measure numbers be shown";
constexpr const char* s_color_lines_dsc 					= "Score lines color";
constexpr const char* s_color_lines_vertical_dsc 			= "Score vertical lines color";
constexpr const char* s_color_lines_horizontal_dsc 			= "Score horizontal lines color";
constexpr const char* s_color_numbers_dsc 					= "Score measure numbers color";
constexpr const char* s_score_lines_vertical_width_dsc 		= "Score vertical lines height, in pixels";
constexpr const char* s_score_lines_horizontal_width_dsc	= "Score horizontal lines width, in pixels";
constexpr const char* s_score_digits_size_dsc 				= "Score digits size";
constexpr const char* s_score_digits_offset_x_dsc 			= "Score digits horizontal offset, as a fraction of a digit";
constexpr const char* s_score_digits_offset_y_dsc 			= "Score digits vertical offset, as a fraction of a digit";
constexpr const char* s_wave_size_dsc 						= "Wave effect size";
constexpr const char* s_wave_opacity_dsc 					= "Wave effect opacity";
constexpr const char* s_wave_amplitude_dsc 					= "Wave effect amplitude";
constexpr const char* s_wave_frequency_dsc 					= "Wave effect frequency";
constexpr const char* s_color_wave_dsc 						= "Wave effect color";
constexpr const char* s_wave_speed_dsc 						= "Wave scrolling speed" ;
constexpr const char* s_wave_noise_intensity_dsc 			= "Wave noise intensity" ;
constexpr const char* s_wave_noise_extent_dsc 				= "Wave noise extent on screen" ;
constexpr const char* s_show_blur_notes_dsc 				= "Should the notes be part of the blur";
constexpr const char* s_bg_img_behind_keyboard_dsc			= "Should the background image extend behind the keyboard";
constexpr const char* s_show_bg_img_dsc 					= "Use a background texture";
constexpr const char* s_blur_attenuation_dsc 				= "Blur attenuation speed";
constexpr const char* s_bg_img_opacity_dsc 					= "Background opacity";
constexpr const char* s_color_bg_dsc 						= "Background color";
constexpr const char* s_bg_img_path_dsc 					= "Path to an image on disk to use as background";
constexpr const char* s_color_major_dsc 					= "Major notes color";
constexpr const char* s_color_particles_dsc 				= "Particles color";
constexpr const char* s_color_minor_dsc 					= "Minor notes color";
constexpr const char* s_color_flashes_dsc 					= "Flash effect color";

#define COLOR_MAJOR_DSC 	"Major notes color for set "
#define COLOR_MINOR_DSC 	"Minor notes color for set "
#define COLOR_PARTICLE_DSC 	"Particles color for set "
#define COLOR_FLASHES_DSC 	"Flash effect color for set "

constexpr const char* s_color_major_dscs[] = {
	COLOR_MAJOR_DSC "0",
	COLOR_MAJOR_DSC "1",
	COLOR_MAJOR_DSC "2",
	COLOR_MAJOR_DSC "3",
	COLOR_MAJOR_DSC "4",
	COLOR_MAJOR_DSC "5",
	COLOR_MAJOR_DSC "6",
	COLOR_MAJOR_DSC "7",
	COLOR_MAJOR_DSC "8",
	COLOR_MAJOR_DSC "9",
	COLOR_MAJOR_DSC "10",
	COLOR_MAJOR_DSC "11"
};

constexpr const char* s_color_minor_dscs[] {
	COLOR_MINOR_DSC "0",
	COLOR_MINOR_DSC "1",
	COLOR_MINOR_DSC "2",
	COLOR_MINOR_DSC "3",
	COLOR_MINOR_DSC "4",
	COLOR_MINOR_DSC "5",
	COLOR_MINOR_DSC "6",
	COLOR_MINOR_DSC "7",
	COLOR_MINOR_DSC "8",
	COLOR_MINOR_DSC "9",
	COLOR_MINOR_DSC "10",
	COLOR_MINOR_DSC "11"
};

constexpr const char* s_color_particles_dscs[]{
	COLOR_PARTICLE_DSC "0",
	COLOR_PARTICLE_DSC "1",
	COLOR_PARTICLE_DSC "2",
	COLOR_PARTICLE_DSC "3",
	COLOR_PARTICLE_DSC "4",
	COLOR_PARTICLE_DSC "5",
	COLOR_PARTICLE_DSC "6",
	COLOR_PARTICLE_DSC "7",
	COLOR_PARTICLE_DSC "8",
	COLOR_PARTICLE_DSC "9",
	COLOR_PARTICLE_DSC "10",
	COLOR_PARTICLE_DSC "11"
};

constexpr const char* s_color_flashes_dscs[]{
	COLOR_FLASHES_DSC "0",
	COLOR_FLASHES_DSC "1",
	COLOR_FLASHES_DSC "2",
	COLOR_FLASHES_DSC "3",
	COLOR_FLASHES_DSC "4",
	COLOR_FLASHES_DSC "5",
	COLOR_FLASHES_DSC "6",
	COLOR_FLASHES_DSC "7",
	COLOR_FLASHES_DSC "8",
	COLOR_FLASHES_DSC "9",
	COLOR_FLASHES_DSC "10",
	COLOR_FLASHES_DSC "11"
};
