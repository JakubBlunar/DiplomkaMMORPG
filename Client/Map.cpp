#include "Map.h"



Map::Map()
{	
	init();
}


Map::~Map()
{
	
}

void Map::init()
{
	width = 7;
	height = 5;

	fields = new Matrix<Field>(width, height);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			fields->set(i, j, new Field());
		}
	}
}

Field* Map::getField(int x, int y) const
{
	return fields->get(x, y);
}

int Map::getWidth() const
{
	return width;
}

int Map::getHeight() const
{
	return height;
}

