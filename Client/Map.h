#pragma once
#include <Box2D/Dynamics/b2World.h>
#include <SFML/System/Time.hpp>
#include "Field.h"
#include "Matrix.h"
#include "Player.h"

class Game;

class Map
{
	int width, height;
	Matrix<Field>* fields;
	b2World* world;
	Player* player;
	std::vector<Player*> players;

	b2Draw* debugDrawInstance;
	void addRectangle(int x, int y);
public:
	Map(Game* g);
	~Map();

	void init();

	void update(sf::Time elapsedTime, Game* game);

	Field* getField(int x, int y) const;
	int getWidth() const;
	int getHeight() const;
	b2World* getB2World() const;
};

