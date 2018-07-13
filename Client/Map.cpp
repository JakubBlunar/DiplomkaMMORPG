#include "Map.h"
#include "Game.h"
#include "Globals.h"
#include "b2GLDraw.h"
#include "Box2DTools.h"


Map::Map(Game* g)
{
	init();

	debugDrawInstance = new SFMLDebugDraw(g->window);
	world = new b2World(b2Vec2(0.f, 0.f));
	world->SetAllowSleeping(true);

	world->SetDebugDraw(debugDrawInstance);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	//flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;
	debugDrawInstance->SetFlags(flags);

	Player *p = new Player(true);
	addPlayer(p);
}


Map::~Map()
{
	delete world;
}

void Map::init()
{
	width = 255;
	height = 255;

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
	world->Step(elapsedTime.asSeconds(), 8, 3);
	for (Entity* entity : entities)
	{
		entity->update(elapsedTime, this);
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

b2World* Map::getB2World() const
{
	return world;
}


void Map::addPlayer(Player* player)
{
	players.push_back(player);
	entities.push_back(player);
	sf::Vector2f position = player->getPosition();
	Box2DTools::addDynamicCircle(position.x, position.y, player, this);
}

void Map::removePlayer(Player* player)
{
	world->DestroyBody(player->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), player), entities.end());
	players.erase(std::remove(players.begin(), players.end(), player), players.end());
}
