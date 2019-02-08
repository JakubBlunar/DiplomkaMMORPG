#ifndef S_MOVING_SPELL_H
#define S_MOVING_SPELL_H

#include "Spell.h"
#include "EntityPosition.h"
#include "json.hpp"
#include <Box2D/Dynamics/b2Body.h>

namespace sf {
	class Time;
}

using json = nlohmann::json;

namespace s {
	class MovableSpell : public Entity, public Spell {
	public:
		MovableSpell();
		virtual ~MovableSpell();

		EntityPosition position;

		float speed;

		void update(sf::Time elapsedTime, s::Server* s, Map* map);
		b2Body* getBody() const override;
		Spell* clone() const override;
		void loadFromJson(json data) override;

		void setSpeed(float speed) {
			this->speed = speed;
		}

		float getSpeed() const {
			return speed;
		}

		sf::Vector2f getPosition() const override;
	};
}
#endif
