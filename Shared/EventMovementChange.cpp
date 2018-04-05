#include "stdafx.h"
#include "EventMovementChange.h"


EventMovementChange::EventMovementChange()
{
	id = EventId::MOVEMENT;
}

void EventMovementChange::accept(EventVisitor* v)
{
	v->visit(this);
}

bool EventMovementChange::loadFromPacket(sf::Packet* p)
{
	if (*p >> x >> y >> velX >> velY)
	{
		return true;
	}
	return false;
}

sf::Packet* EventMovementChange::toPacket()
{
	sf::Packet* p = new sf::Packet();
	*p << id << x << y << velX << velY;
	return p;
}
