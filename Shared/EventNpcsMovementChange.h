#ifndef NPCS_MOVEMENT_CHANGE_H
#define NPCS_MOVEMENT_CHANGE_H

#include "GameEvent.h"
#include "../Shared/EntityConstants.h"

class EventNpcsMovementChange :
	public GameEvent
{
public:
	EventNpcsMovementChange();
	~EventNpcsMovementChange();

	void addNpcInfo(sf::Int32 npcId, float posX, float posY, float velX, float velY);

	std::map<sf::Int32, MovementData> npcsMovements;

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif