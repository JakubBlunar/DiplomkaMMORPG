#include "NpcCommandStay.h"
#include "Npc.h"


s::NpcCommandStay::NpcCommandStay(Npc* npc, Map* m, Server* s, sf::Time duration): NpcCommand(duration) {
	this->npc = npc;
	this->map = m;
	this->server = s;
	inited = false;
}

s::NpcCommandStay::~NpcCommandStay()
{

}

void s::NpcCommandStay::update(sf::Time elapsedTime)
{
	NpcCommand::update(elapsedTime);

	if (!inited) {
		npc->setMovementDirection(sf::Vector2f(0, 0), npc->getSpeed());
		inited = true;
	}
}
