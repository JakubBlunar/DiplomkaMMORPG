#include "stdafx.h"
#include "Astar.h"



Astar::Astar(MapGrid* grid): current(nullptr), start(nullptr), end(nullptr) {
	this->grid = grid;
}


Astar::~Astar()
{
}

int Astar::findPath(sf::Vector2f from, sf::Vector2f to)
{
	grid->reset();

	this->from = from;
	this->to = to;

	int fromX = grid->transformXToGrid(from.x);
	int fromY = grid->transformYToGrid(from.y);

	int toX = grid->transformXToGrid(to.x);
	int toY = grid->transformYToGrid(to.y);

	start = grid->grid->get(fromX, fromY);
	end = grid->grid->get(toX, toY);

	if(start->isWall() || end->isWall()) {
		return -1;
	}

	openSet.insert(std::make_pair(start->id, start));

	int res = 0;
	while (res == 0) {
		res = step();
	}


    MapGridSpot* temp = current;

	path.clear();
	
	path.push_front(sf::Vector2f((float)temp->positionX, (float)temp->positionY));
    while (temp->previous) {
        path.push_front(sf::Vector2f((float)temp->previous->positionX, (float)temp->previous->positionY));
        temp = temp->previous;
    }
	path.push_front(from);

	if(res == 1) {
		return 1;
	}
	return -1;
}

int Astar::step()
{
	int openSetSize = openSet.size();
	if (openSetSize > 0 && current != end) {
		
		current = openSet.begin()->second;
       
		for (auto spotPair : openSet)
		{
			if (spotPair.second->f < current->f) {
                current = spotPair.second;
            }
		}

        openSet.erase(current->id);
        closedSet.insert(std::make_pair(current->id, current));

		std::list<MapGridSpot*> neighbours = current->neighbours;
		for (MapGridSpot* neighbour : neighbours)
		{
			bool newPath = false;

			auto found = closedSet.find(neighbour->id);
            if (found == closedSet.end() && !neighbour->isWall()) {
                float tempG = current->g + 1;
				auto openSetFound = openSet.find(neighbour->id);
                if (openSetFound != openSet.end()) {
                    if (tempG < neighbour->g) {
                        neighbour->g = tempG;
                        newPath = true;
                    }
                } else {
                    neighbour->g = tempG;
                    newPath = true;
                    openSet.insert(std::make_pair(neighbour->id, neighbour));
                }

                if (newPath) {
                    neighbour->h = heuristic(neighbour, end);
                    neighbour->f = neighbour->g + neighbour->h;
                    neighbour->previous = current;
                }
            }
		}
    } else {
        if (current == end) {
            return 1;
        } 
        return -1;
    }

    return 0;
}

float Astar::heuristic(MapGridSpot * a, MapGridSpot * b) const {
	return (float)abs(a->x - b->x) + abs(a->y - b->y);
}
