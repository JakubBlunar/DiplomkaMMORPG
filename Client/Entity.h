#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/System/Time.hpp>
#include "Subscriber.h"
#include "Component.h"
#include <Box2D/Dynamics/b2Body.h>

class Map;

enum class EntityType
{
	PLAYER
};

class Entity: Subscriber
{
public:
	virtual ~Entity();

	virtual void update(sf::Time elapsedTime, Map* map);
	Component* getComponent(ComponentType type) const;
	virtual void setBody(b2Body * body);
	virtual b2Body* getBody();
	virtual EntityType getType() = 0;

	sf::Vector2f getPosition() const;

protected:
	Entity();
	std::vector<Component *> components;
	b2Body* body;
};


#endif
