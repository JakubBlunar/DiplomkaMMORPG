#include "Spell.h"
#include "ServerGlobals.h"
#include "Npc.h"
#include "EffectModifyAttributes.h"
#include "EffectDealDamage.h"
#include "EffectHealTarget.h"

s::Spell::Spell(): instanceId(-1), target(nullptr) {
	owner = nullptr;
}


s::Spell::~Spell() {
	effects.clear(); 
}

void s::Spell::addEffect(Effect* effect) {
	effects.push_back(effect);
}

void s::Spell::cast(Entity* entity) {
	for (Effect* effect : effects) {
		effect->apply(owner, target);
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

void s::Spell::loadFromJson(json jsonData) {
	json jeffects = jsonData["effects"].get<json::object_t>();
	for (auto& el : jeffects.items()) {
		int key = stoi(el.key());
		json data = el.value();

		Effect *e = nullptr;
		switch (key) {
			case 1: {
				e = new EffectModifyAttributes(spellInfo);
				break;
			}
			case 2: {
				e = new EffectDealDamage(spellInfo, 1);
				break;
			}
			case 3: {
				e = new EffectHealTarget(spellInfo, 1);
				break;
			}
			default: continue;
		}

		if (!data.empty()) {
			e->loadFromJson(data);
		}
		
		effects.push_back(e);
	}
}

