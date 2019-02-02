#ifndef EVENT_FREE_SPELLS_TO_LEARN_H
#define EVENT_FREE_SPELLS_TO_LEARN_H

#include "GameEvent.h"
class EventFreeSpellToLearn :
	public GameEvent
{
public:
	EventFreeSpellToLearn();
	virtual ~EventFreeSpellToLearn();

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::vector<int> spellIds;
};

#endif
