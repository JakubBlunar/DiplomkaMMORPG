#ifndef S_NPC_COMMAND_COMBAT
#define S_NPC_COMMAND_COMBAT

#include "NpcCommand.h"
#include "Entity.h"

namespace s {
	class NpcCommandCombat :
		public NpcCommand {

		Npc* npc;
		Entity* target;
	public:
		NpcCommandCombat(Npc* npc, Server* s);
		~NpcCommandCombat();

		void update(sf::Time elapsedTime, NpcUpdateEvents* npcUpdateEvents) override;
	};

}

#endif
