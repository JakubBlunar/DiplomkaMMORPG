#ifndef BOT_COMMAND_MOVE_TO_H
#define BOT_COMMAND_MOVE_TO_H

#include "BotCommand.h"
#include <SFML/System/Vector2.hpp>
#include <list>

class Player;
class Map;
class BotGame;

class BotCommandMoveTo : public BotCommand
{
	std::list<sf::Vector2f> path;
	sf::Vector2f endPosition;
	Player* player;
	Map* map;
	BotGame* game;
public:
	BotCommandMoveTo(sf::Vector2f position, Player*p, Map* m, BotGame *g, sf::Time maxDuration);
	~BotCommandMoveTo();

	void update(sf::Time elapsedTime, BotGame* g) override;

	void init();
};

#endif
