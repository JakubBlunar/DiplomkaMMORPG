#include "stdafx.h"
#include "EventCharacterChooseResponse.h"


EventCharacterChooseResponse::EventCharacterChooseResponse(): success(false) {
	id = CHARACTER_CHOOSE_RESPONSE;
}


EventCharacterChooseResponse::~EventCharacterChooseResponse() {
}

void EventCharacterChooseResponse::accept(EventVisitor* v) {
	v->visit(this);
}

bool EventCharacterChooseResponse::loadFromPacket(sf::Packet* p) {
	if (*p >> success >> message >> characterData) {
		return true;
	}
	return false;
}

sf::Packet* EventCharacterChooseResponse::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << success << message << characterData;
	return p;
}

std::string EventCharacterChooseResponse::toString() const {
	return "Character choose response " + std::to_string(success) + ", " + message + ", " + characterData;
}
