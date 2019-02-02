#ifndef EVENT_NPC_MOVEMENT_CHANGE
#define EVENT_NPC_MOVEMENT_CHANGE

#include "GameEvent.h"

class EventNpcMovementChange :
	public GameEvent {
public:
	EventNpcMovementChange();

	float x, y, velX, velY, speed;
	int spawnId;

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;
};

#endif // !EVENT_NPC_MOVEMENT_CHANGE
