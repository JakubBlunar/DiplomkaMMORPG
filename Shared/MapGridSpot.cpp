#include "stdafx.h"

#include "MapGrid.h"
#include "MapGridSpot.h"


MapGridSpot::MapGridSpot(int id, int x, int y, int positionX, int positionY) : wall(false), previous(nullptr), g(0), f(0),
h(0) {
	this->id = id;
	this->x = x;
	this->y = y;
	this->positionX = positionX;
	this->positionY = positionY;
	neighbours.clear();
}


MapGridSpot::~MapGridSpot()
{
}

void MapGridSpot::setWall(bool wall)
{
	this->wall = wall;
}

bool MapGridSpot::isWall() const {
	return wall;
}

void MapGridSpot::reset() {
	g = 0;
	f = 0;
	h = 0;
	previous = nullptr;
}

void MapGridSpot::initNeighbours(MapGrid* mapGrid) {
	neighbours.clear();

	int cols = mapGrid->getNumCols();
	int rows = mapGrid->getNumRows();
	auto grid = mapGrid->grid;

	if (x < cols - 1) {
		neighbours.push_back(grid->get(x + 1, y));
	}
	if (x > 0) {
		neighbours.push_back(grid->get(x - 1, y));
	}
	if (y < rows - 1) {
		neighbours.push_back(grid->get(x, y + 1));
	}
	if (y > 0) {
		neighbours.push_back(grid->get(x, y - 1));
	}

	if (x > 0 && y > 0 && !(grid->get(x - 1, y)->isWall() && grid->get(x, y - 1)->isWall())) {
		neighbours.push_back(grid->get(x - 1, y - 1));
	}
	if (x > 0 && y < rows - 1 && !(grid->get(x, y + 1)->isWall() && grid->get(x - 1, y)->isWall())) {
		neighbours.push_back(grid->get(x - 1, y + 1));
	}
	if (x < cols - 1 && y > 0 && !(grid->get(x, y - 1)->isWall() && grid->get(x + 1, y)->isWall())) {
		neighbours.push_back(grid->get(x + 1, y - 1));
	}
	if (x < cols - 1 && y < rows - 1 && !(grid->get(x, y + 1) - isWall() && grid->get(x + 1, y)->isWall())) {
		neighbours.push_back(grid->get(x + 1, y + 1));
	}
}
