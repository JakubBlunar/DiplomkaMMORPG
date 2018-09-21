#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "PositionComponent.h"
#include "../Client/json.hpp"
#include "RenderComponent.h"
#include <list>

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

	b2Fixture* getMelleView() const;
	void setMelleView(b2Fixture* fixture);

	b2Fixture* getMelleRange() const;
	void setMelleRange(b2Fixture* fixture);

	std::list<Entity*> melleRangeEntities;
	std::list<Entity*> melleViewEntities;
	bool isControlledByPlayer() const;
private:
	bool playerControlled;
	PositionComponent* positionComponent;
	RenderComponent* renderComponent;

	b2Fixture* melleView;
	b2Fixture* melleRange;

	sf::Vector2f lastMovement;
};

#endif
