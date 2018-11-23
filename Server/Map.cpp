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
#include "Npc.h"
#include "Location.h"
#include "Spawn.h"
#include "Random.h"
#include "NpcEventNpcIsIdle.h"
#include "EventDispatcher.h"

s::Map::Map(): id(0), world(nullptr), width(0), height(0) {
	npcUpdateInterval = sf::milliseconds(200);
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

void s::Map::addNpc(Npc * npc)
{
	lock.lock();
	sf::Vector2f position = npc->getPosition();

	b2Body* npcBody = createCircle(b2_kinematicBody, position.x, position.y, FIELD_SIZE / 2, NPC, GAME_OBJECT | BOUNDARY);
	npc->setBody(npcBody);
	npc->setMap(this);

	npcs.push_back(npc);
	lock.unlock();
}

void s::Map::removeNpc(Npc * npc)
{
	lock.lock();
	auto body = npc->getBody();
	if (body) {
		world->DestroyBody(body);
		npc->setBody(nullptr);
	}

	npcs.erase(std::remove(npcs.begin(), npcs.end(), npc), npcs.end());

	lock.unlock();
}

void s::Map::update(sf::Time deltaTime, Server* s) {
	lastUpdateNpc += deltaTime;

	lock.lock();
	
	if (characters.empty()) {
		lock.unlock();
		return;
	}

	world->Step(deltaTime.asSeconds(), 3, 2);

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

	
	for (std::pair<int, Location*> element : locations) {
		element.second->update(deltaTime, s, this);
	}

	NpcUpdateEvents eventsContainer;
	eventsContainer.npcsMovementChange = nullptr;

	if (lastUpdateNpc > npcUpdateInterval) {
		for (Npc* npc : npcs) {
			if (npc->isAlive()) {
				s->npcManager.updateNpc(lastUpdateNpc, npc, s, &eventsContainer);
			}
		}
		lastUpdateNpc = sf::Time::Zero;
	}
	
	lock.unlock();

	if (eventsContainer.npcsMovementChange != nullptr) {
		if (!eventsContainer.npcsMovementChange->npcsMovements.empty()) {
			sendEventToAllPlayers(eventsContainer.npcsMovementChange);
		}
		delete eventsContainer.npcsMovementChange;
	}
}

void s::Map::loadFromJson(std::string path, Server *s) {

	spdlog::get("log")->info("Loading map: {}", path);
	lock.lock();

	json mapData = JsonLoader::instance()->loadJson(path);

	json mapProperties = mapData["properties"].get<json::object_t>();

	id = (int)mapProperties["id"].get<json::number_integer_t>();
	width = (int)mapData["width"].get<json::number_integer_t>();
	height = (int)mapData["height"].get<json::number_integer_t>();

	//mapGrid = new MapGrid(width * (int)FIELD_SIZE, height * (int)FIELD_SIZE);

	world = new b2World(b2Vec2(0.f, 0.f));
	world->SetAllowSleeping(true);


	createBox(b2_staticBody, -6, -5, width * FIELD_SIZE + 12, 5, BOUNDARY, PLAYER | NPC);
	createBox(b2_staticBody, -5, -6, 5, FIELD_SIZE * height + 12, BOUNDARY, PLAYER | NPC);
	createBox(b2_staticBody, width * FIELD_SIZE, -6, 5, FIELD_SIZE * height + 12, BOUNDARY, PLAYER | NPC);
	createBox(b2_staticBody, -6, height * FIELD_SIZE, FIELD_SIZE * width + 12, 5, BOUNDARY, PLAYER | NPC);

	int initGridCount = 10;
	vector<MapGrid*> grids;
	grids.reserve(initGridCount);

	for (int i = 0; i < initGridCount; i++) {
		grids.push_back(new MapGrid(width * (int)FIELD_SIZE, height * (int)FIELD_SIZE));
	}


	json layers = mapData["layers"].get<json::array_t>();
	for (json::iterator layerIterator = layers.begin(); layerIterator != layers.end(); layerIterator++) {
		json layer = *layerIterator;

		std::string layerType = layer["type"].get<json::string_t>();
		std::string layerName = layer["name"].get<json::string_t>();

		if (layerType == "objectgroup" && layerName == "locations") {
			json locations = layer["objects"].get<json::array_t>();
			for (json::iterator locationIterator = locations.begin(); locationIterator != locations.end(); locationIterator++) {
				json jsonLoc = *locationIterator;

				int id = stoi(jsonLoc.value("name", "-100000"));
				float positionX = (float) jsonLoc["x"].get<json::number_float_t>();
				float positionY = (float) jsonLoc["y"].get<json::number_float_t>();

				Location* loc = nullptr;

				if (jsonLoc.find("polygon") != jsonLoc.end()) {
					json polygon = jsonLoc["polygon"].get<json::array_t>();

					int size = polygon.size();
					b2Vec2* vertices = new b2Vec2[size];
					int i = 0;
					for (json::iterator polygonIterator = polygon.begin(); polygonIterator != polygon.end(); polygonIterator++) {
						json point = *polygonIterator;
						float x = (float)point["x"].get<json::number_float_t>();
						float y = (float)point["y"].get<json::number_float_t>();
						vertices[i].Set(positionX + x, positionY + y);
						i++;
					}
					loc = new Location(id, vertices, size, sf::Vector2f(positionX, positionY), this);
				} else if(jsonLoc.find("width") != jsonLoc.end() && jsonLoc.find("height") != jsonLoc.end()) {
					int size = 4;
					b2Vec2* vertices = new b2Vec2[size];

					float width = (float)jsonLoc["width"].get<json::number_float_t>();
					float height = (float)jsonLoc["height"].get<json::number_float_t>();

					vertices[0].Set(positionX, positionY);
					vertices[1].Set(positionX + width, positionY);
					vertices[2].Set(positionX, positionY + height);
					vertices[3].Set(positionX + width, positionY + height);

					loc = new Location(id, vertices, size, sf::Vector2f(positionX, positionY), this);
				}

				if (loc) {
					this->locations.insert(std::make_pair(id, loc));
				}
				

			}
			
		}

	
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

							if (bodyType == BodyType::RECTANGLE) {
								createBox(b2_staticBody, positionX, positionY, width, height, GAME_OBJECT, PLAYER | ENEMY_PLAYER | NPC);
								for (MapGrid* grid: grids) {
									grid->setWall(sf::Vector2f(positionX, positionY + height / 2), sf::Vector2f(width, height));
								}
							}
							if (bodyType == BodyType::CIRCLE) {
								createCircle(b2_staticBody, positionX, positionY, width, GAME_OBJECT, PLAYER | ENEMY_PLAYER | NPC);
								for (MapGrid* grid: grids) {
									grid->setWall(sf::Vector2f(positionX, positionY), sf::Vector2f(width, width));
								}
							}
						
						}
						else
							throw "cannot load gameObject from file " + gameObjectType;
				
					}

				} else {
					float width = (float) gameObject["width"].get<json::number_float_t>();
					float height = (float) gameObject["height"].get<json::number_float_t>();

					if (width > 0 && height > 0) {
						createBox(b2_staticBody, positionX, positionY, width, height, BOUNDARY, PLAYER | ENEMY_PLAYER | NPC);
						for (MapGrid* grid: grids) {
							grid->setWall(sf::Vector2f(positionX, positionY + height / 2), sf::Vector2f(width, height));
						}
					}
				}
			}
		}

	}

	for (MapGrid* grid: grids) {
		grid->initNeighbours();
		mapGridObjectPool.addObject(grid);
	}

	json mapSpawns = JsonLoader::instance()->loadJson("Maps/Spawns/" + std::to_string(id));
	int mapId = (int)mapSpawns["mapId"].get<json::number_integer_t>();
	if (mapId == id) {
		json mapLocations = mapSpawns["locations"].get<json::array_t>();
		for (json::iterator locationIterator = mapLocations.begin(); locationIterator != mapLocations.end(); locationIterator++) {
			json spawnLocation = *locationIterator;
			int locationId = (int)spawnLocation["id"].get<json::number_integer_t>();

			auto foundLoc = locations.find(locationId);
			if (foundLoc != locations.end()) {
				Location* loc = foundLoc->second;
				json locationSpawns = spawnLocation["spawns"].get<json::array_t>();
				for (json::iterator spawniterator = locationSpawns.begin(); spawniterator != locationSpawns.end(); spawniterator++) {
					json jsonSpawn = *spawniterator;

					int npcType = (int)jsonSpawn["npcType"].get<json::number_integer_t>();
					int maxCount = (int)jsonSpawn["maxCount"].get<json::number_integer_t>();

					if (npcType > 0 && maxCount > 0) {
						Spawn* spawn = new Spawn(npcType, maxCount, loc);
						loc->addSpawn(spawn);

						spawn->init(s);
					}
				}

				if (spawnLocation.find("npcs") != spawnLocation.end()) {
					json spawnLocationNpcs = spawnLocation["npcs"].get<json::array_t>();
					for (json::iterator npcOnLocationIterator = spawnLocationNpcs.begin(); npcOnLocationIterator != spawnLocationNpcs.end(); npcOnLocationIterator++) {
						json locationNpc = *npcOnLocationIterator;
						int npcType = (int)locationNpc["npcType"].get<json::number_integer_t>();
						int posX = (int)locationNpc["posX"].get<json::number_integer_t>();
						int posY = (int)locationNpc["posY"].get<json::number_integer_t>();

						sf::Vector2f locationPosition = loc->getPosition();
						sf::Vector2f spawnPosition;
						spawnPosition.x = posX + locationPosition.x;
						spawnPosition.y = posY + locationPosition.y;

						Npc* npc = s->npcManager.createNpc(npcType);
						npc->setSpawnPosition(spawnPosition);
						npc->setPosition(spawnPosition);

						addNpc(npc);
						loc->addNpc(npc);

						int prob = Random::instance()->randomUniformInt(0, 100);
						if (prob < 20) {
							npc->setDeadTimestamp(sf::seconds(s->getServerTime().asSeconds() - npc->getRespawnTime().asSeconds() * Random::instance()->randomUniformFloat(0.1f, 0.9f)));
							npc->setNpcState(NpcState::DEAD);
						} else {
							NpcEventNpcIsIdle* e = new NpcEventNpcIsIdle();
							e->npc = npc;
							EventDispatcher<NpcEventNpcIsIdle>::dispatchEvent(e, s);
						}
					}
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

json s::Map::getNpcsJson()
{
	json jsonNpcs = json::array();
	sf::Lock mutexLock(lock);
	for (auto& npc : npcs) {
		jsonNpcs.push_back(npc->toJson());
	}
	return jsonNpcs;
}

MapGrid* s::Map::getGrid() {
	return mapGridObjectPool.getObject();
}

void s::Map::returnGrid(MapGrid* grid) {
	sf::Lock mutexLock(lock);
	mapGridObjectPool.addObject(grid);
}

void s::Map::sendEventToAnotherPlayers(GameEvent* event, int characterId)
{
	sf::Lock mutexLock(lock);

	sf::Packet* p = event->toPacket();

	std::for_each(
		characters.begin(),
		characters.end(),
	    [=](Character* character)
	    {
			if (character && character->id != characterId) {
				Session* s = character->getAccount()->getSession();
				s->socket->send(*p);
			}
	    });

	delete p;
}

void s::Map::sendEventToAllPlayers(GameEvent* event) {
	sf::Lock mutexLock(lock);

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
