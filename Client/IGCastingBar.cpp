#include "IGCastingBar.h"
#include "Spell.h"
#include "Player.h"
#include "Game.h"

IGCastingBar::IGCastingBar(): player(nullptr) {
	size = sf::Vector2f(200.f, 38.f);
}


IGCastingBar::~IGCastingBar()
{
}

void IGCastingBar::render(Game* g, IGManager* manager) {
	if(!player) return;

	SpellInfo* spell = player->getCastingSpell();
	if (!spell)
		return;

	sf::Time startCastingTime = player->getStartCastingTime();
	sf::Time actualTime = g->getGameTime();

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);


	if (!ImGui::Begin("castingBar", &visible,
	                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushFont(g->fonts.getFont(ImGuiFonts::PRODIGY_TINY, 15));


	ImGui::PushItemWidth(-1);
	
	sf::Time a = actualTime - startCastingTime;
	sf::Time b = spell->castingTime;

	if (a.asMicroseconds() < 0) {
		a = sf::Time::Zero;
	}

	float perc = a.asSeconds() / b.asSeconds();
	if (perc > 1) {
		perc = 1;
	}
	std::string text = convertFloatToString(a.asSeconds(), 2) + "/" + convertFloatToString(b.asSeconds(), 2) + " ("+ convertFloatToString(perc * 100, 0)+"%)";
	
	ImGui::ProgressBar(perc, ImVec2(-1,0), text.c_str());


	ImGui::PopItemWidth();
	ImGui::PopFont();

	focused = false;

	ImGui::End(); // end window
}

void IGCastingBar::beforeRender(Game* game) {
	IGWindow::beforeRender(game);

	sf::Vector2u windowSize = game->window->getSize();

	float left = windowSize.x / 2.f - size.x / 2.f;
	float top = windowSize.y * 0.7f - size.y / 2.f;
	setPosition(sf::Vector2f(left, top));
}
