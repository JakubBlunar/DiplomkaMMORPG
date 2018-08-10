#ifndef COLLIDER_H
#define COLLIDER_H

#include "Entity.h"
#include "PositionComponent.h"

class Collider: public Entity
{
public:
	explicit Collider(long long id);
	~Collider();

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map);
	EntityType getType() override;
	EntityCategory getEntityCategory() override;

	PositionComponent* getPositionComponent() const;
private:
	PositionComponent* positionComponent;
};

#endif
