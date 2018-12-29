#pragma once
#include <string>
#include <imgui.h>
#include <map>

class ImGuiFonts
{
	void loadFont(std::string font, int size);
	std::map<std::pair<std::string, int>, ImFont*> fonts;
public:
	ImGuiFonts();
	~ImGuiFonts();

	static constexpr const char* PRODIGY_TINY = "Data/ProggyTiny.ttf";

	void init();
	
	ImFont* getFont(std::string font, int size);
};

