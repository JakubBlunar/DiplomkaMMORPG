#include "Entity.h"
#include "PositionComponent.h"


Entity::Entity(sf::Uint32 id): body(nullptr) {
	this->id = id;
}

Entity::~Entity() {
}

void Entity::update(sf::Time elapsedTime, Map* map, Game* g) {
	for (Component* component : components) {
		component->update(elapsedTime, this, map);
	}
}

Component* Entity::getComponent(ComponentType type) const {
	for (Component* const component : components) {
		if (component->getType() == type) {
			return component;
		}
	}
	return nullptr;
}

void Entity::setBody(b2Body* body) {
	this->body = body;
	body->SetUserData(this);
}

b2Body* Entity::getBody() {
	return body;
}

sf::Vector2f Entity::getPosition() const {
	PositionComponent* positionComponent = (PositionComponent*)getComponent(ComponentType::POSITION);
	if (positionComponent != nullptr) {
		return positionComponent->getPosition();
	}
	return sf::Vector2f(-10000, -10000);
}

sf::Vector2f Entity::getSize() const {
	PositionComponent* positionComponent = (PositionComponent*)getComponent(ComponentType::POSITION);
	if (positionComponent != nullptr) {
		return positionComponent->getSize();
	}
	return sf::Vector2f(0, 0);
}

sf::Uint32 Entity::getId() const {
	return id;
}
