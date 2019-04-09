#ifndef S_ENTITY_TO_ENTITY_RAY_CAST_H
#define S_ENTITY_TO_ENTITY_RAY_CAST_H

#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include "Entity.h"

namespace s {
	class EntityPosition;

	class EntityToEntityRayCast : public b2RayCastCallback {
	public:
		Entity* startEntity;
		Entity* endEntity;
		bool canSee;
		float closestEntityDistance;

		EntityToEntityRayCast(Entity* startEntity, Entity* endEntity);
		virtual ~EntityToEntityRayCast();
		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
			const b2Vec2& normal, float32 fraction) override;
	};
}
#endif
