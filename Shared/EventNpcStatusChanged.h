#ifndef NPC_STATUS_CHANGED_H
#define NPC_STATUS_CHANGED_H

#include "GameEvent.h"
#include "NpcConstants.h"

class EventNpcStatusChanged :
	public GameEvent
{
public:
	int spawnId;
	NpcState npcState;

	EventNpcStatusChanged();
	~EventNpcStatusChanged();
	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif