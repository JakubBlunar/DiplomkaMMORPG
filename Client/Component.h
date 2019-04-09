#ifndef COMPONENT_H
#define COMPONENT_H

#include <SFML/System/Time.hpp>

class Map;
class Entity;

enum class ComponentType {
	POSITION = 0,
	RENDER = 1,
	ATTRIBUTES = 2
};

class Component {
public:
	virtual ~Component();
	virtual void update(sf::Time elapsedTime, Entity* entity, Map* map) = 0;
	virtual ComponentType getType() = 0;
protected:
	Component();
};

#endif
