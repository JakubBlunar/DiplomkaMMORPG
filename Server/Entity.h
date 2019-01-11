#ifndef S_ENTITY_H
#define S_ENTITY_H

#include "../Shared/EntityConstants.h"
#include <Box2D/Dynamics/b2Body.h>

namespace s {

	class Entity {
	protected:
		Entity(): entityType() {} ;
		virtual ~Entity() = default;

		EntityType entityType;

	public:
		EntityType getEntityType() const {
			return entityType;
		}

		virtual b2Body*  getBody() const = 0;
	};
}

#endif