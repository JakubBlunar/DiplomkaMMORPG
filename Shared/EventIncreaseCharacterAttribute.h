#ifndef EVENT_INCREASE_CHARACTER_ATTRIBUTE_H
#define EVENT_INCREASE_CHARACTER_ATTRIBUTE_H

#include "GameEvent.h"
#include "../Shared/EntityConstants.h"

class EventIncreaseCharacterAttribute :
	public GameEvent
{
public:
	EventIncreaseCharacterAttribute();
	EventIncreaseCharacterAttribute(int playerId, EntityAttributeType attribute);
	virtual ~EventIncreaseCharacterAttribute();

	int playerId;
	EntityAttributeType attribute;

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif