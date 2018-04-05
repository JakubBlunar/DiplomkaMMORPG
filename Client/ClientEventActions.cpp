#include "ClientEventActions.h"

#include "Game.h"
#include "SFML\Network.hpp"
#include "EventMovementChange.h"
#include "PacketManager.h"
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
