#ifndef S_CHARACTER_MANAGER_H
#define S_CHARACTER_MANAGER_H

#include "Manager.h"
#include <vector>
class EventLearnSpell;
class EventIncreaseCharacterAttribute;

namespace s {
	class Npc;
	class Character;
	class Session;
	struct SpellInfo;
	

	class CharacterManager :
		public Manager {
		Server* server;
	public:
		CharacterManager();
		virtual ~CharacterManager();
		void init(Server* server);

		void handleEvent(EventIncreaseCharacterAttribute* event, Session* playerSession, Server* s) const;
		void handleEvent(EventLearnSpell* event, Session* playerSession, Server* s) const;
		void handleNpcKill(Character* character, Npc* npc) const;

		std::vector<SpellInfo*>* getFreeSpellsForLearn(Character* character) const;

	};

}

#endif