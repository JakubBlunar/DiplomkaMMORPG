#ifndef S_SPELL_EVENT_NPC_EXECUTE_H
#define S_SPELL_EVENT_NPC_EXECUTE_H

#include "SpellEvent.h"

namespace s {
	class Npc;

	class SpellEventNpcExecute :
		public SpellEvent {
		Npc* npc;
	public:
		SpellEventNpcExecute();
		virtual ~SpellEventNpcExecute();

		void setNpc(Npc* npc);

		Npc* getCharacter() const {
			return npc;
		}

		void execute(Server* s) override;
	};

};

#endif
