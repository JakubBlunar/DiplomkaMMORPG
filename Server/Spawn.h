#ifndef S_SPAWN_H
#define S_SPAWN_H

#include <vector>
#include "Npc.h"
#include <SFML/System/Time.hpp>


namespace s {
	class Server;
	class Location;

	class Spawn
	{
		int npcType;
		int maxSpawnedNpcs;

		Location* location;
		std::vector<s::Npc*> spawnedNpcs;
		sf::Time lastUpdate;
	public:
		Spawn(int npcType, int maxSpawnedNpcs, Location* l);
		~Spawn();

		void init(Server *s);
		void update(sf::Time elapsedTime, s::Server* s, Location* l);
	};
}

#endif