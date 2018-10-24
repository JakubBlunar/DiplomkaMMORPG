#ifndef NPC_MANAGER_H
#define NPC_MANAGER_H

#include <SFML/System/Mutex.hpp>
#include <map>
#include "Manager.h"
#include "EventNpcsMovementChange.h"

namespace s {
	class Npc;

	struct NpcUpdateEvents {
		EventNpcsMovementChange* npcsMovementChange;
	};

	class NpcManager: public Manager
	{
		std::map<int, Npc*> npcs;
		sf::Mutex lock;
	public:
		NpcManager();
		~NpcManager();

		Npc* createNpc(int npcType);
		Npc* findNpc(int spawnId);

		void updateNpc(sf::Time elapsedTime, Npc* npc, Server* s, NpcUpdateEvents * npcUpdateEvents);
	};
}

#endif