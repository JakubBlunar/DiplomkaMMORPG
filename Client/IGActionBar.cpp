#include "IGActionBar.h"
#include "Game.h"
#include "imgui-SFML.h"
#include <iostream>
#include "ImGuiFonts.h"


IGActionBar::IGActionBar() {
	
}


IGActionBar::~IGActionBar() {}

void IGActionBar::render(Game* g, IGManager* manager) {
	if (!player) return;

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);

	if (!ImGui::Begin("ActionBar", &visible,
	                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-1);


	int spellCount = spells->size();
	for (int i = 0; i < spellCount; i++) {
		SpellInfo* si = spells->at(i);
		ImGui::SameLine();
		if (ImGui::ImageButton(si->icon)) {
			player->castSpell(si);
		}
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(200);
			
			ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 11));
			ImGui::Text(si->name.c_str());

			if (si->manaCost > 0) {
				std::string manaCost = "Mana: " + convertFloatToString(si->manaCost, 0);
				ImGui::Text(manaCost.c_str());
			}

			if (si->cooldownTime != sf::Time::Zero) {
				std::string cooldown = "Cooldown: " + convertFloatToString(si->cooldownTime.asMilliseconds(), 0) +
					" ms";
				ImGui::Text(cooldown.c_str());
			}
			if (si->castingTime != sf::Time::Zero) {
				std::string castingTime = "CastingTime: " + convertFloatToString(si->castingTime.asMilliseconds(), 0) +
					" ms";
				ImGui::Text(castingTime.c_str());
			}
			ImGui::PopFont();
			ImGui::PopTextWrapPos();

			ImGui::EndTooltip();
		}
	}

	ImGui::PopItemWidth();

	focused = ImGui::IsWindowFocused();
	ImGui::SetWindowFontScale(1);

	ImGui::End(); // end window

}

void IGActionBar::beforeRender(Game* game) {
	IGWindow::beforeRender(game);

	if (player) {
		sf::Vector2f size(32 * 15, 0);
		int spellCount = spells->size();
		int rows = spellCount / 10 + 1;
		size.y = rows * 54;
		size.x = 10 * 50;

		if (spellCount < 10) {
			size.x = spellCount * 55.f + 10;
		}
		setSize(size);
	}

	Camera* c = game->getCamera();

	float left = c->getResolution().x / 2 - size.x / 2;
	float top = c->getResolution().y - size.y;
	setPosition(sf::Vector2f(left, top));


}
