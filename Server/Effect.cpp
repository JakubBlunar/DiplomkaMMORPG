#include "Effect.h"

s::Effect::Effect(SpellInfo* spellInfo) {
	this->spellInfo = spellInfo;
}
int s::Effect::getId() { return id; }
void s::Effect::apply(Entity* caster, Entity* target) {}


void s::Effect::loadFromJson(json jsonData) {}
void s::Effect::loadFromFile(std::string filename) {}


