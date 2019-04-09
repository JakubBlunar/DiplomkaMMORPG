#include "stdafx.h"
#include "EventIncreaseCharacterAttribute.h"


EventIncreaseCharacterAttribute::EventIncreaseCharacterAttribute() : playerId(0), attribute() {}

EventIncreaseCharacterAttribute::EventIncreaseCharacterAttribute(int playerId, EntityAttributeType attribute) {
	this->playerId = playerId;
	this->attribute = attribute;
	id = INCREASE_CHARACTER_ATTRIBUTE;
}

EventIncreaseCharacterAttribute::~EventIncreaseCharacterAttribute()
{
}

bool EventIncreaseCharacterAttribute::loadFromPacket(sf::Packet* p) {
	sf::Uint8 attributeNum;
	if (*p >> playerId >> attributeNum) {
		attribute = static_cast<EntityAttributeType>(attributeNum);
		return true;
	}
	return false;
}

sf::Packet* EventIncreaseCharacterAttribute::toPacket() {
	sf::Packet* p = new sf::Packet();
	if (*p << id << playerId << static_cast<sf::Uint8>(attribute)) {
		return p;
	}
	throw "Cannot convert to packet";
}
