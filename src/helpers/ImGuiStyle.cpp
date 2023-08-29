#include "ImGuiStyle.h"

#include <imgui/imgui.h>
#include <cmath>

#include "../resources/font_data_Lato.h"

void ImGui::configureFont(ImFontConfig& font){
	font = ImFontConfig();
	font.FontData = (void*)(fontDataLato);
	font.FontDataSize = size_fontDataLato;
	font.SizePixels = 16.0f;
	// Font data is static
	font.FontDataOwnedByAtlas = false;

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL;
	io.Fonts->AddFont(&font);
}

void ImGui::configureStyle(){
	// Customize the style.
	ImGui::StyleColorsDark();
	ImGuiStyle & style = ImGui::GetStyle();
	// Colors.
	const ImVec4 a( 0.9f, 0.7f, 1.0f, 1.0f );
	ImVec4* colors = style.Colors;
	const ImVec4 textColor = ImVec4( 0.95f, 0.95f, 0.95f, 1.00f );
	const ImVec4 bgColor					= ImVec4(0.10f * a.x, 0.10f * a.y, 0.10f * a.z, 1.00f);
	const ImVec4 buttonColor				= ImVec4(0.20f * a.x, 0.20f * a.y, 0.20f * a.z, 1.00f);
	const ImVec4 buttonHoverColor			= ImVec4(0.32f * a.x, 0.32f * a.y, 0.32f * a.z, 1.00f);
	const ImVec4 buttonActiveColor			= ImVec4(0.40f * a.x, 0.40f * a.y, 0.40f * a.z, 1.00f);
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.7f, 0.7f, 0.7f, 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(0.00f * a.x, 0.00f * a.y, 0.00f * a.z, 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(0.10f * a.x, 0.10f * a.y, 0.10f * a.z, 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(0.00f * a.x, 0.00f * a.y, 0.00f * a.z, 0.50f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f * a.x, 0.00f * a.y, 0.00f * a.z, 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(0.16f * a.x, 0.16f * a.y, 0.16f * a.z, 1.00f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.14f * a.x, 0.14f * a.y, 0.14f * a.z, 1.00f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.22f * a.x, 0.22f * a.y, 0.22f * a.z, 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.14f * a.x, 0.14f * a.y, 0.14f * a.z, 1.00f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f * a.x, 0.14f * a.y, 0.14f * a.z, 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.00f * a.x, 0.00f * a.y, 0.00f * a.z, 0.00f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.37f * a.x, 0.37f * a.y, 0.37f * a.z, 1.00f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.14f * a.x, 0.14f * a.y, 0.14f * a.z, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.40f * a.x, 0.40f * a.y, 0.40f * a.z, 1.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(0.58f * a.x, 0.58f * a.y, 0.58f * a.z, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.95f * a.x, 0.95f * a.y, 0.95f * a.z, 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.59f * a.x, 0.59f * a.y, 0.59f * a.z, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.89f * a.x, 0.89f * a.y, 0.89f * a.z, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.28f * a.x, 0.28f * a.y, 0.28f * a.z, 1.00f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.28f * a.x, 0.28f * a.y, 0.28f * a.z, 1.00f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(0.44f * a.x, 0.44f * a.y, 0.44f * a.z, 1.00f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f * a.x, 0.00f * a.y, 0.00f * a.z, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.20f * a.x, 0.20f * a.y, 0.20f * a.z, 1.00f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(0.87f * a.x, 0.87f * a.y, 0.87f * a.z, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f * a.x, 1.00f * a.y, 1.00f * a.z, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.78f * a.x, 0.78f * a.y, 0.78f * a.z, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.47f * a.x, 0.47f * a.y, 0.47f * a.z, 0.35f);

	colors[ImGuiCol_Text]                   = textColor;
	colors[ImGuiCol_CheckMark]              = textColor;
	colors[ImGuiCol_WindowBg]               = bgColor;
	colors[ImGuiCol_TitleBg]                = bgColor;
	colors[ImGuiCol_TitleBgCollapsed]       = bgColor;
	colors[ImGuiCol_ScrollbarGrab]          = buttonColor;
	colors[ImGuiCol_ScrollbarGrabHovered]   = buttonHoverColor;
	colors[ImGuiCol_ScrollbarGrabActive]    = buttonActiveColor;
	colors[ImGuiCol_SliderGrab]             = buttonColor;
	colors[ImGuiCol_SliderGrabActive]       = buttonActiveColor;
	colors[ImGuiCol_Button]                 = buttonColor;
	colors[ImGuiCol_ButtonHovered]          = buttonHoverColor;
	colors[ImGuiCol_ButtonActive]           = buttonActiveColor;
	colors[ImGuiCol_Header]                 = buttonColor;
	colors[ImGuiCol_HeaderHovered]          = buttonHoverColor;
	colors[ImGuiCol_HeaderActive]           = buttonActiveColor;
	colors[ImGuiCol_Separator]              = buttonColor;
	colors[ImGuiCol_SeparatorHovered]       = buttonHoverColor;
	colors[ImGuiCol_SeparatorActive]        = buttonActiveColor;
	colors[ImGuiCol_ResizeGrip]             = buttonColor;
	colors[ImGuiCol_ResizeGripHovered]      = buttonHoverColor;
	colors[ImGuiCol_ResizeGripActive]       = buttonActiveColor;
	colors[ImGuiCol_Tab]                    = buttonColor;
	colors[ImGuiCol_TabHovered]             = buttonHoverColor;
	colors[ImGuiCol_TextSelectedBg]         = buttonColor;
	colors[ImGuiCol_DragDropTarget]         = textColor;

	// Frames.
	style.FrameRounding		 = 5.0f;
	style.GrabRounding		 = 3.0f;
	style.WindowRounding	 = 5.0f;
	style.ScrollbarRounding  = 12.0f;
	style.ScrollbarSize		 = 12.0f;
	style.WindowTitleAlign.x = 0.5f;
	style.FramePadding.y	 = 4.0f;
	style.ItemSpacing.y		 = 3.0f;
}

bool ImGui::SliderPercent( const char* label, float* v, float v_min, float v_max )
{
	float tmp = ( *v ) * 100.0f;
	const bool press = ImGui::SliderFloat( label, &tmp, v_min * 100.0f, v_max * 100.0f, "%.0f%%" );
	if( press )
	{
		*v = tmp / 100.0f;
	}
	return press;
}

void ImGui::helpTooltip( const char* msg )
{
	if( ImGui::IsItemHovered() )
	{
		ImGui::SetTooltip( "%s", msg );
	}
}