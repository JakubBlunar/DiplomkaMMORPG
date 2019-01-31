#include "Effect.h"

s::Effect::Effect(SpellInfo spellInfo) {
	this->spellInfo = spellInfo;
}

void s::Effect::apply(Entity* caster, Entity* target) {}


void s::Effect::loadFromJson(json jsonData) {}
void s::Effect::loadFromFile(std::string filename) {}


