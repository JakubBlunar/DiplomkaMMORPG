#ifndef MAP_GRID_SPOT_H
#define MAP_GRID_SPOT_H
#include <list>

class MapGrid;

class MapGridSpot
{
	bool wall;
public:
	MapGridSpot(int id, int x, int y, int positionX, int positionY);
	~MapGridSpot();
	int x, y, id, positionX, positionY;

	MapGridSpot* previous;
	std::list<MapGridSpot*> neighbours;
	float g, f, h;
	void setWall(bool wall);
	bool isWall() const;

	void reset();
	void initNeighbours(MapGrid* mapGrid);
};

#endif
