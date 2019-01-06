#ifndef ENTITY_SPELLS_H
#define ENTITY_SPELLS_H
#include "SpellEventExecute.h"

namespace s {
	class EntitySpells {
	protected:
		EntitySpells();
		virtual ~EntitySpells();

		sf::Mutex spellMutex;

		SpellEventExecute* castingSpell;
	public:

		void setCastingSpell(SpellEventExecute* castingSpell) {
			sf::Lock lock(spellMutex);
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
