#pragma once
#include <string>
#include <imgui.h>
#include <map>

class ImGuiFonts
{
	std::map<std::pair<std::string, int>, ImFont*> fonts;
public:
	ImGuiFonts();
	~ImGuiFonts();

	static constexpr const char* PRODIGY_TINY = "Data/ProggyTiny.ttf";

	ImFont* getFont(std::string font, int size);
};

