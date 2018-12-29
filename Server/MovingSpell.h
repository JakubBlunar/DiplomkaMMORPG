#ifndef S_MOVING_SPELL_H
#define S_MOVING_SPELL_H

#include "Spell.h"

namespace s {
	class MovableSpell : public Spell {
	public:
		MovableSpell();
		~MovableSpell();
	};
}
#endif
