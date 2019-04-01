#include "stdafx.h"
#include "EventAutoattackPlayer.h"


EventAutoattackPlayer::EventAutoattackPlayer()
{
	id = AUTOATTACK_PLAYER;
}


EventAutoattackPlayer::~EventAutoattackPlayer()
{
}

bool EventAutoattackPlayer::loadFromPacket(sf::Packet * p)
{
	if (*p >> playerId >> npcId) {
		return true;
	}
	return false;
}

sf::Packet * EventAutoattackPlayer::toPacket()
{
	sf::Packet* p = new sf::Packet();
	if (*p << id << playerId << npcId) {
		return p;
	}
	throw "cannot convert into packet";
}
