#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include "EventId.h"
#include "SFML\Network.hpp"

class EventVisitor;

class GameEvent
{
protected:
	EventId id;
public:
	GameEvent();

	EventId getId();
	virtual void accept(EventVisitor*) = 0;
	virtual bool loadFromPacket(sf::Packet* p) = 0;
	virtual sf::Packet* toPacket() = 0;
};

#endif