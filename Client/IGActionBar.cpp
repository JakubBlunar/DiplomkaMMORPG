#include "IGActionBar.h"
#include "Game.h"
#include "imgui-SFML.h"
#include <iostream>
#include "ImGuiFonts.h"
#include "Spell.h"

#define MAX_OPACITY_ON_COOLDOWN 100.f

IGActionBar::IGActionBar(): player(nullptr), spells(nullptr) { }

IGActionBar::~IGActionBar() {}

void IGActionBar::render(Game* g, IGManager* manager) {
	if (!player) return;

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);

	if (!ImGui::Begin("ActionBar", &visible,
	                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-1);


	int spellCount = spells->size();
	for (int i = 0; i < spellCount; i++) {
		SpellInfo* si = spells->at(i);
		ImGui::SameLine();

		int cooldownOpacity = 200;
		SpellCooldown* globalCooldown = player->getGlobalCooldown(g);
		if (globalCooldown) {
			float length = globalCooldown->cooldownTo.asSeconds() - globalCooldown->cooldownFrom.asSeconds();
			float part = g->getGameTime().asSeconds() - globalCooldown->cooldownFrom.asSeconds();

			float perc = part / length;
			cooldownOpacity = (int)ceil(perc * MAX_OPACITY_ON_COOLDOWN);
			if (cooldownOpacity < 0) {
				cooldownOpacity = 0;
			}
			if (cooldownOpacity > MAX_OPACITY_ON_COOLDOWN) {
				cooldownOpacity = (int)MAX_OPACITY_ON_COOLDOWN;
			}
		}

		SpellCooldown* spellCooldown = player->getCooldown(si->id, g);
		if (spellCooldown) {
			float length = spellCooldown->cooldownTo.asSeconds() - spellCooldown->cooldownFrom.asSeconds();
			float part = g->getGameTime().asSeconds() - spellCooldown->cooldownFrom.asSeconds();

			float perc = part / length;
			cooldownOpacity = (int)ceil(perc * MAX_OPACITY_ON_COOLDOWN);
			if (cooldownOpacity < 0) {
				cooldownOpacity = 0;
			}
			if (cooldownOpacity > MAX_OPACITY_ON_COOLDOWN) {
				cooldownOpacity = (int)MAX_OPACITY_ON_COOLDOWN;
			}
		}

		if (ImGui::ImageButton(si->icon, -1, sf::Color::Black, sf::Color(255,255,255,55 + cooldownOpacity))) {
			player->castSpell(si, g->getMap(), g);
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
				std::string cooldown = "Cooldown: " + convertFloatToString((float)si->cooldownTime.asMilliseconds(), 0) +
					" ms";
				ImGui::Text(cooldown.c_str());
			}
			if (si->castingTime != sf::Time::Zero) {
				std::string castingTime = "CastingTime: " + convertFloatToString((float)si->castingTime.asMilliseconds(), 0) +
					" ms";
				ImGui::Text(castingTime.c_str());
			}
			ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 9));
			ImGui::Text(si->description.c_str());


			ImGui::PopFont();
			ImGui::PopFont();
			ImGui::PopTextWrapPos();

			ImGui::EndTooltip();
		}
	}

	ImGui::PopItemWidth();

	focused = false;
	ImGui::SetWindowFontScale(1);

	ImGui::End(); // end window

}

void IGActionBar::beforeRender(Game* game) {
	IGWindow::beforeRender(game);

	if (player) {
		sf::Vector2f size(32 * 15.f, 0);
		int spellCount = spells->size();
		int rows = spellCount / 10 + 1;
		size.y = rows * 54.f;
		size.x = 10.f * 50.f;

		if (spellCount < 10.f) {
			size.x = spellCount * 55.f + 10.f;
		}
		setSize(size);
	}
	
	sf::Vector2u windowSize = game->window->getSize();

	float left = windowSize.x / 2 - size.x / 2;
	float top = windowSize.y - size.y;
	setPosition(sf::Vector2f(left, top));


}
