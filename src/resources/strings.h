#pragma once

constexpr const char* s_lock_colors 						= "lock-colors";
constexpr const char* s_lock_colors_dsc						= "Should the keys and all effects use the same color";
constexpr const char* s_quality 							= "quality";
constexpr const char* s_quality_dsc 						= "Global rendering quality";
constexpr const char* s_sets_separator_control_points 		= "sets-separator-control-points";
constexpr const char* s_sets_separator_control_points_dsc 	= "Sets of control points for dynamic set assignment";
constexpr const char* s_filter_hide_channels 				= "filter-hide-channels";
constexpr const char* s_filter_hide_channels_dsc 			= "Hidden channels" ;
constexpr const char* s_filter_show_channels 				= "filter-show-channels";
constexpr const char* s_filter_show_channels_dsc 			= "Enabled channels" ;
constexpr const char* s_filter_hide_tracks 					= "filter-hide-tracks";
constexpr const char* s_filter_hide_tracks_dsc 				= "Hidden tracks" ;
constexpr const char* s_filter_show_tracks 					= "filter-show-tracks";
constexpr const char* s_filter_show_tracks_dsc 				= "Enabled tracks" ;
constexpr const char* s_colors_per_set 						= "colors-per-set";
constexpr const char* s_colors_per_set_dsc 					= "Should each notes set use its own key/effects colors";
constexpr const char* s_sets_mode 							= "sets-mode";
constexpr const char* s_sets_mode_dsc 						= "How should notes be grouped into sets";
constexpr const char* s_sets_separator_key 					= "sets-separator-key";
constexpr const char* s_sets_separator_key_dsc 				= "If notes are grouped in two sets, defines the key where the split should happen";
constexpr const char* s_min_key 							= "min-key";
constexpr const char* s_min_key_dsc 						= "Lowest key to display";
constexpr const char* s_max_key 							= "max-key";
constexpr const char* s_max_key_dsc 						= "Highest key to display";
constexpr const char* s_smooth 								= "smooth";
constexpr const char* s_smooth_dsc 							= "Apply anti-aliasing to smooth all lines";
constexpr const char* s_time_scale 							= "time-scale";
constexpr const char* s_time_scale_dsc 						= "Vertical display scale";
constexpr const char* s_preroll 							= "preroll";
constexpr const char* s_preroll_dsc 						= "Preroll time in seconds before starting to play";
constexpr const char* s_scroll_speed 						= "scroll-speed";
constexpr const char* s_scroll_speed_dsc 					= "Playback speed";
constexpr const char* s_scroll_reverse 						= "scroll-reverse";
constexpr const char* s_scroll_reverse_dsc 					= "Notes scroll from bottom to top instead of the opposite";
constexpr const char* s_scroll_horizontal 					= "scroll-horizontal";
constexpr const char* s_scroll_horizontal_dsc 				= "Notes scroll from right to left when enabled";
constexpr const char* s_layers 								= "layers";
constexpr const char* s_layers_dsc 							= "Active layers indices, from background to foreground";
constexpr const char* s_show_particles 						= "show-particles";
constexpr const char* s_show_particles_dsc 					= "Should particles be shown";
constexpr const char* s_show_flashes 						= "show-flashes";
constexpr const char* s_show_flashes_dsc 					= "Should flashes be shown";
constexpr const char* s_show_blur 							= "show-blur";
constexpr const char* s_show_blur_dsc 						= "Should the blur be visible";
constexpr const char* s_show_keyboard 						= "show-keyboard";
constexpr const char* s_show_keyboard_dsc 					= "Should the keyboard be shown";
constexpr const char* s_show_score 							= "show-score";
constexpr const char* s_show_score_dsc 						= "Should the score (lines, numbers) be shown";
constexpr const char* s_show_notes 							= "show-notes";
constexpr const char* s_show_notes_dsc 						= "Should the notes be shown";
constexpr const char* s_show_pedal 							= "show-pedal";
constexpr const char* s_show_pedal_dsc 						= "Display the pedals indicator";
constexpr const char* s_show_wave 							= "show-wave";
constexpr const char* s_show_wave_dsc 						= "Display the wave effect along the top of the keyboard";
constexpr const char* s_notes_major_img_scroll 				= "notes-major-img-scroll";
constexpr const char* s_notes_major_img_scroll_dsc 			= "Should the major notes texture scroll with the notes";
constexpr const char* s_notes_minor_img_scroll 				= "notes-minor-img-scroll";
constexpr const char* s_notes_minor_img_scroll_dsc 			= "Should the minor notes texture scroll with the notes";
constexpr const char* s_minor_size 							= "minor-size";
constexpr const char* s_minor_size_dsc 						= "Relative size of the minor keys and notes";
constexpr const char* s_fadeout_notes 						= "fadeout-notes";
constexpr const char* s_fadeout_notes_dsc 					= "Notes fade-out at the edge of the screen";
constexpr const char* s_notes_edge_width 					= "notes-edge-width";
constexpr const char* s_notes_edge_width_dsc 				= "Control the width of the edge around notes";
constexpr const char* s_notes_edge_intensity 				= "notes-edge-intensity";
constexpr const char* s_notes_edge_intensity_dsc 			= "Control the intensity of the edge around notes";
constexpr const char* s_notes_corner_radius 				= "notes-corner-radius";
constexpr const char* s_notes_corner_radius_dsc  			= "Rounding radius of the notes corners";
constexpr const char* s_notes_major_img_scale 				= "notes-major-img-scale";
constexpr const char* s_notes_major_img_scale_dsc   		= "Scale of the texture applied to major notes";
constexpr const char* s_notes_major_img_intensity 			= "notes-major-img-intensity";
constexpr const char* s_notes_major_img_intensity_dsc		= "Intensity of the texture applied to major notes";
constexpr const char* s_notes_minor_img_scale 				= "notes-minor-img-scale";
constexpr const char* s_notes_minor_img_scale_dsc   		= "Scale of the texture applied to minor notes";
constexpr const char* s_notes_minor_img_intensity 			= "notes-minor-img-intensity";
constexpr const char* s_notes_minor_img_intensity_dsc		= "Intensity of the texture applied to minor notes";
constexpr const char* s_notes_major_img_path 				= "notes-major-img-path";
constexpr const char* s_notes_major_img_path_dsc 			= "Path to an image on disk to use as texture for the major notes";
constexpr const char* s_notes_minor_img_path 				= "notes-minor-img-path";
constexpr const char* s_notes_minor_img_path_dsc 			= "Path to an image on disk to use as texture for the minor notes";
constexpr const char* s_flashes_size 						= "flashes-size";
constexpr const char* s_flashes_size_dsc 					= "Flash effect size";
constexpr const char* s_flashes_halo_inner 					= "flashes-halo-inner";
constexpr const char* s_flashes_halo_inner_dsc 				= "Inner size of the halo surrounding the flash";
constexpr const char* s_flashes_halo_outer 					= "flashes-halo-outer";
constexpr const char* s_flashes_halo_outer_dsc 				= "Outer size of the halo surrounding the flash";
constexpr const char* s_flashes_halo_intensity 				= "flashes-halo-intensity";
constexpr const char* s_flashes_halo_intensity_dsc 			= "Brightness of the halo surrounding the flash";
constexpr const char* s_flashes_img_path 					= "flashes-img-path";
constexpr const char* s_flashes_img_path_dsc 				= "Path to an image on disk to use as the flash flipbook";
constexpr const char* s_flashes_img_rows 					= "flashes-img-rows";
constexpr const char* s_flashes_img_rows_dsc 				= "Number of rows in the flipbook image";
constexpr const char* s_flashes_img_columns 				= "flashes-img-columns";
constexpr const char* s_flashes_img_columns_dsc 			= "Number of columns in the flipbook image";
constexpr const char* s_particles_count 					= "particles-count";
constexpr const char* s_particles_count_dsc 				= "Number of particles in each burst";
constexpr const char* s_particles_size 						= "particles-size";
constexpr const char* s_particles_size_dsc 					= "Size of the particles";
constexpr const char* s_particles_speed 					= "particles-speed";
constexpr const char* s_particles_speed_dsc 				= "Particles speed";
constexpr const char* s_particles_expansion 				= "particles-expansion";
constexpr const char* s_particles_expansion_dsc 			= "Particles expansion factor";
constexpr const char* s_particles_paths 					= "particles-paths";
constexpr const char* s_particles_paths_dsc 				= "Set of paths (separated by spaces) to black and white images on disk to use as particles";
constexpr const char* s_particles_turbulences 				= "particles-turbulences";
constexpr const char* s_particles_turbulences_dsc 			= "Particles turbulence strength";
constexpr const char* s_pedal_size 							= "pedal-size";
constexpr const char* s_pedal_size_dsc 						= "Pedals size";
constexpr const char* s_pedal_opacity 						= "pedal-opacity";
constexpr const char* s_pedal_opacity_dsc 					= "Pedals opacity when not pressed";
constexpr const char* s_color_pedal 						= "color-pedal";
constexpr const char* s_color_pedal_dsc 					= "Pedals color when pressed";
constexpr const char* s_color_pedal_top 					= "color-pedal-top";
constexpr const char* s_color_pedal_top_dsc 				= "Top pedal color when pressed";
constexpr const char* s_color_pedal_center 					= "color-pedal-center";
constexpr const char* s_color_pedal_center_dsc 				= "Center pedal color when pressed";
constexpr const char* s_color_pedal_left 					= "color-pedal-left";
constexpr const char* s_color_pedal_left_dsc 				= "Left color when pressed";
constexpr const char* s_color_pedal_right 					= "color-pedal-right";
constexpr const char* s_color_pedal_right_dsc 				= "Right pedal color when pressed";
constexpr const char* s_pedal_merge 						= "pedal-merge";
constexpr const char* s_pedal_merge_dsc 					= "Display only one global pedal";
constexpr const char* s_pedal_location 						= "pedal-location";
constexpr const char* s_pedal_location_dsc 					= "Pedals location on screen";
constexpr const char* s_pedal_img_offset_x 					= "pedal-img-offset-x";
constexpr const char* s_pedal_img_offset_x_dsc 				= "Horizontal overlap offset between pedals";
constexpr const char* s_pedal_img_offset_y 					= "pedal-img-offset-y";
constexpr const char* s_pedal_img_offset_y_dsc 				= "Vertical overlap offset between pedals";
constexpr const char* s_pedal_img_mirrored 					= "pedal-img-mirrored";
constexpr const char* s_pedal_img_mirrored_dsc 				= "Should the right pedal be mirrored horizontally";
constexpr const char* s_pedal_top_img_path 					= "pedal-top-img-path";
constexpr const char* s_pedal_top_img_path_dsc 				= "Path to an image on disk to use as texture for the top pedal";
constexpr const char* s_pedal_center_img_path 				= "pedal-center-img-path";
constexpr const char* s_pedal_center_img_path_dsc 			= "Path to an image on disk to use as texture for the center pedal";
constexpr const char* s_pedal_side_img_paths 				= "pedal-side-img-paths";
constexpr const char* s_pedal_side_img_paths_dsc 			= "Paths to one or two images on disk to use as textures for the side pedals";
constexpr const char* s_keyboard_highlight 					= "keyboard-highlight";
constexpr const char* s_keyboard_highlight_dsc 				= "Should the pressed keys be highlighted";
constexpr const char* s_keyboard_custom_colors 				= "keyboard-custom-colors";
constexpr const char* s_keyboard_custom_colors_dsc 			= "Override notes color for pressed keys";
constexpr const char* s_keyboard_minor_edges 				= "keyboard-minor-edges";
constexpr const char* s_keyboard_minor_edges_dsc 			= "Show edges around pressed minor keys";
constexpr const char* s_color_keyboard_major 				= "color-keyboard-major";
constexpr const char* s_color_keyboard_major_dsc 			= "Custom color for pressed major keys";
constexpr const char* s_color_keyboard_minor 				= "color-keyboard-minor";
constexpr const char* s_color_keyboard_minor_dsc 			= "Custom color for pressed minor keys";
constexpr const char* s_color_keyboard 						= "color-keyboard";
constexpr const char* s_color_keyboard_dsc 					= "Keyboard edges and keys color";
constexpr const char* s_keyboard_size 						= "keyboard-size";
constexpr const char* s_keyboard_size_dsc 					= "Vertical size of the keyboard";
constexpr const char* s_keyboard_minor_height 				= "keyboard-minor-height";
constexpr const char* s_keyboard_minor_height_dsc 			= "Vertical fraction of the keyboard taken by minor keys";
constexpr const char* s_show_horiz_lines 					= "show-horiz-lines";
constexpr const char* s_show_horiz_lines_dsc 				= "Should horizontal score lines be shown";
constexpr const char* s_show_vert_lines 					= "show-vert-lines";
constexpr const char* s_show_vert_lines_dsc 				= "Should vertical score lines be shown";
constexpr const char* s_show_numbers 						= "show-numbers";
constexpr const char* s_show_numbers_dsc 					= "Should measure numbers be shown";
constexpr const char* s_color_lines 						= "color-lines";
constexpr const char* s_color_lines_dsc 					= "Score lines color";
constexpr const char* s_color_lines_vertical 				= "color-lines-vertical";
constexpr const char* s_color_lines_vertical_dsc 			= "Score vertical lines color";
constexpr const char* s_color_lines_horizontal 				= "color-lines-horizontal";
constexpr const char* s_color_lines_horizontal_dsc 			= "Score horizontal lines color";
constexpr const char* s_color_numbers 						= "color-numbers";
constexpr const char* s_color_numbers_dsc 					= "Score measure numbers color";
constexpr const char* s_score_lines_vertical_width 			= "score-lines-vertical-width";
constexpr const char* s_score_lines_vertical_width_dsc 		= "Score vertical lines height, in pixels";
constexpr const char* s_score_lines_horizontal_width 		= "score-lines-horizontal-width";
constexpr const char* s_score_lines_horizontal_width_dsc	= "Score horizontal lines width, in pixels";
constexpr const char* s_score_digits_size 					= "score-digits-size";
constexpr const char* s_score_digits_size_dsc 				= "Score digits size";
constexpr const char* s_score_digits_offset_x 				= "score-digits-offset-x";
constexpr const char* s_score_digits_offset_x_dsc 			= "Score digits horizontal offset, as a fraction of a digit";
constexpr const char* s_score_digits_offset_y 				= "score-digits-offset-y";
constexpr const char* s_score_digits_offset_y_dsc 			= "Score digits vertical offset, as a fraction of a digit";
constexpr const char* s_wave_size 							= "wave-size";
constexpr const char* s_wave_size_dsc 						= "Wave effect size";
constexpr const char* s_wave_opacity 						= "wave-opacity";
constexpr const char* s_wave_opacity_dsc 					= "Wave effect opacity";
constexpr const char* s_wave_amplitude 						= "wave-amplitude";
constexpr const char* s_wave_amplitude_dsc 					= "Wave effect amplitude";
constexpr const char* s_wave_frequency 						= "wave-frequency";
constexpr const char* s_wave_frequency_dsc 					= "Wave effect frequency";
constexpr const char* s_color_wave 							= "color-wave";
constexpr const char* s_color_wave_dsc 						= "Wave effect color";
constexpr const char* s_wave_speed 							= "wave-speed";
constexpr const char* s_wave_speed_dsc 						= "Wave scrolling speed" ;
constexpr const char* s_wave_noise_intensity 				= "wave-noise-intensity";
constexpr const char* s_wave_noise_intensity_dsc 			= "Wave noise intensity" ;
constexpr const char* s_wave_noise_extent 					= "wave-noise-extent";
constexpr const char* s_wave_noise_extent_dsc 				= "Wave noise extent on screen" ;
constexpr const char* s_show_blur_notes 					= "show-blur-notes";
constexpr const char* s_show_blur_notes_dsc 				= "Should the notes be part of the blur";
constexpr const char* s_bg_img_behind_keyboard 				= "bg-img-behind-keyboard";
constexpr const char* s_bg_img_behind_keyboard_dsc			= "Should the background image extend behind the keyboard";
constexpr const char* s_show_bg_img 						= "show-bg-img";
constexpr const char* s_show_bg_img_dsc 					= "Use a background texture";
constexpr const char* s_blur_attenuation 					= "blur-attenuation";
constexpr const char* s_blur_attenuation_dsc 				= "Blur attenuation speed";
constexpr const char* s_bg_img_opacity 						= "bg-img-opacity";
constexpr const char* s_bg_img_opacity_dsc 					= "Background opacity";
constexpr const char* s_color_bg 							= "color-bg";
constexpr const char* s_color_bg_dsc 						= "Background color";
constexpr const char* s_bg_img_path 						= "bg-img-path";
constexpr const char* s_bg_img_path_dsc 					= "Path to an image on disk to use as background";
constexpr const char* s_color_major 						= "color-major";
constexpr const char* s_color_major_dsc 					= "Major notes color";
constexpr const char* s_color_particles 					= "color-particles";
constexpr const char* s_color_particles_dsc 				= "Particles color";
constexpr const char* s_color_minor 						= "color-minor";
constexpr const char* s_color_minor_dsc 					= "Minor notes color";
constexpr const char* s_color_flashes 						= "color-flashes";
constexpr const char* s_color_flashes_dsc 					= "Flash effect color";


#define COLOR_MAJOR_KEY		"color-major-"
#define COLOR_MINOR_KEY		"color-minor-"
#define COLOR_PARTICLE_KEY	"color-particles-"
#define COLOR_FLASHES_KEY	"color-flashes-"

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
