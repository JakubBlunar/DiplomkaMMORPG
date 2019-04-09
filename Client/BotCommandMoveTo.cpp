#include "BotCommandMoveTo.h"
#include <SFML/System/Vector2.hpp>
#include "MapGrid.h"
#include "Map.h"
#include "Player.h"
#include "Astar.h"
#include "BotGame.h"
#include "Globals.h"

BotCommandMoveTo::BotCommandMoveTo(sf::Vector2f position, Player* p, Map* m, BotGame *g, sf::Time maxDuration) : BotCommand(maxDuration)
{
	endPosition = position;
	player = p;
	map = m;
	game = g;
}


BotCommandMoveTo::~BotCommandMoveTo()
{
}

void BotCommandMoveTo::update(sf::Time elapsedTime, BotGame* g) {
	BotCommand::update(elapsedTime, g);

	if (finished) {
		return;
	}

	b2Vec2 pos = player->getBody()->GetPosition();
	sf::Vector2f start = sf::Vector2f(pos.x * METTOPIX, pos.y * METTOPIX);

	if (!path.empty()) {
		sf::Vector2f finish = path.front();
		float distance = sqrt(pow(start.x - finish.x, 2) + pow(start.y - finish.y, 2));

		if (distance < 25) {
			path.pop_front();
			return;
		}

		sf::Vector2f direction = sf::Vector2f(0, 0);

		if (start.x < finish.x && (finish.x - start.x) > 8) {
			direction.x = 1;
		}

		if (start.x > finish.x && (start.x - finish.x) > 8) {
			direction.x = -1;
		}

		if (start.y < finish.y && (finish.y - start.y) > 8) {
			direction.y = 1;
		}

		if (start.y > finish.y && (start.y - finish.y) > 8) {
			direction.y = -1;
		}

		player->setMovementDirection(direction, g);
	}
	else {
		if (!finished)
			finished = true;

		player->setMovementDirection(sf::Vector2f(0, 0), g);
	}


}

void BotCommandMoveTo::init()
{
	game->print("Bot moving player into:  " + std::to_string(endPosition.x) + "," + std::to_string(endPosition.y));
	MapGrid* mapGrid = map->getGrid();

	sf::Vector2f pathFindingFrom = player->getPosition();

	Astar astar(mapGrid);
	int res = astar.findPath(pathFindingFrom, endPosition);
	if (res == 1) {
		path = astar.path;
	}
	else {
		finished = true;
	}
}
