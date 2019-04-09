#ifndef EVENT_SPELL_CAST_RESULT_H
#define EVENT_SPELL_CAST_RESULT_H

#include "GameEvent.h"
#include "EntityConstants.h"
#include "SpellConstants.h"

class EventSpellCastResult :
	public GameEvent
{
public:
	EventSpellCastResult();
	virtual ~EventSpellCastResult();

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;

	int entityId;
	EntityCategory entityCategory;
	int spellId;
	SpellCastResultCode result;
	SpellTarget target;
	int targetId;
	sf::Vector2f startPosition;
	sf::Vector2f targetPosition;
};

#endif
