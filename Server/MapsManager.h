#ifndef MAPS_MANAGER_H
#define MAPS_MANAGER_H

#include "Manager.h"
#include <map>

namespace s
{
	class Map;
}

namespace s
{
	class MapsManager :
		public Manager
	{

	public:
		MapsManager();
		~MapsManager();

		std::map<int, Map*> maps;

		Map* getMap(int id);
		void update(sf::Time elapsedTime, Server* s) override;
	};

}

#endif
