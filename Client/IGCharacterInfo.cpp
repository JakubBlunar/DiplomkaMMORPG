#include "IGCharacterInfo.h"
#include "ImGuiFonts.h"
#include "Game.h"
#include "EventIncreaseCharacterAttribute.h"


IGCharacterInfo::IGCharacterInfo()
{
}


IGCharacterInfo::~IGCharacterInfo()
{
}

void IGCharacterInfo::render(Game* g, IGManager* manager) {
	if (!player) return;

	

	ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 16));

	ImGui::SetNextWindowSizeConstraints(ImVec2(180, 200), ImVec2(-1, FLT_MAX));
	ImGui::SetNextWindowPos(ImVec2(1055, 225), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 255), ImGuiCond_FirstUseEver);

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

	float skills = attributes->getAttribute(EntityAttributeType::FREE_ATTRIBUTES);
	std::string attribute = "Stats";
	if (skills > 0) {
		attribute += " (" + convertFloatToString(skills, 0) + " free)";
	}

	ImGui::Text(attribute.c_str());


	attribute = "Strength: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::STRENGTH), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+##1")) {
			increaseAttribute(EntityAttributeType::STRENGTH, g);
		}
	}

	attribute = "Agility: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::AGILITY), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+##2")) {
			increaseAttribute(EntityAttributeType::AGILITY, g);
		}
	}

	attribute = "Intellect: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::INTELECT), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+##3")) {
			increaseAttribute(EntityAttributeType::INTELECT, g);
		}
	}

	attribute = "Stamina: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::STAMINA), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+##4")) {
			increaseAttribute(EntityAttributeType::STAMINA, g);
		}
	}

	attribute = "Spirit: " + convertFloatToString(attributes->getAttribute(EntityAttributeType::SPIRIT), 0);
	ImGui::Text(attribute.c_str());
	if (skills > 0) {
		ImGui::SameLine();
		if (ImGui::Button("+##5")) {
			increaseAttribute(EntityAttributeType::SPIRIT, g);
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

void IGCharacterInfo::increaseAttribute(EntityAttributeType attribute, Game* g) const {
	EventIncreaseCharacterAttribute e(player->getId(), attribute);
	sf::Packet* p = e.toPacket();
	g->packet_manager->sendPacket(p);
	delete p;
}


