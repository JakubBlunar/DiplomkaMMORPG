#pragma once
#include "NpcEvent.h"

namespace s {
	class NpcEventNpcIsIdle :
		public NpcEvent
	{
	public:
		NpcEventNpcIsIdle(Npc* npc);
		~NpcEventNpcIsIdle();
	};

}
