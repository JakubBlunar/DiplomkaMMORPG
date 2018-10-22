#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include "Subscriber.h"
#include "Component.h"
#include <Box2D/Dynamics/b2Body.h>
#include "EntityConstants.h"
#include <SFML/System.hpp>

struct LastServerPosition {
	float x;
	float y;
	float velocityX;
	float velocityY;
};


class Game;
class Map;

enum class EntityType {
	PLAYER,
	GAMEOBJECT,
	COLLIDER,
	NPC
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
	uint32 getId() const;
	bool containsPoint(sf::Vector2f point) const;
	std::string getName() const;
protected:
	Entity(sf::Uint32 id);
	std::vector<Component *> components;
	b2Body* body;
	std::string name;
	sf::Uint32 id;
};


#endif
