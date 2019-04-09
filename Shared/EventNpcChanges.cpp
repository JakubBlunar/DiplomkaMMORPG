#include "stdafx.h"
#include "EventNpcChanges.h"
#include "DeleteElementPointer.h"


EventNpcChanges::EventNpcChanges() : spawnId(0) {
	id = NPC_CHANGES;
}


EventNpcChanges::~EventNpcChanges()
{
	std::for_each(changes.begin(), changes.end(), DeletePointerElement<NpcChange*>());
	changes.clear();
}

void EventNpcChanges::addChange(NpcChange* change) {
	changes.push_back(change);
}

bool EventNpcChanges::loadFromPacket(sf::Packet* p) {
	int count;
	if (*p >> spawnId >> count) {
		for (int i = 0; i < count; i++) {
			int type;
			std::string data;
			if (*p >> type >> data) {
				NpcChange* change = new NpcChange();
				change->type = static_cast<NpcChangeType>(type);
				change->data = data;
				addChange(change);
			}
			else {
				return false;
			}
		}
		return true;
	}
	return false;
}

sf::Packet* EventNpcChanges::toPacket() {
	sf::Packet* p = new sf::Packet();

	int count = changes.size();
	if (*p << id << spawnId << count) {
		for (auto change : changes) {
			if (*p << change->type << change->data) {

			}
			else {
				throw "Error while inserting into packet";
			}
		}
	}
	else {
		throw "Error while inserting into packet";
	}

	return p;
}
