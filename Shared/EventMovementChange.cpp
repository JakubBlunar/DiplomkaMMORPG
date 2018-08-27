#include "stdafx.h"
#include "EventMovementChange.h"


EventMovementChange::EventMovementChange() {
	id = EventId::MOVEMENT;
}

void EventMovementChange::accept(EventVisitor* v) {
	if (v)
		v->visit(this);
}

bool EventMovementChange::loadFromPacket(sf::Packet* p) {
	if (*p >> playerId >> x >> y >> velX >> velY >> speed) {
		return true;
	}
	return false;
}

sf::Packet* EventMovementChange::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << playerId << x << y << velX << velY << speed;
	return p;
}
