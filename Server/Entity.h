#ifndef S_ENTITY_H
#define S_ENTITY_H
#include "../Shared/EntityConstants.h"


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
	};
}

#endif