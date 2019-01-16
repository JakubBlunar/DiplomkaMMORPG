#pragma once
#ifndef EVENT_NPC_ATTRIBUTES_CHANGED
#define EVENT_NPC_ATTRIBUTES_CHANGED

#include "GameEvent.h"
#include <SFML/Graphics.hpp>
#include "EntityConstants.h"

class EventAttributesChanged :
	public GameEvent
{
	
public:
	EventAttributesChanged();
	~EventAttributesChanged();

	int spawnId;
	std::map<EntityAttributeType, float> changes;
	void setChange(EntityAttributeType type, float newValue);

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif
