#ifndef S_SPAWN_H
#define S_SPAWN_H

#include <vector>
#include "Npc.h"
#include <SFML/System/Time.hpp>


namespace s {
	class Server;

	class Spawn
	{
		int npctype;
		int locationId;

		int maxSpawnedNpcs;

		std::vector<s::Npc*> spawnedNpcs;
		sf::Time lastUpdate;
	public:
		Spawn();
		~Spawn();

		void init();
		void update(sf::Time elapsedTime, s::Server* s);
	};
}

#endif