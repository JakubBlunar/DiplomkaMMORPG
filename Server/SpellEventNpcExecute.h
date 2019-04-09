#ifndef S_SPELL_EVENT_NPC_EXECUTE_H
#define S_SPELL_EVENT_NPC_EXECUTE_H

#include "SpellEvent.h"
#include "SpellConstants.h"

namespace s {
	class Character;
	class Npc;

	class SpellEventNpcExecute :
		public SpellEvent {
		Npc* npc;
	public:
		SpellEventNpcExecute();
		virtual ~SpellEventNpcExecute();

		void setNpc(Npc* npc);

		SpellTarget spellTarget;
		Character* targetCharacter;
		Npc* targetNpc;

		Npc* getNpc() const {
			return npc;
		}

		void execute(Server* s) override;
	};

};

#endif
