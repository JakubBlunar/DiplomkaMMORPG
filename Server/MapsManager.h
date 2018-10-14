#ifndef MAPS_MANAGER_H
#define MAPS_MANAGER_H

#include "Manager.h"
#include <map>
#include <vector>

namespace s {
	class Map;
	class Server;
}

namespace s {
	class MapsManager :
		public Manager {

	public:
		MapsManager();
		~MapsManager();

		std::map<int, Map*> maps;

		Map* getMap(int id);
		void update(sf::Time elapsedTime, Server* s) override;
		void init(Server *s);
		void read_directory(std::string pattern, std::vector<std::string>& v) const;
	};

}

#endif
