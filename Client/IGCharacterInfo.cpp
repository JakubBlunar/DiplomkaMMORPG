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

	float skills = 0;

	ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 16));

	ImGui::SetNextWindowSizeConstraints(ImVec2(160, 100), ImVec2(-1, FLT_MAX));
	ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);

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

	focused = ImGui::IsWindowFocused();
	ImGui::End();

	ImGui::PopFont();
}


