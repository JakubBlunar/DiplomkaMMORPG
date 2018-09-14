#include "Map.h"
#include <Box2D/Box2D.h>
#include "ServerGlobals.h"
#include "JsonLoader.h"
#include <spdlog/spdlog.h>
#include "Account.h"
#include "EventMovementChange.h"
#include "EntityConstants.h"
#include "EventCharacterMapJoin.h"
#include "EventCharacterMapLeave.h"

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

	b2Body* characterBody = createCircle(b2_kinematicBody, position.x, position.y, FIELD_SIZE / 2, PLAYER, GAME_OBJECT | BOUNDARY);
	character->body = characterBody;
	character->mapId = id;

	EventCharacterMapJoin eventMapJoin;
	eventMapJoin.mapId = id;
	eventMapJoin.characterData = character->toJson().dump();

	sendEventToAnotherPlayers(&eventMapJoin, character->id);

	characters.push_back(character);
	lock.unlock();
}

void s::Map::removeCharacter(Character* character) {
	lock.lock();
	if (character->body) {
		world->DestroyBody(character->body);
		character->body = nullptr;
	}

	EventCharacterMapLeave* e = new EventCharacterMapLeave();
	e->characterId = character->id;
	e->mapId = id;

	sendEventToAnotherPlayers(e, character->id);
	delete e;

	characters.erase(std::remove(characters.begin(), characters.end(), character), characters.end());
	lock.unlock();
}

void s::Map::update(sf::Time deltaTime, Server* s) {
	lock.lock();
	world->Step(deltaTime.asSeconds(), 5, 2);

	std::for_each(
		characters.begin(),
		characters.end(),
	    [=](Character* character)
	    {
			b2Vec2 position = character->body->GetPosition();
			b2Vec2 velocity = character->body->GetLinearVelocity();

			character->position.x = position.x * METTOPIX;
			character->position.y = position.y * METTOPIX;

			character->movement.x = velocity.x * METTOPIX;
			character->movement.y = velocity.y * METTOPIX;
	    });
	lock.unlock();
}

void s::Map::loadFromJson(std::string path) {

	spdlog::get("log")->info("Loading map: {}", path);
	lock.lock();

	json mapData = JsonLoader::instance()->loadJson(path);

	json mapProperties = mapData["properties"].get<json::object_t>();

	id = (int)mapProperties["id"].get<json::number_integer_t>();
	width = (int)mapData["width"].get<json::number_integer_t>();
	height = (int)mapData["height"].get<json::number_integer_t>();


	world = new b2World(b2Vec2(0.f, 0.f));
	world->SetAllowSleeping(true);


	createBox(b2_staticBody, 0, -2, width * FIELD_SIZE, 2, BOUNDARY, PLAYER);
	createBox(b2_staticBody, -2, 0, 2, FIELD_SIZE * height, BOUNDARY, PLAYER);
	createBox(b2_staticBody, width * FIELD_SIZE, 0, 2, FIELD_SIZE * height, BOUNDARY, PLAYER);
	createBox(b2_staticBody, 0, height * FIELD_SIZE, FIELD_SIZE * width, 2, BOUNDARY, PLAYER);

	json layers = mapData["layers"].get<json::array_t>();
	for (json::iterator layerIterator = layers.begin(); layerIterator != layers.end(); layerIterator++) {
		json layer = *layerIterator;

		std::string layerType = layer["type"].get<json::string_t>();
		std::string layerName = layer["name"].get<json::string_t>();

		if (layerType == "objectgroup" && layerName == "gameobjects") {
			json gameObjects = layer["objects"].get<json::array_t>();

			for (json::iterator gameObjectIterator = gameObjects.begin(); gameObjectIterator != gameObjects.end(); gameObjectIterator++) {
				json gameObject = *gameObjectIterator;
				float positionX = (float) gameObject["x"].get<json::number_float_t>();
				float positionY = (float) gameObject["y"].get<json::number_float_t>();

				if (gameObject.count("point") && gameObject["point"].get<json::boolean_t>()) {
						
					string gameObjectType = gameObject["name"].get<json::string_t>();
				
					if (!gameObjectType.empty()) {
						json jsonFile = JsonLoader::instance()->loadJson("GameObjects/" + gameObjectType);
	
						json position = jsonFile["position"].get<json::object_t>();
						if (position.is_object()) {
							BodyType bodyType = static_cast<BodyType>(position["bodyType"].get<json::number_integer_t>());
							float width = (float)position["width"].get<json::number_float_t>();
							float height = (float)position["height"].get<json::number_float_t>();

							if (bodyType == BodyType::RECTANGLE)
								createBox(b2_staticBody, positionX, positionY, width, height, GAME_OBJECT, PLAYER | ENEMY_PLAYER);
							if (bodyType == BodyType::CIRCLE) 
								createCircle(b2_staticBody, positionX, positionY, width, GAME_OBJECT, PLAYER | ENEMY_PLAYER);
						}
						else
							throw "cannot load gameObject from file " + gameObjectType;
				
					}

				} else {
					float width = (float) gameObject["width"].get<json::number_float_t>();
					float height = (float) gameObject["height"].get<json::number_float_t>();

					if (width > 0 && height > 0) 
						createBox(b2_staticBody, positionX, positionY, width, height, BOUNDARY, PLAYER | ENEMY_PLAYER);
				}
			}
		}
	}


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

void s::Map::sendEventToAnotherPlayers(GameEvent* event, int characterId)
{
	lock.lock();

	sf::Packet* p = event->toPacket();

	std::for_each(
		characters.begin(),
		characters.end(),
	    [=](Character* character)
	    {
			if (character->id != characterId) {
				Session* s = character->getAccount()->getSession();
				s->socket->send(*p);
			}
	    });

	delete p;
	lock.unlock();
}

void s::Map::sendEventToAllPlayers(GameEvent* event) {
	lock.lock();

	sf::Packet* p = event->toPacket();

	std::for_each(
		characters.begin(),
		characters.end(),
	    [=](Character* character)
	    {
			Session* s = character->getAccount()->getSession();
			s->socket->send(*p);
	    });

	delete p;
	lock.unlock();
}

void s::Map::handleEvent(GameEvent* event, Session* playerSession, Server* server) {
	switch(event->getId()) {
		case MOVEMENT: {
			EventMovementChange* e = (EventMovementChange*)event;

			lock.lock();
			Character* character = playerSession->getAccount()->getCharacter();
			character->body->SetTransform(b2Vec2(e->x * PIXTOMET, e->y * PIXTOMET), character->body->GetAngle());
			character->body->SetLinearVelocity(b2Vec2(e->velX * PIXTOMET, e->velY * PIXTOMET));

			character->position = sf::Vector2f(e->x, e->y);
			character->movement = sf::Vector2f(e->velX, e->velY);
			character->speed = e->speed;
			sendEventToAnotherPlayers(event, character->id);

			lock.unlock();
			break;
		}
		default:
			break;
	}
}


b2Body* s::Map::createCircle(b2BodyType bodyType, float x, float y, float radius, int16 categoryBits, uint16 maskBits) {
	lock.lock();

	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;

	b2Body* bdCircle = world->CreateBody(&bodyDef);
	lock.unlock();

	b2CircleShape dynamicCircle;
	dynamicCircle.m_radius = (radius / 2.0f) * PIXTOMET;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicCircle;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;

	bdCircle->CreateFixture(&fixtureDef);

	lock.unlock();
	return bdCircle;
}

b2Body* s::Map::createBox(b2BodyType bodyType, float x, float y, float width, float height, int16 categoryBits, uint16 maskBits) {
	lock.lock();
	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(x * PIXTOMET, y * PIXTOMET);
	bodyDef.angle = 0;
	bodyDef.fixedRotation = true;
	b2Body* bodyBox = world->CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((width / 2) * PIXTOMET, (height / 2) * PIXTOMET);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;

	bodyBox->CreateFixture(&fixtureDef);
	lock.unlock();
	return bodyBox;
}
