#include "IGGameMenu.h"
#include "Game.h"
#include "EventCharacterLogout.h"
#include "SceneManager.h"

IGGameMenu::IGGameMenu() {
	size = sf::Vector2f(250, 260);
}


IGGameMenu::~IGGameMenu() {}

void IGGameMenu::render(Game* g, IGManager* manager) {

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);

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
		ImGui::OpenPopup("LogoutCharacterPrompt");
		ImGui::SetNextWindowPos(position);
	}

	if (ImGui::Button("Exit", sf::Vector2f(ImGui::GetWindowWidth() * 0.935f, 40))) {
		ImGui::OpenPopup("ExitPrompt");
		ImGui::SetNextWindowPos(position);
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

	if (ImGui::BeginPopupModal("LogoutCharacterPrompt", &open,
	                           ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowFontScale(0.2f);
		ImGui::Text("Do you realy wish to logout?");
		ImGui::NewLine();

		ImGui::SameLine();
		if (ImGui::Button("Yes")) {
			ImGui::CloseCurrentPopup();

			EventCharacterLogout e;
			e.characterId = g->getAccount()->getPlayerEntity()->getId();

			sf::Packet* p = e.toPacket();
			g->packet_manager->sendPacket(p);
			delete p;

			g->sceneManager->changeScene(SceneType::CHARACTER_CHOOSE);

			delete g->getAccount()->getPlayerEntity();
			delete g->getMap();
		}

		ImGui::SameLine();
		if (ImGui::Button("No")) {
			ImGui::CloseCurrentPopup();
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

	Camera* c = game->getCamera();

	float left = c->getResolution().x / 2 - size.x / 2;
	float top = c->getResolution().y / 2 - size.y / 2;
	setPosition(sf::Vector2f(left, top));
}
