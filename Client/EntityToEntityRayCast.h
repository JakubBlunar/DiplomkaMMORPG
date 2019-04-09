#ifndef ENTITY_TO_ENTITY_RAY_CAST_H
#define ENTITY_TO_ENTITY_RAY_CAST_H

#include <Box2D/Dynamics/b2WorldCallbacks.h>

class Entity;

class EntityToEntityRayCast : public b2RayCastCallback {
public:
	Entity* startEntity;
	Entity* closestEntity;
	Entity* endEntity;
	float closestEntityDistance;

	EntityToEntityRayCast(Entity* startEntity, Entity* endEntity);
	virtual ~EntityToEntityRayCast();
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction) override;
};

#endif
