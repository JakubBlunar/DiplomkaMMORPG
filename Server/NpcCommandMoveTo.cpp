﻿#include "NpcCommandMoveTo.h"
#include "Astar.h"
#include "Npc.h"
#include "ServerGlobals.h"
#include "NpcEventNpcIsIdle.h"

s::NpcCommandMoveTo::NpcCommandMoveTo(sf::Vector2f position, Npc* npc, Map* m, Server* s, sf::Time maxDuration) : NpcCommand(maxDuration, s)
{
	this->endPosition = position;
	this->npc = npc;
	this->map = m;
	this->server = s;

}


s::NpcCommandMoveTo::~NpcCommandMoveTo()
{
}

void s::NpcCommandMoveTo::update(sf::Time elapsedTime, NpcUpdateEvents * npcUpdateEvents) {
	NpcCommand::update(elapsedTime, npcUpdateEvents);

	if (finished) {
		if (!finishedEventDispatched) {
			NpcEventNpcIsIdle* e = new NpcEventNpcIsIdle(npc);
			dispatchFinishEvent(e);
		}

		return;
	}

	b2Vec2 pos = npc->position.getBody()->GetPosition();
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

		if (start.x < 70) {
			direction.x = 0;
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

		if (start.y < 70) {
			direction.y = 0;
		}

		npc->setMovementDirection(direction, npc->attributes.getAttribute(EntityAttributeType::SPEED, true), npcUpdateEvents);
	}
	else {
		if (!finished) {
			finished = true;
			NpcEventNpcIsIdle* e = new NpcEventNpcIsIdle(npc);
			dispatchFinishEvent(e);
		}

		npc->setMovementDirection(sf::Vector2f(0, 0), npc->attributes.getAttribute(EntityAttributeType::SPEED, true), npcUpdateEvents);
	}
}

void s::NpcCommandMoveTo::init() {

	MapGrid* mapGrid = map->getGrid();

	sf::Vector2f pathFindingFrom = npc->position.getPosition();
	if (pathFindingFrom.x < 100)
		pathFindingFrom.x = 100;

	if (pathFindingFrom.y < 100)
		pathFindingFrom.y = 100;

	Astar astar(mapGrid);
	int res = astar.findPath(pathFindingFrom, endPosition);
	map->returnGrid(mapGrid);

	if (res == 1) {
		path = astar.path;
	}
	else {
		finished = true;
	}


}
