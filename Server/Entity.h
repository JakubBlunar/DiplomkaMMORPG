#ifndef S_ENTITY_H
#define S_ENTITY_H

#include "../Shared/EntityConstants.h"
#include <Box2D/Dynamics/b2Body.h>

namespace s {

	class Entity {
	protected:
		Entity(): entityType() {} ;
		EntityType entityType;

		sf::Mutex mutex;
	public:
		EntityType getEntityType() const {
			return entityType;
		}

		void lock() { mutex.lock(); }
		void unlock() { mutex.lock(); }

		virtual b2Body* getBody() const = 0;
	};
}

#endif
