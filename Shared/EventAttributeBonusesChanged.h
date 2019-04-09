#ifndef EVENT_NPC_ATTRIBUTES_BONUSES_CHANGED
#define EVENT_NPC_ATTRIBUTES_BONUSES_CHANGED

#include "GameEvent.h"
#include <SFML/Graphics.hpp>
#include "EntityConstants.h"

class EventAttributeBonusesChanged :
	public GameEvent
{

public:
	EventAttributeBonusesChanged();
	~EventAttributeBonusesChanged();

	int spawnId;
	EntityCategory entityType;
	std::map<EntityAttributeType, float> changes;
	void setChange(EntityAttributeType type, float newValue);

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif
