#include "CharacterChooseScene.h"
#include "IGAvailableCharacters.h"
#include "SceneManager.h"
#include "EventCharacterChooseResponse.h"
#include "EventDispatcher.h"

CharacterChooseScene::CharacterChooseScene(SceneType sceneType, Game* g) : Scene(sceneType, g) {
	IGAvailableCharacters* selectCharactersWindow = new IGAvailableCharacters();
	windowManager->addWindow("selectCharacter", selectCharactersWindow);
	windowManager->openAll();

	background.load("bg2.png", sf::Vector2i(1360, 768), sf::Vector2i(0,0));
	background.setPosition(0,0);

	EventDispatcher<EventCharacterChooseResponse>::addSubscriber(this);
}


void CharacterChooseScene::beforeChange() {
	Scene::beforeChange();
}

void CharacterChooseScene::afterChange() {
	Scene::afterChange();
}

void CharacterChooseScene::update(sf::Time elapsedTime) {
	Scene::update(elapsedTime);
}

void CharacterChooseScene::render() {
	game->window->draw(background);
	Scene::render();
}

void CharacterChooseScene::handleEvent(GameEvent* event) {
	switch(event->getId()) {
		case CHARACTER_CHOOSE_RESPONSE: {
			EventCharacterChooseResponse* e = (EventCharacterChooseResponse*) event;

			if (e->success) {
				json response = json::parse(e->characterData);
				//game->print("Character DATA: " + response.dump());
				json characterData = response["character"].get<json::object_t>();


				Player* p = new Player(true);
				p->loadFromJson(characterData);

				int mapId = (int)characterData["mapId"].get<json::number_integer_t>();

				Map* map = new Map(game);
				map->loadFromFile(mapId);

				game->getAccount()->setPlayerEntity(p);
				map->addPlayer(p);
				p->setMap(map);

				json otherPlayers = response["otherPlayers"].get<json::array_t>();
				for (json::iterator it = otherPlayers.begin(); it != otherPlayers.end(); ++it) {
					Player* otherPlayer = new Player(false);
					otherPlayer->loadFromJson(*it);

					if (otherPlayer->getId() != p->getId())
						map->addPlayer(otherPlayer);
				}

				json npcs = response["npcs"].get<json::array_t>();
				for (json::iterator it = npcs.begin(); it != npcs.end(); ++it) {
					Npc* npc = new Npc();
					npc->loadFromJson(*it);
					map->addNpc(npc);
				}

				game->changeMap(map);
				game->sceneManager->changeScene(SceneType::GAMEPLAY);
			}
			else {
				IGPopup* popup = new IGPopup("Error", e->message, sf::Vector2f(550, 180), "Ok");
				windowManager->pushPopup(popup);
			}
		}
	}
}
