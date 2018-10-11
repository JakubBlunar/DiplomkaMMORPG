#ifndef S_LOCATION_H
#define S_LOCATION_H

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>
#include <list>
#include "SFML/System.hpp"


namespace s {
	class Map;
	class Server;
	class Spawn;

	class Location
	{
		int id;
		b2PolygonShape* shape;
		b2Vec2* vertices;
		b2AABB rect;
		b2Transform transform;
		int verticesCount;

		std::list<Spawn*> spawns;
		Map* map;
	public:
		Location(int id, b2Vec2* vertices, int verticesCount, Map* m);
		~Location();

		sf::Vector2f generateRandomPoint() const;

		Map* getMap() const;
		void addSpawn(Spawn* spawn);
		void update(sf::Time elapsedTime, s::Server* s, Map* map);
	};
}
#endif