#include "Spell.h"
#include "ServerGlobals.h"
#include "Npc.h"

s::Spell::Spell(): instanceId(-1), target(nullptr) {
	owner = nullptr;
}


s::Spell::~Spell() {}

void s::Spell::addEffect(Effect* effect) {
	effects.push_back(effect);
}

void s::Spell::cast(Entity* entity) {
	for (Effect* effect : effects) {
		effect->apply(entity, target);
	}
}

s::Spell* s::Spell::clone() const {
	Spell* cloned = new Spell();
	
	cloned->setInstanceId(instanceId);
	cloned->spellInfo = spellInfo;

	for (Effect* const effect : effects) {
		cloned->addEffect(effect->clone());
	}

	return cloned;
}

void s::Spell::loadFromJson(json data) {
	
}

