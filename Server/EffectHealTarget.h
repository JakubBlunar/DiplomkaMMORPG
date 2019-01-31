#pragma once
#include "Effect.h"

namespace s {
	class Character;
	class Npc;

	class EffectHealTarget :
		public Effect {
		float modifier;

		void castHealing(Character* caster, Character* target) const;
		void castHealing(Character* caster, Npc* target) const;
		void castHealing(Npc* caster, Character* target) const;
		void castHealing(Npc* caster, Npc* target) const;
	public:
		EffectHealTarget(SpellInfo spellInfo, float modifier);
		virtual ~EffectHealTarget();

		void apply(Entity* caster, Entity* target) override;
		Effect* clone() override;
		void loadFromJson(json jsonData) override;
	};

}
