#ifndef EVENT_AUTOATTACK_PLAYER
#define EVENT_AUTOATTACK_PLAYER

#include "GameEvent.h"
class EventAutoattackPlayer :
	public GameEvent
{
public:
	EventAutoattackPlayer();
	~EventAutoattackPlayer();

	int playerId;
	int npcId;

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif
