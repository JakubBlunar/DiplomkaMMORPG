#pragma once
#include "Effect.h"

namespace s {
	class Npc;
	class Character;

	class EffectDealDamage :
		public Effect {

		float modifier;

		void dealDamage(Character* caster, Character* target) const;
		void dealDamage(Character* caster, Npc* target) const;
		void dealDamage(Npc* caster, Character* target) const;
		void dealDamage(Npc* caster, Npc* target) const;
	public:
		EffectDealDamage(SpellInfo spellInfo, float modifier);
		virtual ~EffectDealDamage();

		void apply(Entity* caster, Entity* target) override;
		Effect* clone() override;
		void loadFromJson(json jsonData) override;
	};
}
