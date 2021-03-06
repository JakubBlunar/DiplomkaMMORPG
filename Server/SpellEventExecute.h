#ifndef S_SPELL_EVENT_EXECUTE
#define S_SPELL_EVENT_EXECUTE

#include "SpellEvent.h"

namespace s {
	class Character;

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
		
		void execute(Server* s) override;
	};

}

#endif