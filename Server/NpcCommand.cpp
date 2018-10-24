#include "NpcCommand.h"
#include "Npc.h"


s::NpcCommand::NpcCommand(): finished(false) {
	maxDuration = sf::seconds(1000000);
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

s::NpcCommand::NpcCommand(sf::Time maxDuration): finished(false) {
	this->maxDuration = maxDuration;
}
