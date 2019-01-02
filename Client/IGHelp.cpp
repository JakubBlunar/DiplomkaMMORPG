#include "IGHelp.h"
#include "Game.h"


IGHelp::IGHelp() {
	selectedMenuItem = 0;
}


IGHelp::~IGHelp() {}

void IGHelp::render(Game* g, IGManager* manager) {

	ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 15));

	ImGui::SetNextWindowPos(ImVec2(250, 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(550, 440), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Help", &visible)) {
		ImGui::End();
		ImGui::PopFont();
		return;
	}

	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Close Console"))
			visible = false;
		ImGui::EndPopup();
	}

	ImGui::SameLine();


	const char* items[] = {"Info", "Controls",};

	ImGui::BeginChild("Menu", ImVec2(120, -1), true);

	ImGui::Text("Menu");
	ImGui::PushItemWidth(100);
	ImGui::ListBox("", &selectedMenuItem, items, IM_ARRAYSIZE(items), 19);
	ImGui::PopItemWidth();
	ImGui::EndChild();

	ImGui::SameLine();


	ImGui::BeginChild("Content", ImVec2(-1, -1), false);
	if (selectedMenuItem == 0) {
		ImGui::TextWrapped("Akalitasia game by Jakub Blunar");
	}
	else if (selectedMenuItem == 1) {
		std::string s =
			R"(
Controls:

For movement use W,S,A,D
Use spells by clicking on action bar or by pressing number 0-9.
Press Enter for writing into chat.
Press C to display character info
		)";
		ImGui::TextWrapped(s.c_str());
	}


	ImGui::EndChild();


	position = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();
	focused = ImGui::IsWindowFocused() && !ImGui::IsWindowCollapsed();

	ImGui::End();
	ImGui::PopFont();
}
