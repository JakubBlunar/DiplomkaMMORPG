#ifndef NPC_COMMAND_STAY_H
#define NPC_COMMAND_STAY_H
#include "Server.h"
#include "NpcCommand.h"

namespace s {
	class NpcCommandStay: public NpcCommand
	{
		Npc* npc;
		Map* map;
		Server* server;
		bool inited;
	public:
		NpcCommandStay(Npc* npc, Map* m, Server *s, sf::Time duration);
		~NpcCommandStay();
		void update(sf::Time elapsedTime, NpcUpdateEvents* npcUpdateEvents) override;
	};
}

#endif
