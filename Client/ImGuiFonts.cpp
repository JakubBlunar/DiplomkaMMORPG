#include "ImGuiFonts.h"
#include <iostream>
#include <imgui-SFML.h>


ImGuiFonts::ImGuiFonts()
{

}


ImGuiFonts::~ImGuiFonts()
{
}

void ImGuiFonts::init() {
	for (int i = 10; i < 50; i++) {
		loadFont(PRODIGY_TINY, i);
	}
}

void ImGuiFonts::loadFont(std::string font, int size)
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* loadedFont = io.Fonts->AddFontFromFileTTF(font.c_str(), (float)size);
	fonts.insert(std::make_pair(std::make_pair(font, size), loadedFont));
}

ImFont* ImGuiFonts::getFont(std::string font, int size) {
	auto found = fonts.find(std::make_pair(font, size));
	if (found != fonts.end()) {
		return found->second;
	}

	throw "Add font" + font + " with size " + std::to_string(size) + " to init";
}
