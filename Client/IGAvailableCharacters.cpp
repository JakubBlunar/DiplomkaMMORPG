#include "IGAvailableCharacters.h"
#include "JsonLoader.h"
#include "Game.h"
#include "imgui-SFML.h"
#include <iostream>

IGAvailableCharacters::IGAvailableCharacters() {
}


IGAvailableCharacters::~IGAvailableCharacters() {
}

void IGAvailableCharacters::sendCharacterChoice(int id, Game* g) const {
	std::cout << "Choosing " << id << std::endl;
	EventCharacterChoose* e = new EventCharacterChoose();
	e->characterId = id;
	sf::Packet* packet = e->toPacket();
	g->packet_manager->sendPacket(packet);
	delete packet;
}

void IGAvailableCharacters::render(Game* g, IGManager* manager) {

	json accountData = g->getAccount()->getJsonData();
	json characters = accountData["characters"].get<json::array_t>();

	/*for (json::iterator it = characters.begin(); it != characters.end(); ++it) {
		std::cout << *it << '\n';
		break;
	}*/

	sf::Vector2u renderWindowSize = g->window->getSize();

	sf::Vector2f windowSize = sf::Vector2f(renderWindowSize.x * 0.94f, renderWindowSize.y * 0.8f);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(sf::Vector2f(renderWindowSize.x * 0.03f, 30));

	RenderSprite* sprite = new RenderSprite();
	sprite->load("Dungeon_A2.png", sf::Vector2i(32, 64), sf::Vector2i(0, 0));

	const sf::Texture* texture = sprite->getTexture();

	bool open = false;
	if (ImGui::Begin("chooseCharacter", &visible,
	                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
	                 ImGuiWindowFlags_NoScrollbar)) {
		ImGui::SetWindowFontScale(0.5);
		ImGui::Text("Choose your character");

		ImGui::SetWindowFontScale(0.25);
		ImGui::SameLine(ImGui::GetWindowWidth() - 320);
		ImGui::Button("Create New Character");

		ImGui::NewLine();

		if (ImGui::BeginChild("chooseCharacterWrapper")) {

			int columns = 4;

			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(100, 100, 0, 1));

			int i = 0;
			for (json::iterator it = characters.begin(); it != characters.end(); ++it) {
				std::string childName = "ch" + std::to_string(i);
				json data = *it;

				std::string characterName = data["name"].get<std::string>();
				int id = data["id"].get<int>();

				if (ImGui::BeginChild(childName.c_str(), sf::Vector2f(windowSize.x * 0.23f, 100), false,
				                      ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar |
				                      ImGuiWindowFlags_AlwaysAutoResize)) {
					ImGui::SetWindowFontScale(0.1f);
					ImGui::NewLine();
					ImGui::Image(*sprite);
					ImGui::SameLine();
					ImGui::BeginGroup();
					ImGui::SetWindowFontScale(0.25f);
					ImGui::TextWrapped(characterName.c_str());

					ImGui::SetWindowFontScale(0.20f);
					ImGui::TextWrapped("Level: 3");
					if (ImGui::Button("Play")) {
						sendCharacterChoice(id, g);
					}
					ImGui::EndGroup();
				}
				ImGui::EndChild();

				ImGui::SetWindowFontScale(0.1f);

				if (i > 0 && i % columns == columns - 1) {
					ImGui::NewLine();
				}
				else {
					ImGui::SameLine();
				}
				i++;
			}

			ImGui::PopStyleColor();
		}
		ImGui::EndChild();
	}

	ImGui::End();
}
