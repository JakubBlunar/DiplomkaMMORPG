#include "stdafx.h"
#include "EventAttributesChanged.h"


EventAttributesChanged::EventAttributesChanged(): spawnId(0) {
	id = NPC_ATTRIBUTES_CHANGED;
}


EventAttributesChanged::~EventAttributesChanged()
{
}

void EventAttributesChanged::setChange(EntityAttributeType type, float newValue) {
	auto found = changes.find(type);
	if (found == changes.end()) {
		changes.insert(std::make_pair(type, newValue));
	} else {
		changes[type] = newValue;
	}
}

void EventAttributesChanged::accept(EventVisitor* v) {
	v->visit(this);
}

bool EventAttributesChanged::loadFromPacket(sf::Packet* p) {
	sf::Uint8 count;
	if (*p >> spawnId >> count) {
		for (int i = 0; i < count; i++) {
			sf::Uint8 at;
			float newValue;
			if (*p >> at >> newValue) {
				setChange(static_cast<EntityAttributeType>(at), newValue);
			} else {
				return false;
			}
		}
		return true;
	}
	return false;
}


sf::Packet* EventAttributesChanged::toPacket() {
	sf::Packet* p = new sf::Packet();

	sf::Uint8 size = (sf::Uint8)changes.size();
	if (*p << id << spawnId << size) {
		for (std::map<EntityAttributeType, float>::iterator it = changes.begin(); it != changes.end(); ++it)
		{
		    if (*p << static_cast<sf::Uint8>(it->first) << it->second) {
			    
		    } else {
			    throw "error while creating packet from event";
		    }
		}
	} else {
		throw "error while creating packet from event";
	}

	return p;
}
