#include "stdafx.h"
#include "EventAttributeBonusesChanged.h"
#include "EntityConstants.h"

EventAttributeBonusesChanged::EventAttributeBonusesChanged() : spawnId(0), entityType() {
	id = ATTRIBUTES_BONUSES_CHANGES;
}


EventAttributeBonusesChanged::~EventAttributeBonusesChanged()
{
}

void EventAttributeBonusesChanged::setChange(EntityAttributeType type, float newValue) {
	auto found = changes.find(type);
	if (found == changes.end()) {
		changes.insert(std::make_pair(type, newValue));
	}
	else {
		changes[type] = newValue;
	}
}

bool EventAttributeBonusesChanged::loadFromPacket(sf::Packet* p) {
	sf::Uint8 count;
	sf::Uint32 entityTypeNum;
	if (*p >> entityTypeNum >> spawnId >> count) {
		entityType = static_cast<EntityCategory>(entityTypeNum);
		for (int i = 0; i < count; i++) {
			sf::Uint8 at;
			float newValue;
			if (*p >> at >> newValue) {
				setChange(static_cast<EntityAttributeType>(at), newValue);
			}
			else {
				return false;
			}
		}
		return true;
	}
	return false;
}


sf::Packet* EventAttributeBonusesChanged::toPacket() {
	sf::Packet* p = new sf::Packet();

	sf::Uint8 size = (sf::Uint8)changes.size();
	if (*p << static_cast<sf::Uint32>(entityType) << id << spawnId << size) {
		for (std::map<EntityAttributeType, float>::iterator it = changes.begin(); it != changes.end(); ++it)
		{
			if (*p << static_cast<sf::Uint8>(it->first) << it->second) {

			}
			else {
				throw "error while creating packet from event";
			}
		}
	}
	else {
		throw "error while creating packet from event";
	}

	return p;
}
