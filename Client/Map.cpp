#include "Map.h"
#include "Game.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define METTOPIX 30.f
#define PIXTOMET 0.03333333333333333f


void Map::addRectangle(int x, int y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(x * PIXTOMET, y * PIXTOMET);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = world->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) * PIXTOMET, (32.f / 2) * PIXTOMET);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

Map::Map(Game* g)
{
	init();

	debugDrawInstance = new b2GLDraw();
	world = new b2World(b2Vec2(0.f, 0.f));
	world->SetAllowSleeping(true);

	world->SetDebugDraw(debugDrawInstance);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugDrawInstance->SetFlags(flags);

	addRectangle(64, 0);
	addRectangle(0, 0);
	addRectangle(32, 32);

}


Map::~Map()
{
	delete world;
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

void Map::update(sf::Time elapsedTime, Game* game)
{
	world->Step(1 / 60.f, 8, 3);
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

b2World* Map::getB2World() const
{
	return world;
}

