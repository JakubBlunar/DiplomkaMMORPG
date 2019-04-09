#ifndef NPC_EVENT_H
#define NPC_EVENT_H

#include "../Shared/GameEvent.h"

namespace  s {
	class Npc;

	class NpcEvent :
		public GameEvent
	{
	public:
		NpcEvent();
		~NpcEvent();

		bool loadFromPacket(sf::Packet* p) override;
		sf::Packet* toPacket() override;
		Npc* npc;
	};
}

#endif
