#pragma once
#include "Entity.h"
#include "PositionComponent.h"
#include "RenderComponent.h"
#include "AttributesComponent.h"

class GameObject :
	public Entity {
public:

	GameObject(sf::Uint32 id, std::string file);
	~GameObject();

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map, Game* g) override;
	void loadFromJson(const std::string& file);

	EntityType getType() override;

	RenderComponent* getRenderComponent() const;
	PositionComponent* getPositionComponent() const;
	EntityCategory getEntityCategory() override;
	uint16 getCollisionMask() override;
private:
	PositionComponent* positionComponent;
	RenderComponent* renderComponent;
	AttributesComponent* attributesComponent;
};
