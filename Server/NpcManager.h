#ifndef NPC_MANAGER_H
#define NPC_MANAGER_H

#include <SFML/System/Mutex.hpp>
#include <map>
#include "Manager.h"

namespace s {
	class Npc;

	class NpcManager: public Manager
	{
		std::map<int, Npc*> npcs;
		sf::Mutex lock;
	public:
		NpcManager();
		~NpcManager();

		Npc* createNpc(int npcType);
		Npc* findNpc(int spawnId);
	};
}

#endif