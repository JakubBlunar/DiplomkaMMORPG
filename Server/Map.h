﻿#ifndef SERVER_MAP_H
#define SERVER_MAP_H
#include "Character.h"
#include <Box2D/Dynamics/b2World.h>
#include "Server.h"
#include <Box2D/Dynamics/b2Body.h>
#include "MapGrid.h"
#include "ObjectPool.h"
#include <map>
#include "EntityToEntityRayCast.h"

namespace s {
	class Npc;
	class Location;

	class Map {
		int id;
		b2World* world;
		std::vector<Character *> characters;
		std::map<int, Character*> charactersById;
		std::map<int, Npc*> npcsBySpawnId;
		std::map<int, MovableSpell*> spellsById;

		std::queue<int> spellsToRemove;

		std::vector<Npc*> npcs;
		int width, height;

		std::map<int, s::Location*> locations;

		ObjectPool<MapGrid> mapGridObjectPool;

		sf::Time lastUpdateNpc;
		sf::Time npcUpdateInterval;
	public:
		Map();
		~Map();
		sf::Mutex lock;

		int getId() const;

		void addCharacter(Character* character);
		void removeCharacter(Character* character);

		void addNpc(Npc* npc);
		void removeNpc(Npc* npc);

		void addSpell(MovableSpell* spell);
		void removeSpell(MovableSpell* spell);


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


		Character* getCharacterById(int id);
		Npc* getNpcBySpawnId(int id);

		EntityToEntityRayCast* makeRayCast(Entity* startEntity, Entity* endEntity);

		sf::Vector2f respawn;


	};

}

#endif
