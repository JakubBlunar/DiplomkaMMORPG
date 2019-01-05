#ifndef S_MOVING_SPELL_H
#define S_MOVING_SPELL_H

#include "Spell.h"
#include "EntityPosition.h"

namespace s {
	class MovableSpell : public Entity, public EntityPosition, public Spell {
	public:
		MovableSpell();
		~MovableSpell();

		void update(sf::Time elapsedTime, s::Server* s, Map* map);
		void cast(Entity* entity) override;
	};
}
#endif
