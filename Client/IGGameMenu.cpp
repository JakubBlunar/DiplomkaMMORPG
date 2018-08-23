#include "IGGameMenu.h"
#include "Game.h"


IGGameMenu::IGGameMenu() {
	size = sf::Vector2f(250, 260);
	centeredPosition = true;
}


IGGameMenu::~IGGameMenu() {
}

void IGGameMenu::render(Game* g, IGManager* manager) {

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);
	ImGui::SetNextWindowPosCenter(centeredPosition);

	if (!ImGui::Begin("GameMenu", &visible,
	                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-1);

	ImGui::SetWindowFontScale(0.3f);
	ImGui::Text("Game Menu");

	if (ImGui::Button("Settings", sf::Vector2f(ImGui::GetWindowWidth() * 0.935f, 40))) {
		close();
	}

	if (ImGui::Button("Logout", sf::Vector2f(ImGui::GetWindowWidth() * 0.935f, 40))) {
		close();
	}

	if (ImGui::Button("Exit", sf::Vector2f(ImGui::GetWindowWidth() * 0.935f, 40))) {
		ImGui::OpenPopup("ExitPrompt");
		ImGui::SetNextWindowPos(position);
		ImGui::SetNextWindowPosCenter(centeredPosition);
	}

	bool open = true;
	if (ImGui::BeginPopupModal("ExitPrompt", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowFontScale(0.2f);
		ImGui::Text("Do you realy wish to end game?");
		ImGui::NewLine();

		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		ImGui::SameLine();
		if (ImGui::Button("Exit")) {
			g->running = false;
		}
		ImGui::EndPopup();
	}

	ImGui::NewLine();
	if (ImGui::Button("Return", sf::Vector2f(ImGui::GetWindowWidth() * 0.935f, 40))) {
		this->close();
	}

	ImGui::PopItemWidth();

	focused = ImGui::IsWindowFocused();
	ImGui::SetWindowFontScale(1);

	ImGui::End(); // end window
}

void IGGameMenu::beforeRender(Game* game) {
	IGWindow::beforeRender(game);

	float left = game->window.getView().getViewport().left + game->window.getView().getViewport().width / 2.0f;
	float top = game->window.getView().getViewport().top + game->window.getView().getViewport().height / 2.0f;
	setPosition(sf::Vector2f(left, top), true);

}
