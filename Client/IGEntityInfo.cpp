#include "IGEntityInfo.h"
#include "Entity.h"
#include "Npc.h"
#include "Player.h"
#include "Game.h"
#include <iomanip>
#include <imgui-SFML.h>
#include "../Server/EntityAttributes.h"

IGEntityInfo::IGEntityInfo(string id, sf::Vector2f position)
{
	this->id = id;
	entity = nullptr;
	setPosition(position, false);
	setSize(sf::Vector2f(220, 80));
}


IGEntityInfo::~IGEntityInfo()
{
}

void IGEntityInfo::setEntity(Entity* entity) {
	this->entity = entity;
}

Entity* IGEntityInfo::getEntity() const {
	return entity;
}

void IGEntityInfo::render(Game * g, IGManager * manager)
{
	if (!entity) {
		return;
	}

	Npc* npc = dynamic_cast<Npc*>(entity);
	Player * player = dynamic_cast<Player*>(entity);


	ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(size, ImGuiCond_Always);


	if (!ImGui::Begin(id.c_str(), &visible,
	                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-1);
	
	AttributesComponent* attributes = (AttributesComponent*) entity->getComponent(ComponentType::ATTRIBUTES);

	ImFont* font = g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 14);
	ImGui::PushFont(font);

	string type;
	if (npc != nullptr) {
		type = "N";
	}
	if(player != nullptr) {
		type = "P";
	}

	float level = attributes->getAttribute(EntityAttributeType::LEVEL);
	string head = convertFloatToString(level, 0) + " " + entity->getName() + "("+ type +")";

	ImGui::Text(head.c_str());

	float hp = attributes->getAttribute(EntityAttributeType::HP);
	float maxHp = attributes->getAttribute(EntityAttributeType::BASE_HP);
	float mp = attributes->getAttribute(EntityAttributeType::MP);
	float maxMp = attributes->getAttribute(EntityAttributeType::BASE_MP);

	float hpPerc = 0;
	if (maxHp > 0) {
		hpPerc = hp / maxHp;
	} 

	float mpPerc = 0;
	if(maxMp > 0) {
		mpPerc = mp / maxMp;
	}

	string text = "HP: " + convertFloatToString(hp, 0) + "/" + convertFloatToString(maxHp, 0) + " ("+ convertFloatToString(hpPerc * 100, 0)+"%)";
	ImGui::ProgressBar(hpPerc, ImVec2(-1,0), text.c_str());

	if (maxMp > 0) {
		text = "MP: " + convertFloatToString(mp, 0) + "/" + convertFloatToString(maxMp, 0) + " ("+ convertFloatToString(mpPerc * 100, 0)+"%)";
		ImGui::ProgressBar(mpPerc, ImVec2(-1,0), text.c_str());
	}
	ImGui::PopFont();

	ImGui::PopItemWidth();

	focused = ImGui::IsWindowFocused();

	ImGui::End(); // end window
}

string IGEntityInfo::convertFloatToString(float number, int precision) const {
	stringstream stream;
	stream << fixed << setprecision(precision) << number;
	return stream.str();
}
