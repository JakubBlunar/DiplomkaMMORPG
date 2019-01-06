#include "SpellEventExecute.h"
#include <spdlog/spdlog.h>
#include "Character.h"


s::SpellEventExecute::SpellEventExecute(): character(nullptr) {}


s::SpellEventExecute::~SpellEventExecute()
{
}

void s::SpellEventExecute::setCharacter(Character* character) {
	this->character = character;
	character->setCastingSpell(this);
}

void s::SpellEventExecute::execute(Server* s) {
	character->setCastingSpell(nullptr);


	spdlog::get("log")->info("Casting spell {}: {}", character->name, spellInfo->name);
}
