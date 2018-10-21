#include "IGEntityInfo.h"
#include "Entity.h"
#include "Npc.h"
#include "Player.h"


IGEntityInfo::IGEntityInfo(string id, sf::Vector2f position)
{
	this->id = id;
	entity = nullptr;
	setPosition(position, false);
	setSize(sf::Vector2f(150, 75));
}


IGEntityInfo::~IGEntityInfo()
{
}

void IGEntityInfo::setEntity(Entity* entity) {
	this->entity = entity;
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

	ImGui::SetWindowFontScale(0.13f);
	if (npc != nullptr) {
		ImGui::Text("Npc");
	}
	if(player != nullptr) {
		ImGui::Text("Player");
	}
	ImGui::Text(entity->getName().c_str());


	ImGui::PopItemWidth();

	focused = ImGui::IsWindowFocused();

	ImGui::End(); // end window
}
