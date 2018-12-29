#ifndef S_SPELL_MANAGER_H
#define S_SPELL_MANAGER_H
#include "IDManager.h"
#include "Effect.h"
#include "Spell.h"

class SpellManager {
	IDManager<sf::Int32> spellIds;
	IDManager<sf::Int32> effectIds;

public:
	SpellManager();
	~SpellManager();
};

#endif
