#include "PositionComponent.h"
#include "Globals.h"
#include <Box2D/Box2D.h>
#include "Entity.h"

PositionComponent::PositionComponent():
	isMovingUp(false),
	isMovingDown(false),
	isMovingRight(false),
	isMovingLeft(false),
	speed(0),
	size(sf::Vector2f(0, 0)) {
}


PositionComponent::~PositionComponent() {
}

void PositionComponent::update(sf::Time elapsedTime, Entity* entity, Map* map) {
	b2Vec2 position = entity->getBody()->GetPosition();
	this->position = sf::Vector2f(ceilNumber(position.x * METTOPIX), ceilNumber(position.y * METTOPIX));
}

ComponentType PositionComponent::getType() {
	return ComponentType::POSITION;
}

void PositionComponent::setMovementDirection(sf::Vector2f direction) {
	this->movement = sf::Vector2f(direction.x * speed, direction.y * speed);
	if (movement.y > 0) {
		isMovingDown = true;
	}
	else if (movement.y < 0) {
		isMovingUp = true;
	}
	else {
		isMovingUp = false;
		isMovingDown = false;
	}

	if (movement.x > 0) {
		isMovingRight = true;
	}
	else if (movement.x < 0) {
		isMovingLeft = true;
	}
	else {
		isMovingLeft = false;
		isMovingRight = false;
	}
}

void PositionComponent::setPosition(sf::Vector2f position) {
	this->position = position;
}

sf::Vector2f PositionComponent::getPosition() const {
	return position;
}

sf::Vector2f PositionComponent::getMovement() const {
	return movement;
}

void PositionComponent::setMovement(sf::Vector2f movement)
{
	this->movement = movement;
	if (movement.y > 0) {
		isMovingDown = true;
	}
	else if (movement.y < 0) {
		isMovingUp = true;
	}
	else {
		isMovingUp = false;
		isMovingDown = false;
	}

	if (movement.x > 0) {
		isMovingRight = true;
	}
	else if (movement.x < 0) {
		isMovingLeft = true;
	}
	else {
		isMovingLeft = false;
		isMovingRight = false;
	}
}

void PositionComponent::setSpeed(float speed) {
	this->speed = speed;
}

float PositionComponent::getSpeed() const {
	return speed;
}

sf::Vector2f PositionComponent::getSize() const {
	return size;
}

void PositionComponent::setSize(sf::Vector2f size) {
	this->size = size;
}

void PositionComponent::setBodyType(BodyType type) {
	this->bodyType = type;
}

BodyType PositionComponent::getBodyType() const {
	return bodyType;
}
