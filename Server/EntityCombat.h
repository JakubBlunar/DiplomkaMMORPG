#ifndef S_ENTITY_COMBAT
#define S_ENTITY_COMBAT

#include <set>
#include "Character.h"

namespace s {
	class EntityCombat {
	public:
		EntityCombat();
		virtual ~EntityCombat();

		bool hasCombat() const { return !attackingCharacters.empty() || !attackingNpcs.empty(); }

		std::set<Character*> attackingCharacters;
		std::set<Npc*> attackingNpcs;

		Entity* target;

		bool isCharacterAttacking(Character* character);
		void setAttackingCharacter(Character* character);

		void setAttackingNpc(Npc* npc);
		bool isNpcAttacking(Npc* npc);

		sf::Vector2f startCombatPosition;

		void reset();
	};
}
#endif