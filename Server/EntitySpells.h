#ifndef ENTITY_SPELLS_H
#define ENTITY_SPELLS_H
#include "SpellEventCharacterExecute.h"
#include <map>

namespace s {
	class EntitySpells {
	protected:
		sf::Mutex spellMutex;

		std::map<int, sf::Time> spellCooldowns;
		SpellEvent* castingSpell;
	public:
		EntitySpells();
		virtual ~EntitySpells();

		std::map<int, SpellInfo*> availableSpells;

		void setCooldown(int spellId, sf::Time time);
		bool hasCooldown(int spellId, sf::Time serverTime) const;
		bool hasAllSpellCooldown(sf::Time time) const;

		void addAvailableSpell(SpellInfo* spellInfo);
		SpellInfo* getSpell(int type);
		std::map<int, SpellInfo*>* getAvailableSpells();

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
