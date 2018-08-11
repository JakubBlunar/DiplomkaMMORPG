#include "ClientEventActions.h"

#include "Game.h"
#include "SFML\Network.hpp"
#include "EventMovementChange.h"
#include "PacketManager.h"
#include "EventLoginResponse.h"
#include "SceneManager.h"

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
		game->sceneManager->changeScene(SceneType::GAMEPLAY);
	}else
	{
		//show popup
	}
	
}
