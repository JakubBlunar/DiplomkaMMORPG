#include "stdafx.h"
#include "EventPlayerStartCastSpell.h"


EventPlayerStartCastSpell::EventPlayerStartCastSpell(): spellId(0), startCastTimestamp(0) {
	id = PLAYER_START_CAST_SPELL;
}


EventPlayerStartCastSpell::~EventPlayerStartCastSpell()
{
}

void EventPlayerStartCastSpell::accept(EventVisitor* v) {
	v->visit(this);
}

bool EventPlayerStartCastSpell::loadFromPacket(sf::Packet* p) {
	if (*p >> spellId >> startCastTimestamp) {
		return true;
	}
	return false;
}

sf::Packet* EventPlayerStartCastSpell::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << spellId << startCastTimestamp;
	return p;
}
