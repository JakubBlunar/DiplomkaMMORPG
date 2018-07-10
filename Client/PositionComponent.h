#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "Component.h"
#include <SFML/System/Vector2.hpp>

class PositionComponent :
	public Component
{
public:
	PositionComponent();
	~PositionComponent();

	void update(sf::Time elapsedTime, Entity* entity, Map* map) override;
	ComponentType getType() override;
	void setMovementDirection(sf::Vector2f direction);

private:
	bool					isMovingUp;
	bool					isMovingDown;
	bool					isMovingRight;
	bool					isMovingLeft;

	float					speed;
	sf::Vector2f			movement;
	sf::Vector2f			lastMovement;
};

#endif
