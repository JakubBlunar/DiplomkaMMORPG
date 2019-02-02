#include "stdafx.h"
#include "EventFreeSpellToLearn.h"


EventFreeSpellToLearn::EventFreeSpellToLearn()
{
	id = FREE_SPELL_TO_LEARN;
}


EventFreeSpellToLearn::~EventFreeSpellToLearn()
{
}

bool EventFreeSpellToLearn::loadFromPacket(sf::Packet* p) {
	int count; 
	if (*p >> count) {
		for (int i = 0; i < count; i++) {
			int spellId;
			if(*p >> spellId) {
				spellIds.push_back(spellId);
			} else return false;
		}
		return true;
	}
	return false;

}
sf::Packet* EventFreeSpellToLearn::toPacket() {
	sf::Packet* p = new sf::Packet();

	int count = spellIds.size(); 
	if (*p << id << count) {
		for (int spellId : spellIds) {
			if(*p << spellId) continue;
			throw "Could not load packet";
		}
		return p;
	}
	throw "Could not load packet";
}
