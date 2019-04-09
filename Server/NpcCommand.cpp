#include "NpcCommand.h"
#include "Npc.h"
#include "../Server/EventDispatcher.h"
#include "NpcEventNpcIsIdle.h"


s::NpcCommand::NpcCommand() : finished(false) {
	maxDuration = sf::seconds(1000000);
	finishedEventDispatched = false;
	server = nullptr;
}


s::NpcCommand::~NpcCommand()
{

}

bool s::NpcCommand::isFinished() const
{
	return finished;
}

void s::NpcCommand::update(sf::Time elapsedTime, NpcUpdateEvents * npcUpdateEvents)
{
	if (finished) {
		return;
	}

	duration += elapsedTime;
	if (duration > maxDuration) {
		finished = true;
	}
}

void s::NpcCommand::dispatchFinishEvent(NpcEvent* e) {
	if (finishedEventDispatched) {
		return;
	}

	finishedEventDispatched = true;
	EventId eventId = e->getId();
	switch (eventId) {
		case NPC_IS_IDLE: {
			EventDispatcher<NpcEventNpcIsIdle>::dispatchEvent((NpcEventNpcIsIdle*)e, server);
		}
		default:
			break;
	}
}

s::NpcCommand::NpcCommand(sf::Time maxDuration, Server* s) : finished(false) {
	this->maxDuration = maxDuration;
	server = s;
	finishedEventDispatched = false;
}
