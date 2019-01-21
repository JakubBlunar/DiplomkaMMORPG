#include "EntitySpells.h"


s::EntitySpells::EntitySpells(): castingSpell(nullptr) {}


s::EntitySpells::~EntitySpells() {}

void s::EntitySpells::setCooldown(int spellId, sf::Time time) {
	auto found = spellCooldowns.find(spellId);
	if (found != spellCooldowns.end()) {
		spellCooldowns[spellId] = time;
	} else {
		spellCooldowns.insert(std::make_pair(spellId, time));
	}
}

bool s::EntitySpells::hasCooldown(int spellId, sf::Time serverTime) const {
	auto found = spellCooldowns.find(spellId);
	if (found == spellCooldowns.end()) {
		return false;
	}
	return found->second > serverTime;
}
