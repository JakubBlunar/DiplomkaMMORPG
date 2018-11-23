#ifndef ENTITY_POSITION_H
#define ENTITY_POSITION_H

#include <SFML/System/Vector2.hpp>
#include <Box2D/Dynamics/b2Body.h>

namespace s {
	class Location;
	class Map;

	class EntityPosition
	{
	public:
		EntityPosition();

		float speed;
		b2Body* body;
		sf::Vector2f position;
		sf::Vector2f movement;
		sf::Vector2f lastMovement;
		sf::Vector2i size;
		
		Map* map;
		Location* location;
		int mapId;

		Map* getMap() const;
		void setMap(Map *map);

		void setBody(b2Body* body);
		b2Body* getBody() const;

		void setSpeed(float speed);
		float getSpeed() const;

		void setSize(sf::Vector2i size);
		sf::Vector2i getSize() const;

		void setPosition(sf::Vector2f position);
		sf::Vector2f getPosition() const;

		sf::Vector2f getLastMovement() const;
	
		void setLocation(Location* l);
		Location* getLocation() const;
	};
}

#endif
