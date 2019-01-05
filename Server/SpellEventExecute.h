#ifndef S_SPELL_EVENT_EXECUTE
#define S_SPELL_EVENT_EXECUTE

#include "SpellEvent.h"

namespace s {
	class Character;

	class SpellEventExecute :
		public SpellEvent
	{
		Character* character;
	public:
		SpellEventExecute();
		virtual ~SpellEventExecute();

		void setCharacter(Character* character);
		
		void execute(Server* s) override;
	};

}

#endif