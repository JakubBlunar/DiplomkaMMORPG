#include "stdafx.h"
#include "EventCharacterMapLeave.h"


EventCharacterMapLeave::EventCharacterMapLeave()
{
	id = CHARACTER_MAP_LEAVE;
}


EventCharacterMapLeave::~EventCharacterMapLeave()
{
}

bool EventCharacterMapLeave::loadFromPacket(sf::Packet* p) {
	if (*p >> characterId >> mapId) {
		return true;
	}
	return false;
}

sf::Packet* EventCharacterMapLeave::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << characterId << mapId;
	return p;
}

std::string EventCharacterMapLeave::toString() const {
	return "Event character map leave: character: " + std::to_string(characterId) + ", map: " + std::to_string(mapId);
}
