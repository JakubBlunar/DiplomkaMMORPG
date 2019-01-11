#include "MovableSpell.h"
#include "ServerGlobals.h"
#include "Box2D/Box2D.h"
#include "EntityPosition.h"

s::MovableSpell::MovableSpell()
{
	entityType = EntityType::MOVABLE_SPELL;
}


s::MovableSpell::~MovableSpell()
{
}

void s::MovableSpell::update(sf::Time elapsedTime, s::Server* s, Map* map) {
	float speed = 100.f;

	if (target && position.getBody()) {
		EntityPosition* epTarget = dynamic_cast<EntityPosition*>(target);
		if (epTarget) {
			sf::Vector2f nextMovement;
			sf::Vector2f actualPosition = position.getPosition();
			sf::Vector2f targetPosition = epTarget->getPosition();
			sf::Vector2f targetMovement = epTarget->getLastMovement();

			float diffX = targetPosition.x - actualPosition.x;
			float diffY = targetPosition.y - actualPosition.y;
			float aspectRatio = abs(diffX / diffY);

			if (abs(diffX) > 10) {	
				if (diffX > 0) 
					nextMovement.x = speed * aspectRatio;
				else
					nextMovement.x = -speed * aspectRatio;	
			} else {
				nextMovement.x = targetMovement.x;
			}

			if (abs(diffY) > 10) {
				if (diffY > 0) 
					nextMovement.y = speed;
				else
					nextMovement.y = -speed;
			} else {
				nextMovement.y = targetMovement.y;
			}
		
			position.getBody()->SetLinearVelocity(b2Vec2(nextMovement.x * PIXTOMET, nextMovement.y * PIXTOMET));
			position.setMovement(nextMovement);
		}
		
	}
}

void s::MovableSpell::cast(Entity* entity) {
	
}
