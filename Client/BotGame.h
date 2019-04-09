#ifndef BOT_GAME_H
#define BOT_GAME_H

#include "Game.h"
#include "BotCommand.h"

class BotGame : public Game
{
public:
	BotGame();
	~BotGame();
	void run() override;
private:
	void update(sf::Time elapsedTime) override;
	void render() override;
	void afterStart() override;

	void botUpdatePlayer();

	BotCommand* botCommand;
};

#endif
