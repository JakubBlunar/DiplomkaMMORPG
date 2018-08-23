#ifndef SERVER_MAP_H
#define SERVER_MAP_H
#include "Character.h"
#include <Box2D/Dynamics/b2World.h>
#include "Server.h"
#include <Box2D/Dynamics/b2Body.h>

namespace s {
	class Map {
		int id;
		b2World* world;
		std::vector<Character *> characters;

		int width, height;
		sf::Mutex lock;

	public:
		Map();
		~Map();

		int getId() const;

		void addCharacter(Character* character);
		void removeCharacter(Character* character);

		void update(sf::Time deltaTime, Server* s);

		void loadFromJson(std::string path);

		json getCharactersJson();

		b2Body* createCircle(b2BodyType bodyType, float x, float y, float radius);
		b2Body* createBox(b2BodyType bodyType, float x, float y, float width, float height);
	};

}

#endif
