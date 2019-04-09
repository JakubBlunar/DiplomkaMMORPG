#ifndef SPELL_EVENT_APPLY_EFFECTS_H
#define SPELL_EVENT_APPLY_EFFECTS_H

#include "SpellEvent.h"

namespace s {
	class Spell;
	class Entity;

	class SpellEventApplyEffects :
		public SpellEvent {

		Spell* spell;
		Entity* entity;
	public:
		SpellEventApplyEffects(Spell* spell, Entity* entity);
		virtual ~SpellEventApplyEffects();

		void execute(Server* s) override;
	};
}

#endif
