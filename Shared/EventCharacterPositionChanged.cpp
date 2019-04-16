#include "stdafx.h"
#include "EventCharacterPositionChanged.h"


EventCharacterPositionChanged::EventCharacterPositionChanged()
{
	id = CHARACTER_POSITION_CHANGE;
}


EventCharacterPositionChanged::~EventCharacterPositionChanged()
{
}

bool EventCharacterPositionChanged::loadFromPacket(sf::Packet * p)
{
	if (*p >> characterId >> positionX >> positionY) {
		return true;
	}
	return false;
}

sf::Packet * EventCharacterPositionChanged::toPacket()
{
	sf::Packet* p = new sf::Packet();
	if (*p << id << characterId << positionX << positionY) {
		return p;
	}
	throw "Cannot convert to packet";
}
