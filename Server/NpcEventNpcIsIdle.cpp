#include "NpcEventNpcIsIdle.h"
#include "Npc.h"


s::NpcEventNpcIsIdle::NpcEventNpcIsIdle(Npc* npc)
{
	id = NPC_IS_IDLE;
	this->npc = npc;
	npc->setRunningNpcEvent(this);
}


s::NpcEventNpcIsIdle::~NpcEventNpcIsIdle()
{
}
