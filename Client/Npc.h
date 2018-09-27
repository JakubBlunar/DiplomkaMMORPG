#ifndef NPC_H
#define NPC_H

#include "Entity.h"
#include "RenderComponent.h"
#include "PositionComponent.h"

class Npc :
	public Entity
{
	int type;

	PositionComponent* positionComponent;
	RenderComponent* renderComponent;

public:
	Npc(sf::Uint32 spawnId, int type);
	~Npc();

	void handleEvent(GameEvent* event) override;
	EntityType getType() override;
	EntityCategory getEntityCategory() override;
	uint16 getCollisionMask() override;

	void update(sf::Time elapsedTime, Map* map, Game* g) override;

	RenderComponent* getRenderComponent() const;
	PositionComponent* getPositionComponent() const;

	void updateMovementAnimation();
};

#endif