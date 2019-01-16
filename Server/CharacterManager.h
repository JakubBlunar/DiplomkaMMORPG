#ifndef S_CHARACTER_MANAGER_H
#define S_CHARACTER_MANAGER_H

#include "Manager.h"
#include "EventIncreaseCharacterAttribute.h"

namespace s {
	class Npc;
	class Character;
	class Session;

	class CharacterManager :
		public Manager {
		Server* server;
	public:
		CharacterManager();
		virtual ~CharacterManager();
		void init(Server* server);

		void handleEvent(EventIncreaseCharacterAttribute* event, Session* playerSession, Server* s) const;
		void handleNpcKill(Character* character, Npc* npc) const;
		
	};

}

#endif