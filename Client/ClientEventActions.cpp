#include "ClientEventActions.h"

#include "Game.h"
#include "SFML\Network.hpp"
#include "EventMovementChange.h"
#include "PacketManager.h"
#include "EventLoginResponse.h"
#include "SceneManager.h"
#include <iostream>
#include "BotGame.h"

ClientEventActions::ClientEventActions(Game* g) {
	this->game = g;
}

ClientEventActions::~ClientEventActions() {
}

void ClientEventActions::visit(EventLoginRequest* e) {
	sf::Packet* packet = e->toPacket();
	game->packet_manager->sendPacket(packet);
	delete packet;
}

void ClientEventActions::visit(EventLoginResponse* e) {
	//game->print(e->toString());
	if (e->status) {
		json jsonData = json::parse(e->account);
		Account* account = new Account();
		account->initFromJson(jsonData);
		game->setAccount(account);

		game->sceneManager->changeScene(SceneType::CHARACTER_CHOOSE);

		if (dynamic_cast<BotGame*>(game) != nullptr) {
			EventCharacterChoose* eventCharacterChoose = new EventCharacterChoose();
			eventCharacterChoose->characterId = 1;
			sf::Packet* packet = eventCharacterChoose->toPacket();
			game->packet_manager->sendPacket(packet);
			delete packet;
		}
	}
	else {
		IGManager* manager = game->sceneManager->getActualScene()->getWindowManager();
		if (manager->getActualPopup()) {
			manager->getActualPopup()->close();
		}
		IGPopup* popup = new IGPopup("Error", e->message, sf::Vector2f(550, 180), "Ok");
		manager->pushPopup(popup);
	}

}

void ClientEventActions::visit(EventCharacterChooseResponse* e) {
	//game->print(e->toString());
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

		json otherPlayers = response["otherPlayers"].get<json::array_t>();
		for (json::iterator it = otherPlayers.begin(); it != otherPlayers.end(); ++it) {
			Player* otherPlayer = new Player(false);
			otherPlayer->loadFromJson(*it);

			if (otherPlayer->getId() != p->getId())
				map->addPlayer(otherPlayer);
		}
		game->changeMap(map);

		game->sceneManager->changeScene(SceneType::GAMEPLAY);
	}
	else {
		IGManager* manager = game->sceneManager->getActualScene()->getWindowManager();
		IGPopup* popup = new IGPopup("Error", e->message, sf::Vector2f(550, 180), "Ok");
		manager->pushPopup(popup);
	}
}
