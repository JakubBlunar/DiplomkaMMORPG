#ifndef EVENT_LEARN_SPELL
#define EVENT_LEARN_SPELL

#include "GameEvent.h"
class EventLearnSpell :
	public GameEvent
{
public:
	EventLearnSpell();
	virtual ~EventLearnSpell();

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;

	int spellId;
	bool success;
};

#endif