#include "Map.h"
#include "Game.h"
#include "Globals.h"
#include "b2GLDraw.h"
#include "Box2DTools.h"
#include "GameObject.h"
#include "JsonLoader.h"


Map::Map(Game* game)
{
	init();

	debugDrawInstance = new SFMLDebugDraw(game->window);
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

	Player *p = new Player(1, true);
	addPlayer(p);

	GameObject* gameObject = new GameObject(1, "GameObjects/largeTree");
	addGameObject(gameObject);

	gameObject = new GameObject(2, "GameObjects/largeTree");
	gameObject->getPositionComponent()->setPosition(sf::Vector2f(280, 320));
	addGameObject(gameObject);

	gameObject = new GameObject(2, "GameObjects/largeTree");
	gameObject->getPositionComponent()->setPosition(sf::Vector2f(150, 270));
	addGameObject(gameObject);
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
	Box2DTools::addCircle(b2_dynamicBody, position.x, position.y, player, this);
}

void Map::removePlayer(Player* player)
{
	world->DestroyBody(player->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), player), entities.end());
	players.erase(std::remove(players.begin(), players.end(), player), players.end());
}

void Map::addGameObject(GameObject* gameObject)
{
	PositionComponent* po = (PositionComponent*) gameObject->getComponent(ComponentType::POSITION);
	if(po == nullptr)
	{
		return;
	}
	
	entities.push_back(gameObject);
	sf::Vector2f position = po->getPosition();
	sf::Vector2f size = po->getSize();
	BodyType bodyType = po->getBodyType();
	if (bodyType == BodyType::RECTANGLE)
	{
		Box2DTools::addBox(b2_kinematicBody, position.x, position.y, size.x, size.y, gameObject, this);
	}else
	{
		Box2DTools::addCircle(b2_kinematicBody, position.x, position.y, size.x, gameObject, this);
	}
}

void Map::removeGameObject(GameObject* gameObject){
	world->DestroyBody(gameObject->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), gameObject), entities.end());
}

