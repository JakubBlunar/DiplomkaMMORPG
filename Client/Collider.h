#ifndef COLLIDER_H
#define COLLIDER_H

#include "Entity.h"
#include "PositionComponent.h"

class Collider : public Entity {
public:
	explicit Collider(sf::Uint32 id);
	~Collider();

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map, Game* g) override;
	EntityType getType() override;
	EntityCategory getEntityCategory() override;

	PositionComponent* getPositionComponent() const;
	uint16 getCollisionMask() override;
private:
	PositionComponent* positionComponent;
};

#endif
