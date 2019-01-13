#include "SpellEventApplyEffects.h"
#include "Spell.h"
#include <spdlog/spdlog.h>

s::SpellEventApplyEffects::SpellEventApplyEffects(Spell* spell, Entity* entity)
{
	this->spell = spell;
	this->entity = entity;
}


s::SpellEventApplyEffects::~SpellEventApplyEffects()
{
}

void s::SpellEventApplyEffects::execute(Server* s) {
	spdlog::get("log")->info("Applying effect of spell {}, id: {}", spell->spellInfo.name, spell->getInstanceId());
	spell->cast(entity);
}
