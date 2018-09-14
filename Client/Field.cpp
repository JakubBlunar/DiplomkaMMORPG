#include "Field.h"
#include "Globals.h"


Field::Field() {

}


Field::~Field() {
	layers.clear();
}


void Field::init() {
	RenderSprite* sprite = new RenderSprite();
	sprite->load("Graphics/Tilesets/hills.png", sf::Vector2i((int)FIELD_SIZE, (int)FIELD_SIZE), sf::Vector2i(0, 96));

	layers.push_back(sprite);
}

void Field::addLayer(RenderSprite * sprite)
{
	layers.push_back(sprite);
}


std::vector<RenderSprite*>* Field::getLayers() {
	return &layers;
}
