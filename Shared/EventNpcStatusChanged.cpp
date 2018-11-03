#include "stdafx.h"
#include "EventNpcStatusChanged.h"


EventNpcStatusChanged::EventNpcStatusChanged(): spawnId(0), npcState(NpcState::IDLE) {
	id = NPC_STATUS_CHANGED;
}


EventNpcStatusChanged::~EventNpcStatusChanged()
{
}

void EventNpcStatusChanged::accept(EventVisitor* v) {
	v->visit(this);
}
bool EventNpcStatusChanged::loadFromPacket(sf::Packet* p) {
	sf::Uint8 intState;
	if (*p >> spawnId >> intState) {
		npcState = static_cast<NpcState>(intState);
		return true;
	}
	return false;
}

sf::Packet* EventNpcStatusChanged::toPacket() {
	sf::Packet* p = new sf::Packet();
	if (*p << id << spawnId << static_cast<sf::Uint8>(npcState)) {
		
	} else {
		throw "Error converting event to packet";
	}
	return p;
}
