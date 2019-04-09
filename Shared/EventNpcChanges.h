#ifndef EVENT_NPC_CHANGES_H
#define EVENT_NPC_CHANGES_H

#include "GameEvent.h"
#include "../Shared/NpcConstants.h"
#include "json.hpp"
#include <list>

using json = nlohmann::json;

struct NpcChange {
	NpcChangeType type;
	std::string data;
};

class EventNpcChanges :
	public GameEvent
{
public:
	EventNpcChanges();
	~EventNpcChanges();

	int spawnId;
	std::list<NpcChange*> changes;

	void addChange(NpcChange *change);

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif
