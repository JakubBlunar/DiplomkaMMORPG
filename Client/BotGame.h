#ifndef BOT_GAME_H
#define BOT_GAME_H

#include "../Client/Game.h"

class BotGame: public Game
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
};

#endif
