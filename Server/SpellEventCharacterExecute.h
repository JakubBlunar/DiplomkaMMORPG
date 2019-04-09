#ifndef S_SPELL_EVENT_EXECUTE
#define S_SPELL_EVENT_EXECUTE

#include "SpellEvent.h"
#include "SpellConstants.h"

namespace s {
	class Character;
	class Npc;

	class SpellEventCharacterExecute :
		public SpellEvent
	{
		Character* character;



	public:
		SpellEventCharacterExecute();
		virtual ~SpellEventCharacterExecute();

		void setCharacter(Character* character);
		Character* getCharacter() const {
			return character;
		}

		SpellTarget spellTarget;
		Character* targetCharacter;
		Npc* targetNpc;
		sf::Vector2f targetPosition;

		void execute(Server* s) override;
		void interrupt(bool sendInfo) override;
	};

}

#endif
