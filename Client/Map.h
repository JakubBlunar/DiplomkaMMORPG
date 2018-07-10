#pragma once
#include <Box2D/Dynamics/b2World.h>
#include <SFML/System/Time.hpp>
#include "Field.h"
#include "Matrix.h"

class Map
{
	int width, height;
	Matrix<Field>* fields;

public:
	Map();
	~Map();

	void init();

	void update(sf::Time);

	Field* getField(int x, int y) const;
	int getWidth() const;
	int getHeight() const;
};

