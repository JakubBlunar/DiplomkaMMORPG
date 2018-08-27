#include "stdafx.h"
#include "EventCharacterMapJoin.h"


EventCharacterMapJoin::EventCharacterMapJoin()
{
	id = CHARACTER_MAP_JOIN;
}


EventCharacterMapJoin::~EventCharacterMapJoin()
{
}

void EventCharacterMapJoin::accept(EventVisitor* v) {
	v->visit(this);
}

bool EventCharacterMapJoin::loadFromPacket(sf::Packet * p)
{
	if (*p >> mapId >> characterData) {
		return true;
	}
	return false;
}

sf::Packet* EventCharacterMapJoin::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << mapId << characterData;
	return p;
}

std::string EventCharacterMapJoin::toString() const {
	return "Event character map join" + std::to_string(mapId) + ", " + characterData;
}

