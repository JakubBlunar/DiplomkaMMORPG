#include "SpellEventExecute.h"
#include <spdlog/spdlog.h>
#include "Character.h"


s::SpellEventExecute::SpellEventExecute()
{
}


s::SpellEventExecute::~SpellEventExecute()
{
}

void s::SpellEventExecute::setCharacter(Character* character) {
	this->character = character;
	character->setCastingSpell(this);
}

void s::SpellEventExecute::execute(Server* s) {
	spdlog::get("log")->info("Casting spell {}: {}", character->name, spellInfo->name);
}
