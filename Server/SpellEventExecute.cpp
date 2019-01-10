#include "SpellEventCharacterExecute.h"
#include <spdlog/spdlog.h>
#include "Character.h"


s::SpellEventCharacterExecute::SpellEventCharacterExecute(): character(nullptr) {}


s::SpellEventCharacterExecute::~SpellEventCharacterExecute()
{
}

void s::SpellEventCharacterExecute::setCharacter(Character* character) {
	this->character = character;
	character->setCastingSpell(this);
}

void s::SpellEventCharacterExecute::execute(Server* s) {
	character->setCastingSpell(nullptr);


	spdlog::get("log")->info("Casting spell {}: {}", character->name, spellInfo->name);
}
