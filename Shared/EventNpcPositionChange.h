#ifndef EVENT_NPC_POSITION_CHANGE
#define EVENT_NPC_POSITION_CHANGE

#include "GameEvent.h"
class EventNpcPositionChange :
	public GameEvent
{
public:
	EventNpcPositionChange();
	~EventNpcPositionChange();

	float positionX;
	float positionY;
	int npcId;
	float velX;
	float velY;

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;

};

#endif
