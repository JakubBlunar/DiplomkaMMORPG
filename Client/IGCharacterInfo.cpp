#include "IGCharacterInfo.h"
#include "ImGuiFonts.h"
#include "Game.h"


IGCharacterInfo::IGCharacterInfo()
{
}


IGCharacterInfo::~IGCharacterInfo()
{
}

void IGCharacterInfo::render(Game* g, IGManager* manager) {
	if (!player) return;

	float skills = 5;

	ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 16));

	ImGui::SetNextWindowSizeConstraints(ImVec2(180, 150), ImVec2(-1, FLT_MAX));
	ImGui::SetNextWindowPos(ImVec2(1055, 225), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(180, 255), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Character Info", &visible)) {
		focused = false;
		ImGui::End();
		ImGui::PopFont();
		return;
	}

	ImGui::PushItemWidth(-1);

	ImGui::TextWrapped(player->getName().c_str());
	float playerLevel = attributes->getAttribute(EntityAttributeType::LEVEL);
	std::string level = "Level: " + convertFloatToString(playerLevel, 0);
	level += " ("+ convertFloatToString(attributes->getAttribute(EntityAttributeType::EXPERIENCE), 0) + " / " + std::to_string(attributes->getXpForLevel((int)playerLevel + 1))+")";
	ImGui::Text(level.c_str());

	ImGui::Separator();

	
	std::string attribute = "Stats";
	if (skills > 0) {
		attribute += " (" + convertFloatToString(skills, 0) + " free)";
	}

	ImGui::Text(attribute.c_str());


	attribute = "Strength: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::STRENGTH), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+")) {
			
		}
	}

	attribute = "Agility: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::AGILITY), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+")) {
			
		}
	}

	attribute = "Intellect: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::INTELECT), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+")) {
			
		}
	}

	attribute = "Stamina: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::STAMINA), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+")) {
			
		}
	}

	attribute = "Spirit: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::SPIRIT), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+")) {
			
		}
	}

	attribute = "Armor: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::ARMOR), 0);
	ImGui::Text(attribute.c_str());
	ImGui::PopItemWidth();

	position = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();
	focused = ImGui::IsWindowFocused();
	ImGui::End();

	ImGui::PopFont();
}


