#include "IGLearnNewSpell.h"
#include "Game.h"
#include "imgui-SFML.h"
#include "ImGuiFonts.h"
#include "EventLearnSpell.h"

IGLearnNewSpell::IGLearnNewSpell(): spells(nullptr) {
	chosenSpell = -1;
}


IGLearnNewSpell::~IGLearnNewSpell() {}

void IGLearnNewSpell::render(Game* g, IGManager* manager) {

	if (!spells) return;

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position, ImGuiCond_Always);

	if (!ImGui::Begin("LearnNewSpells", &visible,
	                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
	                  ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-1);
	ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 13));
	ImGui::TextWrapped("You can learn one new spell from spell bellow. Choose wisely.");
	ImGui::NewLine();

	int spellCount = spells->size();
	for (int i = 0; i < spellCount; i++) {
		SpellInfo* si = spells->at(i);
		ImGui::SameLine();

		sf::Color color = sf::Color::White;
		if(chosenSpell == i) {
			color = sf::Color::Red;
		}
		if (ImGui::ImageButton(si->icon, -1, color,color)) {
			if (chosenSpell == i) {
				chosenSpell = -1;
			}
			else {
				chosenSpell = i;
			}
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
				std::string cooldown = "Cooldown: " + convertFloatToString((float)si->cooldownTime.asMilliseconds(), 0)
					+
					" ms";
				ImGui::Text(cooldown.c_str());
			}
			if (si->castingTime != sf::Time::Zero) {
				std::string castingTime = "CastingTime: " + convertFloatToString(
						(float)si->castingTime.asMilliseconds(), 0) +
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

	if (chosenSpell != -1) {
		ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 13));
		if (ImGui::Button("Learn##learnSpell")) {
			EventLearnSpell e;
			e.spellId = spells->at(chosenSpell)->id;

			sf::Packet* p = e.toPacket();
			g->packet_manager->sendPacket(p);

			close();
			delete p;
		}
		ImGui::PopFont();
	}

	ImGui::PopFont();
	ImGui::PopItemWidth();
	focused = false;
	ImGui::SetWindowFontScale(1);

	ImGui::End(); // end window

}

void IGLearnNewSpell::beforeRender(Game* game) {
	IGWindow::beforeRender(game);

	if (spells && !spells->empty()) {
		sf::Vector2f size(75.f * spells->size(), 130.f);
		if (size.x < 250) {
			size.x = 250;
		} 
		setSize(size);
	}

	sf::Vector2u windowSize = game->window->getSize();
	float left = windowSize.x / 2 - size.x / 2;
	float top = windowSize.y / 2 - size.y / 2;
	setPosition(sf::Vector2f(left, top));
}
