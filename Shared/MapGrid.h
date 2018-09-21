#ifndef MAP_GRID_H
#define MAP_GRID_H

#include "Matrix.h"
#include "MapGridSpot.h"
#include <SFML/Graphics/Rect.hpp>

class Camera;

class MapGrid
{
	int rows, cols, width, height;
	int positionMargin = 24;

	int nextSpotId = 0;
public:
	MapGrid(int width, int height);
	~MapGrid();


	Matrix<MapGridSpot>* grid;

	int getNumRows() const;
	int getNumCols() const;

	int transformXToGrid(float worldCoord) const;
	int transformYToGrid(float worldCoord) const;

	void setWall(sf::Vector2f worldPosition, sf::Vector2f size);
	
	void initNeighbours();
	void reset();
};

#endif