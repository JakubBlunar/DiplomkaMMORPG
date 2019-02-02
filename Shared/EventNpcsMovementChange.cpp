#include "stdafx.h"
#include "EventNpcsMovementChange.h"
#include <iostream>


EventNpcsMovementChange::EventNpcsMovementChange()
{
	id = NPCS_MOVEMENT_CHANGE;
}


EventNpcsMovementChange::~EventNpcsMovementChange()
{
	npcsMovements.clear();
}

void EventNpcsMovementChange::addNpcInfo(sf::Int32 npcId, float posX, float posY, float velX, float velY)
{
	sf::Lock lock(mutex);
	npcsMovements.insert(std::make_pair(npcId, MovementData{ posX, posY, velX, velY }));
}

bool EventNpcsMovementChange::loadFromPacket(sf::Packet* p) {
	int count;
	if (*p >> count) {
		for (int i = 0; i < count; i++) {
			sf::Int32 npcId;
			float x, y, velocityX, velocityY;
			if (*p >> npcId >> x >> y >> velocityX >> velocityY) {
				addNpcInfo(npcId, x, y, velocityX, velocityY);
			} else {
				npcsMovements.clear();
				return false;
			}
		}
		return true;
	}
	return false;
}

sf::Packet * EventNpcsMovementChange::toPacket()
{
	sf::Packet* p = new sf::Packet();

	int count = (int)npcsMovements.size();
	*p << id << count;

	for (std::map<int, MovementData>::iterator it = npcsMovements.begin(); it != npcsMovements.end(); ++it)
	{
	    *p << it->first;
		*p << it->second.x;
		*p << it->second.y;
		*p << it->second.velocityX;
		*p << it->second.velocityY;
	}

	return p;
}

