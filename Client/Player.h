#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "PositionComponent.h"
#include "../Client/json.hpp"
#include "RenderComponent.h"

using json = nlohmann::json;

class Player :
	public Entity {
public:
	Player(bool playerControlled);
	~Player();

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map, Game* g) override;

	EntityType getType() override;
	EntityCategory getEntityCategory() override;
	uint16 getCollisionMask() override;

	void updateMovementAnimation();

	void loadFromJson(json jsonData);
	
private:
	bool playerControlled;
	PositionComponent* positionComponent;
	RenderComponent* renderComponent;

	sf::Vector2f lastMovement;
};

#endif
