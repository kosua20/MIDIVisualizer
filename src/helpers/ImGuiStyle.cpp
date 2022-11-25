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
	ImVec4 * colors						    = style.Colors;
	const ImVec4 bgColor					= ImVec4(sqrt(0.01f), sqrt(0.01f), sqrt(0.01f), 1.00f);
	const ImVec4 buttonColor				= ImVec4(sqrt(0.04f), sqrt(0.04f), sqrt(0.04f), 1.00f);
	const ImVec4 buttonHoverColor			= ImVec4(sqrt(0.10f), sqrt(0.10f), sqrt(0.10f), 1.00f);
	const ImVec4 buttonActiveColor			= ImVec4(sqrt(0.16f), sqrt(0.16f), sqrt(0.16f), 1.00f);
	const ImVec4 textColor					= ImVec4(sqrt(0.65f), sqrt(0.65f), sqrt(0.65f), 1.00f);

	colors[ImGuiCol_TextDisabled]           = ImVec4(sqrt(0.22f ), sqrt(0.22f ), sqrt(0.22f ), 1.00f);
	colors[ImGuiCol_ChildBg]                = ImVec4(sqrt(0.00f ), sqrt(0.00f ), sqrt(0.00f ), 0.00f);
	colors[ImGuiCol_PopupBg]                = ImVec4(sqrt(0.01f ), sqrt(0.01f ), sqrt(0.01f ), 1.00f);
	colors[ImGuiCol_Border]                 = ImVec4(sqrt(0.00f ), sqrt(0.00f ), sqrt(0.00f ), 0.50f);
	colors[ImGuiCol_BorderShadow]           = ImVec4(sqrt(0.00f ), sqrt(0.00f ), sqrt(0.00f ), 0.00f);
	colors[ImGuiCol_FrameBg]                = ImVec4(sqrt(0.025f), sqrt(0.025f), sqrt(0.025f), 1.00f);
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(sqrt(0.02f ), sqrt(0.02f ), sqrt(0.02f ), 1.00f);
	colors[ImGuiCol_FrameBgActive]          = ImVec4(sqrt(0.05f ), sqrt(0.05f ), sqrt(0.05f ), 1.00f);
	colors[ImGuiCol_TitleBgActive]          = ImVec4(sqrt(0.02f ), sqrt(0.02f ), sqrt(0.02f ), 1.00f);
	colors[ImGuiCol_MenuBarBg]              = ImVec4(sqrt(0.02f ), sqrt(0.02f ), sqrt(0.02f ), 1.00f);
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(sqrt(0.00f ), sqrt(0.00f ), sqrt(0.00f ), 0.00f);
	colors[ImGuiCol_TabActive]              = ImVec4(sqrt(0.14f ), sqrt(0.14f ), sqrt(0.14f ), 1.00f);
	colors[ImGuiCol_TabUnfocused]           = ImVec4(sqrt(0.02f ), sqrt(0.02f ), sqrt(0.02f ), 0.97f);
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(sqrt(0.16f ), sqrt(0.16f ), sqrt(0.16f ), 1.00f);
	colors[ImGuiCol_PlotLines]              = ImVec4(sqrt(0.34f ), sqrt(0.34f ), sqrt(0.34f ), 1.00f);
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(sqrt(0.91f ), sqrt(0.91f ), sqrt(0.91f ), 1.00f);
	colors[ImGuiCol_PlotHistogram]          = ImVec4(sqrt(0.35f ), sqrt(0.35f ), sqrt(0.35f ), 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(sqrt(0.79f ), sqrt(0.79f ), sqrt(0.79f ), 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(sqrt(0.08f ), sqrt(0.08f ), sqrt(0.08f ), 1.00f);
	colors[ImGuiCol_TableBorderStrong]      = ImVec4(sqrt(0.08f ), sqrt(0.08f ), sqrt(0.08f ), 1.00f);
	colors[ImGuiCol_TableBorderLight]       = ImVec4(sqrt(0.19f ), sqrt(0.19f ), sqrt(0.19f ), 1.00f);
	colors[ImGuiCol_TableRowBg]             = ImVec4(sqrt(0.00f ), sqrt(0.00f ), sqrt(0.00f ), 0.00f);
	colors[ImGuiCol_TableRowBgAlt]          = ImVec4(sqrt(0.04f ), sqrt(0.04f ), sqrt(0.04f ), 1.00f);
	colors[ImGuiCol_NavHighlight]           = ImVec4(sqrt(0.75f ), sqrt(0.75f ), sqrt(0.75f ), 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(sqrt(1.00f ), sqrt(1.00f ), sqrt(1.00f ), 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(sqrt(0.61f ), sqrt(0.61f ), sqrt(0.61f ), 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(sqrt(0.22f ), sqrt(0.22f ), sqrt(0.22f ), 0.35f);

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

