#include "stdafx.h"
#include "EventNpcMovementChange.h"


EventNpcMovementChange::EventNpcMovementChange()
{
	id = NPC_MOVEMENT_CHANGE;
}


bool EventNpcMovementChange::loadFromPacket(sf::Packet* p) {
	if (*p >> spawnId >> x >> y >> velX >> velY >> speed) {
		return true;
	}
	return false;
}
sf::Packet* EventNpcMovementChange::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << spawnId << x << y << velX << velY << speed;
	return p;
}

std::string EventNpcMovementChange::toString() const {
	return "Npc movement change" + std::to_string(spawnId);
}

