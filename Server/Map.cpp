#include "Map.h"
#include <Box2D/Box2D.h>
#include "ServerGlobals.h"

s::Map::Map(): id(0)
{
	world = new b2World(b2Vec2(0.f, 0.f));
	world->SetAllowSleeping(true);
}

s::Map::~Map()
{
}

void s::Map::addCharacter(Character* character)
{
	sf::Vector2f position = character->position;

	b2Body* characterBody = createCircle(b2_kinematicBody, position.x, position.y, 32);
	character->body = characterBody;
	character->mapId = id;

	characters.push_back(character);
}

void s::Map::removeCharacter(Character * character)
{
	if(character->body)
	{
		world->DestroyBody(character->body);
	}
	characters.erase(std::remove(characters.begin(), characters.end(), character), characters.end());
}

void s::Map::update(sf::Time deltaTime, Server * s) const
{
	world->Step(deltaTime.asSeconds(), 5, 2);
}

void s::Map::loadFromJson()
{

}


b2Body* s::Map::createCircle(b2BodyType bodyType, float x, float y, float radius) const
{
    b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x*PIXTOMET + (radius / 2.0f) * PIXTOMET, y*PIXTOMET + (radius / 2.0f) * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
    b2Body* bdCircle = world->CreateBody(&bodyDef);
    b2CircleShape dynamicCircle;
    dynamicCircle.m_radius = (radius / 2.0f) * PIXTOMET;
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicCircle;

    bdCircle->CreateFixture(&fixtureDef);

    return bdCircle;
}
