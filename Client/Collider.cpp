#include "Collider.h"

Collider::Collider(long long id): Entity(id) {
	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
}

Collider::~Collider() {
}

void Collider::handleEvent(GameEvent* event) {
}

void Collider::update(sf::Time elapsedTime, Map* map, Game* g) {
	Entity::update(elapsedTime, map, g);
}

EntityType Collider::getType() {
	return EntityType::COLLIDER;
}

EntityCategory Collider::getEntityCategory() {
	return EntityCategory::BOUNDARY;
}

PositionComponent* Collider::getPositionComponent() const {
	return positionComponent;
}

uint16 Collider::getCollisionMask() {
	return PLAYER | ENEMY_PLAYER;
}
