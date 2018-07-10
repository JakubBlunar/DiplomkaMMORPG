#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "PositionComponent.h"

class Player :
	public Entity
{
public:
	Player(bool playerControlled);
	~Player();

	void handleEvent(GameEvent* event) override;
	void update(sf::Time elapsedTime, Map* map) override;

private:
	bool playerControlled;
	PositionComponent* positionComponent;
};

#endif