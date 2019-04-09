#ifndef MANAGER_H
#define MANAGER_H
#include <SFML/System/Time.hpp>

namespace s {
	class Server;
}

namespace s {
	class Manager {
	protected:
		Manager();
		~Manager();
		bool dynamic;
	public:
		bool isDynamic() const;

		virtual void update(sf::Time elapsedTime, Server* s) {
		}
	};
}


#endif
