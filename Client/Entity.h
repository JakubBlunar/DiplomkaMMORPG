#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/System/Time.hpp>
#include "Subscriber.h"
#include "Component.h"
#include <Box2D/Dynamics/b2Body.h>

class Game;
class Map;

enum class EntityType {
	PLAYER,
	GAMEOBJECT,
	COLLIDER,
};

enum EntityCategory {
	BOUNDARY = 0x0001,
	PLAYER = 0x0002,
	ENEMY_PLAYER = 0x0004,
	GAME_OBJECT = 0x0008,
	SENSOR = 0x0010
};

class Entity : public Subscriber {
public:
	virtual ~Entity();

	virtual void update(sf::Time elapsedTime, Map* map, Game* g);
	Component* getComponent(ComponentType type) const;
	virtual void setBody(b2Body* body);
	virtual b2Body* getBody();

	virtual EntityType getType() = 0;
	virtual EntityCategory getEntityCategory() = 0;
	virtual uint16 getCollisionMask() = 0;

	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	long long getId() const;

protected:
	Entity(long long id);
	std::vector<Component *> components;
	b2Body* body;
	std::string name;
	long long id;
};


#endif
