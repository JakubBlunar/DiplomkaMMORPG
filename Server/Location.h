#ifndef S_LOCATION_H
#define S_LOCATION_H

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>
#include <list>
#include "SFML/System.hpp"
#include "Npc.h"


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
		sf::Vector2f position;

		std::list<Spawn*> spawns;
		std::list<Npc*> locationNpcs;

		Map* map;
	public:
		Location(int id, b2Vec2* vertices, int verticesCount, sf::Vector2f position, Map* m);
		~Location();

		sf::Vector2f generateRandomPoint() const;
		sf::Vector2f getPosition() const;

		Map* getMap() const;
		void addSpawn(Spawn* spawn);
		void addNpc(Npc* npc);
		void update(sf::Time elapsedTime, s::Server* s, Map* map);

		void checkNpcRespawn(Npc* npc, sf::Time serverTime, Server* s) const;
	};
}
#endif