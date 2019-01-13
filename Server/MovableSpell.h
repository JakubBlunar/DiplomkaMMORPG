#ifndef S_MOVING_SPELL_H
#define S_MOVING_SPELL_H

#include "Spell.h"
#include "EntityPosition.h"

namespace s {
	class MovableSpell : public Entity, public Spell {
	public:
		MovableSpell();
		virtual ~MovableSpell();

		EntityPosition position;

		void update(sf::Time elapsedTime, s::Server* s, Map* map);
		b2Body* getBody() const override;
		Spell* clone() const override;
		void loadFromJson(json data) override;
	};
}
#endif
