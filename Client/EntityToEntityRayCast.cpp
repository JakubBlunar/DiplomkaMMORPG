#include "EntityToEntityRayCast.h"
#include <Box2D/Box2D.h>
#include "Entity.h"


EntityToEntityRayCast::EntityToEntityRayCast(Entity* startEntity, Entity* endEntity): closestEntity(nullptr), closestEntityDistance(999999999.f) {
	this->startEntity = startEntity;
	this->endEntity = endEntity;
}


EntityToEntityRayCast::~EntityToEntityRayCast()
{
}

float32 EntityToEntityRayCast::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal,
	float32 fraction) {
		b2Body* body = fixture->GetBody();
		Entity* hitObj = static_cast<Entity*>(body->GetUserData());

	    if (!hitObj || fraction < 0) {
	        return 1;
	    }

		EntityType type = hitObj->getType();
	    if (type == EntityType::SPELL) {
		    return 1;
	    }

		if (type == EntityType::NPC || type == EntityType::PLAYER) {
			if (hitObj == startEntity) {
				return 1;  
			}
			if(hitObj != endEntity) {
				return 1;
			}
		}

		float distance = b2Distance(body->GetPosition(), startEntity->getBody()->GetPosition());
		if (closestEntityDistance < distance) {
			return 1;
		}

		closestEntityDistance = distance;
		closestEntity = hitObj;

		return 1.0f;
	}
