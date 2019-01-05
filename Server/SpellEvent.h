#ifndef S_SPELL_EVENT_H
#define S_SPELL_EVENT_H

#include <SFML/System.hpp>

namespace s {
	struct SpellInfo;
	class Server;

	class SpellEvent {
	public:
		SpellEvent();
		virtual ~SpellEvent();
		
		sf::Time executeTime;
		SpellInfo* spellInfo;

		virtual void execute(Server* s) = 0;
	};
}

#endif