#ifndef ASTAR
#define ASTAR
#include "MapGrid.h"
#include <map>

class Astar
{
	int step();
	float heuristic(MapGridSpot* a, MapGridSpot* b) const;

	std::map<int, MapGridSpot*> openSet;
	std::map<int, MapGridSpot*> closedSet;

	MapGridSpot* current;
	MapGridSpot* start;
	MapGridSpot* end;

	sf::Vector2f from;
	sf::Vector2f to;

	MapGrid* grid;

public:
	Astar(MapGrid* grid);
	~Astar();
	std::list<sf::Vector2f> path;
	int findPath(sf::Vector2f from, sf::Vector2f to);
};

#endif
