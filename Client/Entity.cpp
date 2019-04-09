#include "Entity.h"
#include "PositionComponent.h"
#include "RenderComponent.h"

Entity::Entity(sf::Uint32 id) : body(nullptr) {
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
	if (this->body) {
		this->body->SetUserData(this);
	}
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

bool Entity::containsPoint(sf::Vector2f point) const
{
	RenderComponent* renderComponent = (RenderComponent*)getComponent(ComponentType::RENDER);
	PositionComponent* positionComponent = (PositionComponent*)getComponent(ComponentType::POSITION);
	if (renderComponent && positionComponent) {
		sf::Vector2f position = positionComponent->getPosition();
		sf::Vector2i offset = renderComponent->getOffset();
		sf::Vector2i size = renderComponent->getSize();

		sf::Vector2f renderOffset = sf::Vector2f(position.x + offset.x, position.y + offset.y);
		if (renderOffset.x <= point.x && point.x <= renderOffset.x + size.x) {
			if (renderOffset.y <= point.y && point.y <= renderOffset.y + size.y) {
				return true;
			}
		}
	}
	return false;
}

std::string Entity::getName() const {
	return name;
}
