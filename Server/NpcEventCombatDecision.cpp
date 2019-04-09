#include "NpcEventCombatDecision.h"
#include "Npc.h"


s::NpcEventCombatDecision::NpcEventCombatDecision(Npc* npc)
{
	id = NPC_COMBAT_DECIDE;
	this->npc = npc;
	npc->setRunningNpcEvent(this);
}


s::NpcEventCombatDecision::~NpcEventCombatDecision()
{
}
