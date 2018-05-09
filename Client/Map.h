#pragma once
#include <Box2D/Dynamics/b2World.h>
#include <SFML/System/Time.hpp>

class Map
{
	int width, height;
	b2World world;

public:
	Map();
	~Map();

	void update(sf::Time )
};

