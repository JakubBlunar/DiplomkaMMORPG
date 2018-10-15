#ifndef SERVER_MAP_H
#define SERVER_MAP_H
#include "Character.h"
#include <Box2D/Dynamics/b2World.h>
#include "Server.h"
#include <Box2D/Dynamics/b2Body.h>
#include "MapGrid.h"
#include "ObjectPool.h"

namespace s {
	class Npc;
	class Location;

	class Map {
		int id;
		b2World* world;
		std::vector<Character *> characters;
		std::vector<Npc*> npcs;
		int width, height;
		sf::Mutex lock;

		std::map<int, s::Location*> locations;

		ObjectPool<MapGrid> mapGridObjectPool;
	public:
		Map();
		~Map();

		int getId() const;

		void addCharacter(Character* character);
		void removeCharacter(Character* character);

		void addNpc(Npc* npc);
		void removeNpc(Npc* npc);

		void update(sf::Time deltaTime, Server* s);

		void loadFromJson(std::string path, Server *s);

		json getCharactersJson();
		json getNpcsJson();

		MapGrid* getGrid();
		void returnGrid(MapGrid* grid);

		void sendEventToAnotherPlayers(GameEvent* event, int characterId);
		void sendEventToAllPlayers(GameEvent* event);

		void handleEvent(GameEvent* event, Session* playerSession, Server* server);

		b2Body * createCircle(b2BodyType bodyType, float x, float y, float radius, int16 categoryBits, uint16 maskBits);
		b2Body* createBox(b2BodyType bodyType, float x, float y, float width, float height, int16 categoryBits, uint16 maskBits);
	};

}

#endif
