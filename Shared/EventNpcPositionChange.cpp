#include "stdafx.h"
#include "EventNpcPositionChange.h"


EventNpcPositionChange::EventNpcPositionChange()
{
	id = NPC_POSITION_CHANGE;
}


EventNpcPositionChange::~EventNpcPositionChange()
{
}

bool EventNpcPositionChange::loadFromPacket(sf::Packet * p)
{
	if (*p >> npcId >> positionX >> positionY >> velX >> velY) {
		return true;
	}
	return false;
}

sf::Packet * EventNpcPositionChange::toPacket()
{
	sf::Packet* p = new sf::Packet();
	if (*p << id << npcId << positionX << positionY << velX << velY) {
		return p;
	}
	throw "Cannot convert to packet";
}
