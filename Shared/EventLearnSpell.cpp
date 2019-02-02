#include "stdafx.h"
#include "EventLearnSpell.h"


EventLearnSpell::EventLearnSpell(): spellId(0), success(false) {
	id = LEARN_SPELL;
}


EventLearnSpell::~EventLearnSpell()
{

}

bool EventLearnSpell::loadFromPacket(sf::Packet* p) {
	if(*p >> spellId >> success) {
		return true;
	}
	return false;
}

sf::Packet* EventLearnSpell::toPacket() {
	sf::Packet* p = new sf::Packet();
	if (*p << id << spellId << success) {
		return p;
	}
	throw "Cannot convert to packet";
}

