#ifndef NPC_H
#define NPC_H

#include "Entity.h"
#include "JsonLoader.h"
#include "../Shared/NpcConstants.h"

class GameEvent;
class AttributesComponent;
class PositionComponent;
class RenderComponent;

class Npc :
	public Entity
{
	int type;
	NpcState npcState;

	PositionComponent* positionComponent;
	RenderComponent* renderComponent;
	AttributesComponent* attributesComponent;

	MovementData lastServerPosition;
public:
	Npc();
	~Npc();

	void handleEvent(GameEvent* event) override;
	EntityType getType() override;
	EntityCategory getEntityCategory() override;
	uint16 getCollisionMask() override;

	void update(sf::Time elapsedTime, Map* map, Game* g) override;

	RenderComponent* getRenderComponent() const;
	PositionComponent* getPositionComponent() const;
	AttributesComponent* getAttributesComponent() const;

	void updateMovementAnimation();

	void loadFromJson(json serverData);

	NpcState getState() const;

	void subscribe();
	void unsubscribe();
};

#endif