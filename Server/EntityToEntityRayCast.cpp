#include "EntityToEntityRayCast.h"

#include <Box2D/Box2D.h>
#include "MovableSpell.h"
#include "Npc.h"


s::EntityToEntityRayCast::
EntityToEntityRayCast(Entity* startEntity, Entity* endEntity):
	closestEntityDistance(999999999.f) {
	this->startEntity = startEntity;
	this->endEntity = endEntity;
	canSee = false;
}


s::EntityToEntityRayCast::~EntityToEntityRayCast() {}

float32 s::EntityToEntityRayCast::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal,
                                                float32 fraction) {
	b2Body* body = fixture->GetBody();

	if (fraction < 0) {
		return 1;
	}

	void* userData = body->GetUserData();
	if (userData) {
		Entity* hitObj = static_cast<Entity*>(userData);
		if (hitObj->getEntityType() == EntityType::MOVABLE_SPELL || hitObj != endEntity) { // if not target then continue
			return 1;
		}
	}

	float distance = b2Distance(body->GetPosition(), startEntity->getBody()->GetPosition());
	if (closestEntityDistance < distance) {
		return 1;
	}

	closestEntityDistance = distance;
	canSee = userData != nullptr;

	return 1.0f;
}
