#include "Field.h"



Field::Field()
{
	init();
}


Field::~Field()
{
}


void Field::init()
{
	RenderSprite * sprite = new RenderSprite();
	sprite->load("Dungeon_A2.png", sf::Vector2i(32, 32), sf::Vector2i(0, 0));

	layers.push_back(sprite);
}


std::vector<RenderSprite*>* Field::getLayers()
{
	return &layers;
}


