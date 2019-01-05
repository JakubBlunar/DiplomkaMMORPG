#ifndef ENTITY_SPELLS_H
#define ENTITY_SPELLS_H
#include "SpellEventExecute.h"

namespace s {
	class EntitySpells {
	protected:
		EntitySpells();
		virtual ~EntitySpells();

		SpellEventExecute* castingSpell;

	public:

		void setCastingSpell(SpellEventExecute* castingSpell) {
			this->castingSpell = castingSpell;
		}

		SpellEventExecute* getEventWithCastingSpell() const {
			return castingSpell;
		}

		bool isCasting() const {
			return castingSpell != nullptr;
		}


	};
}

#endif
