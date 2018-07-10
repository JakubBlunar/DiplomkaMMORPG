#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/System/Time.hpp>
#include "Subscriber.h"
#include "Component.h"

class Map;

class Entity: Subscriber
{
public:
	virtual ~Entity();

	virtual void update(sf::Time elapsedTime, Map* map) = 0;
	Component* getComponent(ComponentType type) const;

protected:
	Entity();
	std::vector<Component *> components;
};


#endif
