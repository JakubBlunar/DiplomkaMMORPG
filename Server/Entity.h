#ifndef S_ENTITY_H
#define S_ENTITY_H

#include "../Shared/EntityConstants.h"
#include <Box2D/Dynamics/b2Body.h>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Vector2.hpp>

namespace s {

	class Entity {
	protected:
		Entity() : entityType() {};
		EntityType entityType;

		sf::Mutex mutex;
	public:
		EntityType getEntityType() const {
			return entityType;
		}

		void lock() { mutex.lock(); }
		void unlock() { mutex.lock(); }

		virtual sf::Vector2f getPosition() const = 0;
		virtual b2Body* getBody() const = 0;
	};
}

#endif
