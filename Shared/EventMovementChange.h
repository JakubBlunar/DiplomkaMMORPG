#ifndef EVENT_MOVEMENT_CHANGED
#define EVENT_MOVEMENT_CHANGED

#include "GameEvent.h"

class EventMovementChange :
	public GameEvent {
public:
	EventMovementChange();

	float x, y, velX, velY;
	int playerId;

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif // !EVENT_MOVEMENT_CHANGED
