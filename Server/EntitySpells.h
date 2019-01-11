#ifndef ENTITY_SPELLS_H
#define ENTITY_SPELLS_H
#include "SpellEventCharacterExecute.h"

namespace s {
	class EntitySpells {
	protected:
		sf::Mutex spellMutex;

		SpellEvent* castingSpell;
	public:
		EntitySpells();
		virtual ~EntitySpells();

		void setCastingSpell(SpellEvent* castingSpell) {
			sf::Lock lock(spellMutex);
			this->castingSpell = castingSpell;
		}

		SpellEvent* getEventWithCastingSpell() const {
			return castingSpell;
		}

		bool isCasting() const {
			return castingSpell != nullptr;
		}


	};
}

#endif
