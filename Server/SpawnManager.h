#ifndef S_SPAWN_MANAGER_H
#define S_SPAWN_MANAGER_H
#include "Spawn.h"
#include <list>


namespace sf {
	class Time;
}

namespace s {
	class Map;
	class Server;

	
	class SpawnManager
	{
		std::list<Spawn*> spawns;

	public:
		SpawnManager();
		~SpawnManager();

		void update(sf::Time elapsedTime, s::Server* s, s::Map* m);
	};
}



#endif