#include "Map.h"
#include <Box2D/Box2D.h>
#include "ServerGlobals.h"
#include "JsonLoader.h"
#include <spdlog/spdlog.h>

s::Map::Map(): id(0) {
}

s::Map::~Map() {
}

int s::Map::getId() const {
	return id;
}

void s::Map::addCharacter(Character* character) {
	lock.lock();
	sf::Vector2f position = character->position;

	b2Body* characterBody = createCircle(b2_kinematicBody, position.x, position.y, 32);
	character->body = characterBody;
	character->mapId = id;


	characters.push_back(character);
	lock.unlock();
}

void s::Map::removeCharacter(Character* character) {
	lock.lock();
	if (character->body) {
		world->DestroyBody(character->body);
		character->body = nullptr;
	}
	characters.erase(std::remove(characters.begin(), characters.end(), character), characters.end());
	lock.unlock();
}

void s::Map::update(sf::Time deltaTime, Server* s) {
	lock.lock();
	world->Step(deltaTime.asSeconds(), 5, 2);
	lock.unlock();
}

void s::Map::loadFromJson(std::string path) {

	spdlog::get("log")->info("Loading map: {}", path);
	lock.lock();

	json mapData = JsonLoader::instance()->loadJson(path);

	id = (int)mapData["id"].get<json::number_integer_t>();

	width = (int)mapData["width"].get<json::number_integer_t>();
	height = (int)mapData["height"].get<json::number_integer_t>();

	world = new b2World(b2Vec2(0.f, 0.f));
	world->SetAllowSleeping(true);


	createBox(b2_staticBody, 0, -2, width * FIELD_SIZE, 2);
	createBox(b2_staticBody, -2, 0, 2, FIELD_SIZE * height);
	createBox(b2_staticBody, width * FIELD_SIZE, 0, 2, FIELD_SIZE * height);
	createBox(b2_staticBody, 0, height * FIELD_SIZE, FIELD_SIZE * width, 2);


	lock.unlock();
}

json s::Map::getCharactersJson() {
	json jsonCharacters = json::array();
	lock.lock();
	for (auto& ch : characters) {
		jsonCharacters.push_back(ch->toJson());
	}
	lock.unlock();
	return jsonCharacters;
}


b2Body* s::Map::createCircle(b2BodyType bodyType, float x, float y, float radius) {
	lock.lock();

	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET + (radius / 2.0f) * PIXTOMET, y * PIXTOMET + (radius / 2.0f) * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;

	b2Body* bdCircle = world->CreateBody(&bodyDef);
	lock.unlock();

	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = (radius / 2.0f) * PIXTOMET;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;

	bdCircle->CreateFixture(&fixtureDef);

	lock.unlock();
	return bdCircle;
}

b2Body* s::Map::createBox(b2BodyType bodyType, float x, float y, float width, float height) {
	lock.lock();
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET + (width / 2) * PIXTOMET, y * PIXTOMET + (height / 2) * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bodyBox = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((width / 2) * PIXTOMET, (height / 2) * PIXTOMET);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	bodyBox->CreateFixture(&fixtureDef);
	lock.unlock();
	return bodyBox;
}
