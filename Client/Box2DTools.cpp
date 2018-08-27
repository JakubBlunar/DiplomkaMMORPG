#include "Box2DTools.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Box2D.h>
#include "Globals.h"
#include "Map.h"
#include "Entity.h"

void Box2DTools::addBox(b2BodyType bodyType, float x, float y, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits) {
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bodyBox = map->getB2World()->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((32.f / 2) * PIXTOMET, (32.f / 2) * PIXTOMET);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;

	bodyBox->CreateFixture(&fixtureDef);

	entity->setBody(bodyBox);
}

void Box2DTools::addBox(b2BodyType bodyType, float x, float y, float width, float height, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits) {
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bodyBox = map->getB2World()->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((width / 2) * PIXTOMET, (height / 2) * PIXTOMET);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;

	bodyBox->CreateFixture(&fixtureDef);

	entity->setBody(bodyBox);
}

void Box2DTools::addStaticBox(float x, float y, Entity* entity, Map* map) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bodyBox = map->getB2World()->CreateBody(&bodyDef);
	b2PolygonShape staticBox;
	staticBox.SetAsBox((32.f / 2) * PIXTOMET, (32.f / 2) * PIXTOMET);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticBox;

	bodyBox->CreateFixture(&fixtureDef);

	entity->setBody(bodyBox);

}

void Box2DTools::addStaticBox(float x, float y, float width, float height, Entity* entity, Map* map) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x * PIXTOMET + (width / 2) * PIXTOMET, y * PIXTOMET + (height / 2) * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bodyBox = map->getB2World()->CreateBody(&bodyDef);
	b2PolygonShape staticBox;
	staticBox.SetAsBox((width / 2) * PIXTOMET, (height / 2) * PIXTOMET);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticBox;
	bodyBox->CreateFixture(&fixtureDef);

	entity->setBody(bodyBox);

}

void Box2DTools::addCircle(b2BodyType bodyType, float x, float y, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits) {
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bdCircle = map->getB2World()->CreateBody(&bodyDef);
	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = 16.f * PIXTOMET;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;

	bdCircle->CreateFixture(&fixtureDef);

	entity->setBody(bdCircle);
}

void Box2DTools::addCircle(b2BodyType bodyType, float x, float y, float radius, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits) {
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bdCircle = map->getB2World()->CreateBody(&bodyDef);
	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = (radius / 2.0f) * PIXTOMET;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;

	bdCircle->CreateFixture(&fixtureDef);

	entity->setBody(bdCircle);
}

void Box2DTools::addStaticCircle(float x, float y, Entity* entity, Map* map) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bdCircle = map->getB2World()->CreateBody(&bodyDef);
	b2CircleShape staticCircle;
	staticCircle.m_radius = 16.f * PIXTOMET;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticCircle;

	bdCircle->CreateFixture(&fixtureDef);

	entity->setBody(bdCircle);
}

void Box2DTools::addStaticCircle(float x, float y, float radius, Entity* entity, Map* map) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bdCircle = map->getB2World()->CreateBody(&bodyDef);
	b2CircleShape staticCircle;
	staticCircle.m_radius = (radius / 2.0f) * PIXTOMET;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticCircle;

	bdCircle->CreateFixture(&fixtureDef);

	entity->setBody(bdCircle);
}
