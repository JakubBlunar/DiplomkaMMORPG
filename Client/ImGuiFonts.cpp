#include "ImGuiFonts.h"
#include <iostream>
#include <imgui-SFML.h>


ImGuiFonts::ImGuiFonts()
{

}


ImGuiFonts::~ImGuiFonts()
{
}

ImFont* ImGuiFonts::getFont(std::string font, int size) {
	auto found = fonts.find(std::make_pair(font, size));
	if (found != fonts.end()) {
		return found->second;
	}

	ImGuiIO& io = ImGui::GetIO();
	ImFont* loadedFont = io.Fonts->AddFontFromFileTTF(font.c_str(), size);
	fonts.insert(std::make_pair(std::make_pair(font, size), loadedFont));
	ImGui::SFML::UpdateFontTexture();
	
	return loadedFont;
}
