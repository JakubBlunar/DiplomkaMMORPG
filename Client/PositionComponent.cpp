#include "PositionComponent.h"
#include <SFML/Window/Keyboard.hpp>


PositionComponent::PositionComponent():
	isMovingUp(false), 
	isMovingDown(false),
	isMovingRight(false),
	isMovingLeft(false),
	speed(0)
{

}


PositionComponent::~PositionComponent()
{
}

void PositionComponent::update(sf::Time elapsedTime, Entity* entity, Map* map)
{
	
}

ComponentType PositionComponent::getType()
{
	return ComponentType::POSITION;
}

void PositionComponent::setMovementDirection(sf::Vector2f direction)
{
	this->movement = sf::Vector2f(direction.x * speed, direction.y * speed);
	if (movement.y > 0)
	{
		isMovingDown = true;
	}else if(movement.y < 0 )
	{
		isMovingUp = true;
	}else
	{
		isMovingUp = false;
		isMovingDown = false;
	}

	if (movement.x > 0)
	{
		isMovingRight = true;
	}else if(movement.x < 0 )
	{
		isMovingLeft = true;
	}else
	{
		isMovingLeft = false;
		isMovingRight = false;
	}
}
