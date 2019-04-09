#include "stdafx.h"
#include "EventCharacterChoose.h"


EventCharacterChoose::EventCharacterChoose() : characterId(0) {
	id = CHARACTER_CHOOSE;
}


EventCharacterChoose::~EventCharacterChoose() {
}

bool EventCharacterChoose::loadFromPacket(sf::Packet* p) {
	if (*p >> characterId) {
		return true;
	}
	return false;
}

sf::Packet* EventCharacterChoose::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << characterId;
	return p;
}

std::string EventCharacterChoose::toString() const {
	return "Choose character request " + characterId;
}
