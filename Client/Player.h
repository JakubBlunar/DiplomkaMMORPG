#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "PositionComponent.h"
#include "../Client/json.hpp"
#include "RenderComponent.h"

using json = nlohmann::json;

class Player :
	public Entity
{
public:
	Player(bool playerControlled);
	~Player();

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map) override;

	EntityType getType() override;
	EntityCategory getEntityCategory() override;

	void loadFromJson(json jsonData);
private:
	bool playerControlled;
	PositionComponent* positionComponent;
	RenderComponent* renderComponent;

	sf::Vector2f lastMovement;
};

#endif