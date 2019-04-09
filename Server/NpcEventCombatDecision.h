#ifndef S_NPC_EVENT_COMBAT_DECISION
#define S_NPC_EVENT_COMBAT_DECISION

#include "NpcEvent.h"

namespace s {
	class Npc;

	class NpcEventCombatDecision :
		public NpcEvent {
	public:
		NpcEventCombatDecision(Npc* npc);
		virtual ~NpcEventCombatDecision();
	};
}
#endif
