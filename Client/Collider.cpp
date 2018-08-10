#include "Collider.h"

Collider::Collider(long long id): Entity(id)
{
	positionComponent = new PositionComponent();
	components.push_back(positionComponent);
}

Collider::~Collider()
{
}

void Collider::handleEvent(GameEvent* event)
{
}

void Collider::update(sf::Time elapsedTime, Map* map)
{
	Entity::update(elapsedTime, map);
}

EntityType Collider::getType()
{
	return EntityType::COLLIDER;
}

EntityCategory Collider::getEntityCategory()
{
	return EntityCategory::BOUNDARY;
}

PositionComponent* Collider::getPositionComponent() const
{
	return positionComponent;
}
