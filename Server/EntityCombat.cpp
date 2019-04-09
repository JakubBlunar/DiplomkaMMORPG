#include "EntityCombat.h"



s::EntityCombat::EntityCombat()
{
}


s::EntityCombat::~EntityCombat()
{
}

bool s::EntityCombat::isCharacterAttacking(Character* character) {
	return attackingCharacters.find(character) != attackingCharacters.end();
}

void s::EntityCombat::setAttackingCharacter(Character* character) {
	attackingCharacters.insert(character);
}

void s::EntityCombat::removeAttackingCharacter(Character* character) {
	attackingCharacters.erase(character);
}

void s::EntityCombat::setAttackingNpc(Npc* npc) {
	attackingNpcs.insert(npc);
}

bool s::EntityCombat::isNpcAttacking(Npc* npc) {
	return attackingNpcs.find(npc) != attackingNpcs.end();
}

void s::EntityCombat::removeAttackingNpc(Npc* npc) {
	attackingNpcs.erase(npc);
}

void s::EntityCombat::reset() {
	startCombatPosition = sf::Vector2f(-1, -1);
	attackingCharacters.clear();
	attackingNpcs.clear();
	target = nullptr;
}
