#include "EntitySpells.h"
#include "Spell.h"

s::EntitySpells::EntitySpells() : castingSpell(nullptr) {}


s::EntitySpells::~EntitySpells() {}

void s::EntitySpells::setCooldown(int spellId, sf::Time time) {
	auto found = spellCooldowns.find(spellId);
	if (found != spellCooldowns.end()) {
		spellCooldowns[spellId] = time;
	}
	else {
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

bool s::EntitySpells::hasAllSpellCooldown(sf::Time time) const {
	if (spellCooldowns.empty()) return false;

	for (auto it = spellCooldowns.begin(); it != spellCooldowns.end(); ++it) {
		if (it->second < time) return false;
	}
	return true;
}

void s::EntitySpells::addAvailableSpell(SpellInfo* spellInfo) {
	availableSpells.insert(std::make_pair(spellInfo->id, spellInfo));
}

s::SpellInfo* s::EntitySpells::getSpell(int type) {
	auto found = availableSpells.find(type);
	if (found == availableSpells.end())
	{
		return nullptr;
	}
	return found->second;
}

std::map<int, s::SpellInfo*>* s::EntitySpells::getAvailableSpells() {
	return &availableSpells;
}
