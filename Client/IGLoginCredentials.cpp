#include "IGLoginCredentials.h"
#include "Game.h"
#include "EventDispatcher.h"
#include "IGManager.h"
#include "EventLoginRequest.h"

IGLoginCredentials::IGLoginCredentials() {
	memset(buffLogin, 0, sizeof(buffLogin));
	memset(buffPassword, 0, sizeof(buffPassword));
	setSize(sf::Vector2f(450, 250));
}


IGLoginCredentials::~IGLoginCredentials() {

}

void IGLoginCredentials::render(Game* game, IGManager* manager) {
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(100, 100, 0, 0));

	ImGui::SetNextWindowSize(size, ImGuiCond_Always);
	ImGui::SetNextWindowPos(position);

	if (!ImGui::Begin("Login", &visible,
	                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove)) {
		focused = false;
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-1);

	ImGui::SetWindowFontScale(0.3f);
	ImGui::Text("Log into your account");


	ImGui::SetWindowFontScale(0.2f);
	ImGui::TextColored(ImVec4(0, 0, 0, 1), "Account Name:");
	ImGui::InputText("##Account name", buffLogin, sizeof(buffLogin));

	ImGui::TextColored(ImVec4(0, 0, 0, 1), "Password:");
	ImGui::InputText("##Password", buffPassword, sizeof(buffPassword), ImGuiInputTextFlags_Password);

	if (ImGui::Button("Login")) {
		if (!manager->isShowingPopup()) {
			std::string name = std::string(buffLogin);
			std::string password = std::string(buffPassword);
			memset(buffPassword, 0, sizeof(buffPassword));

			if (!name.empty() && !password.empty()) {
				IGPopup* popup = new IGPopup("", "Logging in...", sf::Vector2f(250, 50), "");
				manager->pushPopup(popup);
				EventLoginRequest* req = new EventLoginRequest(name, password);
				sf::Packet* packet = req->toPacket();
				game->packet_manager->sendPacket(packet);
				delete packet;
			}
		}
	}

	ImGui::PopItemWidth();

	focused = ImGui::IsWindowFocused();

	ImGui::End(); // end window

	ImGui::PopStyleColor();
}

void IGLoginCredentials::beforeRender(Game* game) {
	IGWindow::beforeRender(game);

	sf::Vector2u windowSize = game->window->getSize();

	float left = windowSize.x / 2 - size.x / 2;
	float top = windowSize.y / 2 - size.y / 2;
	setPosition(sf::Vector2f(left, top));
}
