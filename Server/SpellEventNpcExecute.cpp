#include "SpellEventNpcExecute.h"
#include "Npc.h"


s::SpellEventNpcExecute::SpellEventNpcExecute()
{
}


s::SpellEventNpcExecute::~SpellEventNpcExecute()
{
}

void s::SpellEventNpcExecute::setNpc(Npc* npc) {
	this->npc = npc;
}
void s::SpellEventNpcExecute::execute(Server* s) {
	npc->setCastingSpell(nullptr);

	if(!npc->isAlive()) return;


}
