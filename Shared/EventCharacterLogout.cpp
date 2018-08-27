#include "stdafx.h"
#include "EventCharacterLogout.h"


EventCharacterLogout::EventCharacterLogout(): characterId(0) {
	id = CHARACTER_LOGOUT;
}


EventCharacterLogout::~EventCharacterLogout()
{
}

void EventCharacterLogout::accept(EventVisitor*) {
	
}

bool EventCharacterLogout::loadFromPacket(sf::Packet * p)
{
	if(*p >> characterId) {
		return true;
	}
	return false;
}

sf::Packet* EventCharacterLogout::toPacket()
{
	sf::Packet* p = new sf::Packet();
	*p << id << characterId;
	return p;
}

std::string EventCharacterLogout::toString() const {
	return "Event character logout: " + std::to_string(characterId);
}
