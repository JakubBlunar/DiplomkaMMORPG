#include "BotCommand.h"



BotCommand::BotCommand(sf::Time maxDuration)
{
	this->maxDuration = maxDuration;
	duration = sf::seconds(0);
	finished = false;
}


BotCommand::~BotCommand()
{
}

bool BotCommand::isFinished() const {
	return finished;
}

void BotCommand::update(sf::Time elapsedTime, BotGame* g)
{
	if (finished) {
		return;
	}


	duration += elapsedTime;
	if (duration > maxDuration) {
		finished = true;
	}
}
