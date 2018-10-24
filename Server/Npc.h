#ifndef S_NPC_H
#define S_NPC_H
#include <string>
#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>
#include "Map.h"
#include "NpcCommand.h"

namespace s {
	class Npc {
		float speed;

		b2Body* body;
		int spawnId;
		int type;

		std::string name;

		sf::Vector2f position;
		sf::Vector2f movement;
		sf::Vector2f lastMovement;
		sf::Vector2i size;


		sf::Time respawnTime;
		
		sf::Time deadTimestamp;

		Map* map;
		Location* location;
		NpcCommand* command;
	public:
	
		Npc();
		~Npc();

		NpcCommand* getNpcCommand() const;
		void setNpcCommand(NpcCommand* command);

		void setSpawnId(int id);
		int getSpawnId() const;

		void loadFromJson(std::string file);
		Npc* clone() const;

		json toJson() const;

		void setBody(b2Body* body);
		b2Body* getBody() const;

		void setSpeed(float speed);
		float getSpeed() const;

		void setMap(Map* map);
		Map* getMap() const;

		void setName(std::string name);
		std::string getName() const;

		void setMovement(float movementX, float movementY);
		void setMovement(sf::Vector2f movement, NpcUpdateEvents * npcUpdateEvents);
		sf::Vector2f getMovement() const;

		void setSize(sf::Vector2i size);
		sf::Vector2i getSize() const;

		void setPosition(sf::Vector2f position);
		sf::Vector2f getPosition() const;

		void setType(int type);
		int getType() const;

		sf::Vector2f getLastMovement() const;

		void setMovementDirection(sf::Vector2f direction, float speed, NpcUpdateEvents * npcUpdateEvents);

		void setLocation(Location* l);
		Location* getLocation() const;
	};
}

#endif