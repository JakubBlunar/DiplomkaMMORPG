#include "stdafx.h"
#include "EventMovementChange.h"


EventMovementChange::EventMovementChange() {
	id = EventId::MOVEMENT;
}

bool EventMovementChange::loadFromPacket(sf::Packet* p) {
	if (*p >> playerId >> x >> y >> velX >> velY) {
		return true;
	}
	return false;
}

sf::Packet* EventMovementChange::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << playerId << x << y << velX << velY;
	return p;
}
