#ifndef EVENT_MOVEMENT_CHANGED
#define EVENT_MOVEMENT_CHANGED

#include "GameEvent.h"

class EventMovementChange :
	public GameEvent
{
public:
	EventMovementChange();

	float x, y, velX, velY;

	void accept(class EventVisitor* v);
	bool loadFromPacket(sf::Packet* p);
	sf::Packet* toPacket();
};

#endif // !EVENT_MOVEMENT_CHANGED
