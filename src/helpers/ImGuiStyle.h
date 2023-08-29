#ifndef ImGuiStyle_h
#define ImGuiStyle_h

struct ImFontConfig;

namespace ImGui {

void configureFont(ImFontConfig& font);
void configureStyle();
bool SliderPercent( const char* label, float* v, float v_min, float v_max );
void helpTooltip( const char* msg );

}





#endif
