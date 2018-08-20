#include "ClientEventActions.h"

#include "Game.h"
#include "SFML\Network.hpp"
#include "EventMovementChange.h"
#include "PacketManager.h"
#include "EventLoginResponse.h"
#include "SceneManager.h"
#include <iostream>

ClientEventActions::ClientEventActions(Game * g)
{
	this->game = g;
}

ClientEventActions::~ClientEventActions()
{
}

void ClientEventActions::visit(EventMovementChange * e)
{
	sf::Packet* packet = e->toPacket();
	game->packet_manager->sendPacket(packet);
	delete packet;
}

void ClientEventActions::visit(EventLoginRequest* e)
{
	sf::Packet* packet = e->toPacket();
	game->packet_manager->sendPacket(packet);
	delete packet;
}

void ClientEventActions::visit(EventLoginResponse* e)
{
	game->print(e->toString());
	if (e->status)
	{
		json jsonData = json::parse(e->account);
		Account* account = new Account();
		account->initFromJson(jsonData);
		game->setAccount(account);
		game->sceneManager->changeScene(SceneType::CHARACTER_CHOOSE);
	}else
	{
		IGManager* manager = game->sceneManager->getActualScene()->getWindowManager();
		if(manager->getActualPopup())
		{
			manager->getActualPopup()->close();
		}
		IGPopup* popup = new IGPopup("Error", e->message, sf::Vector2f(550, 180), "Ok");
		manager->pushPopup(popup);
	}
	
}

void ClientEventActions::visit(EventCharacterChooseResponse* e)
{
	game->print(e->toString());
	if(e->success)
	{
		json characterData = json::parse(e->characterData);
		game->print("Character DATA: " + characterData.dump());
		Player* p = new Player(true);
		p->loadFromJson(characterData);

		Map* map = new Map(game);

		game->getAccount()->setPlayerEntity(p);
		map->addPlayer(p);
		game->changeMap(map);

		game->sceneManager->changeScene(SceneType::GAMEPLAY);
	}else
	{
		IGManager* manager = game->sceneManager->getActualScene()->getWindowManager();
		IGPopup* popup = new IGPopup("Error", e->message, sf::Vector2f(550, 180), "Ok");
		manager->pushPopup(popup);
	}
}
