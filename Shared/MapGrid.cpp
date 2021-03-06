﻿#include "stdafx.h"
#include "MapGrid.h"

MapGrid::MapGrid(int width, int height)
{
	this->width = width;
	this->height = height;

	cols = (width - positionMargin) / positionMargin;
	rows = (height - positionMargin) / positionMargin;

	grid = new Matrix<MapGridSpot>(cols, rows);

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			grid->set(i, j, new MapGridSpot(nextSpotId, i, j, positionMargin + i * positionMargin, positionMargin + j * positionMargin));
			nextSpotId++;
		}
	}
}


MapGrid::~MapGrid()
{
}

int MapGrid::getNumRows() const {
	return rows;
}

int MapGrid::getNumCols() const {
	return cols;
}

int MapGrid::transformXToGrid(float worldCoord) const {
	int coord = (int)(worldCoord) / positionMargin;
	if (coord >= cols) {
		return cols - 1;
	}
	return coord;
}

int MapGrid::transformYToGrid(float worldCoord) const
{
	int coord = (int)(worldCoord) / positionMargin;
	if (coord >= rows) {
		return rows - 1;
	}
	return coord;
}


void MapGrid::setWall(sf::Vector2f worldPosition, sf::Vector2f size) const {
	int fromX = this->transformXToGrid(worldPosition.x);
	int fromY = this->transformYToGrid(worldPosition.y);

	int toX = (int)ceil(this->transformXToGrid(worldPosition.x + size.x + 6));
	int toY = (int)ceil(this->transformYToGrid(worldPosition.y + size.y + 6));

	if (fromX < 0) {
		return;
	}

	if (fromY < 0) {
		return;
	}

	for (int x = fromX; x <= toX; x++) {
		for (int y = fromY; y <= toY; y++) {
			MapGridSpot * spot = grid->get(x, y);
			spot->setWall(true);
		}
	}
}

void MapGrid::setWall(sf::Vector2i coords) const {
	if (coords.x >= cols || coords.y >= rows) {
		throw "Out of range" + std::to_string(coords.x) + ", " + std::to_string(coords.y);
	}
	MapGridSpot * spot = grid->get(coords.x, coords.y);
	spot->setWall(true);
}



void MapGrid::initNeighbours()
{
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			MapGridSpot* spot = grid->get(i, j);
			spot->initNeighbours(this);
			spot->reset();
		}
	}
}

void MapGrid::reset() {
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			grid->get(i, j)->reset();
		}
	}
}

ObjectPoolItem * MapGrid::clone()
{
	MapGrid* newGrid = new MapGrid(width, height);

	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			MapGridSpot* spot = grid->get(i, j);
			if (spot->isWall()) {
				newGrid->setWall(sf::Vector2i(i, j));
			}
		}
	}

	newGrid->initNeighbours();

	return newGrid;
}
