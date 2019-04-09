#ifndef BOT_COMMAND_H
#define BOT_COMMAND_H

#include <SFML/System/Time.hpp>

class BotGame;

class BotCommand
{
public:
	BotCommand(sf::Time maxDuration);
	~BotCommand();

	sf::Time maxDuration;
	sf::Time duration;

	bool finished;

	bool isFinished() const;
	virtual void update(sf::Time elapsedTime, BotGame* g);

};

#endif
