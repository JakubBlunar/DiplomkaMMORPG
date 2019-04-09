#include "NpcCommandStay.h"
#include "Npc.h"
#include "NpcEventNpcIsIdle.h"


s::NpcCommandStay::NpcCommandStay(Npc* npc, Map* m, Server* s, sf::Time duration) : NpcCommand(duration, s) {
	this->npc = npc;
	this->map = m;
	this->server = s;
	inited = false;
}

s::NpcCommandStay::~NpcCommandStay()
{

}

void s::NpcCommandStay::update(sf::Time elapsedTime, NpcUpdateEvents * npcUpdateEvents)
{
	NpcCommand::update(elapsedTime, npcUpdateEvents);

	if (finished) {
		if (!finishedEventDispatched) {
			NpcEventNpcIsIdle* e = new NpcEventNpcIsIdle(npc);
			dispatchFinishEvent(e);
		}
		return;
	}

	if (!inited) {
		npc->setMovementDirection(sf::Vector2f(0, 0), npc->attributes.getAttribute(EntityAttributeType::SPEED, true), npcUpdateEvents);
		inited = true;
	}
}
