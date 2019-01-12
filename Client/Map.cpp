#include "Map.h"
#include "Game.h"
#include "Globals.h"
#include "b2GLDraw.h"
#include "Box2DTools.h"
#include "GameObject.h"
#include "JsonLoader.h"
#include "Collider.h"
#include "EventDispatcher.h"
#include "EventCharacterMapLeave.h"
#include "TileSetsHolder.h"
#include "Utils.h"
#include <Box2D/Box2D.h>
#include "b2GLDraw.h"
#include "Spell.h"
#include "EventSpellCastResult.h"
#include "EntityPrototypes.h"
#include "GamePlayScene.h"

Map::Map(Game* g) {
	this->game = g;
}


Map::~Map() {
	unsubscribe();

	for (unsigned int i = 0; i < entities.size(); i++) {
		if (entities[i] != player) {
			delete entities[i];
		}
	}

	players.clear();
	entities.clear();
	npcs.clear();
	delete world;
	delete fields;
}

void Map::init() {
	/*fields = new Matrix<Field>(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			fields->set(i, j, new Field());
		}
	}

	/*GameObject* gameObject = new GameObject(1, "GameObjects/largeTree");
	addGameObject(gameObject);

	gameObject = new GameObject(2, "GameObjects/largeTree");
	gameObject->getPositionComponent()->setPosition(sf::Vector2f(280, 320));
	addGameObject(gameObject);

	gameObject = new GameObject(2, "GameObjects/largeTree");
	gameObject->getPositionComponent()->setPosition(sf::Vector2f(150, 270));
	addGameObject(gameObject);

	Collider* collider = new Collider(-1);
	PositionComponent* position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(0, -2));
	position->setSize(sf::Vector2f(width * FIELD_SIZE, 2));
	this->addCollider(collider);

	collider = new Collider(-2);
	position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(-2, 0));
	position->setSize(sf::Vector2f(2, FIELD_SIZE * height));
	this->addCollider(collider);

	collider = new Collider(-3);
	position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(width * FIELD_SIZE, 0));
	position->setSize(sf::Vector2f(2, FIELD_SIZE * height));
	this->addCollider(collider);

	collider = new Collider(-4);
	position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(0, height * FIELD_SIZE));
	position->setSize(sf::Vector2f(FIELD_SIZE * width, 2));
	this->addCollider(collider);

	subscribe();
	*/


}

void Map::update(sf::Time elapsedTime, Game* game) {
	world->Step(elapsedTime.asSeconds(), 6, 2);
	for (Entity* entity : entities) {
		entity->update(elapsedTime, this, game);
	}
}

Field* Map::getField(int x, int y) const {
	return fields->get(x, y);
}

int Map::getWidth() const {
	return width;
}

int Map::getHeight() const {
	return height;
}

b2World* Map::getB2World() const {
	return world;
}

MapGrid* Map::getGrid() const {
	return grid;
}

EntityToEntityRayCast* Map::makeRayCast(Entity* startEntity, Entity* endEntity) const {
	EntityToEntityRayCast* callback = new EntityToEntityRayCast(startEntity, endEntity);

	b2Vec2 start = startEntity->getBody()->GetPosition();
	b2Vec2 end = endEntity->getBody()->GetPosition();

	world->RayCast(callback, start, end);
	return callback;
}

std::vector<Entity*>* Map::getEntities() {
	return &entities;
}

void Map::handleEvent(GameEvent* event) {
	switch (event->getId()) {
		case CHARACTER_MAP_JOIN: {
			EventCharacterMapJoin* e = (EventCharacterMapJoin*)event;
			if (e->mapId != id)
				return;

			json characterData = json::parse(e->characterData);
			Player* p = new Player(false);
			p->loadFromJson(characterData);

			auto exists = players.find(p->getId());
			if (exists == players.end()) {
				return;
			}

			addPlayer(p);
			return;
		}
		case CHARACTER_MAP_LEAVE: {
			EventCharacterMapLeave* e = (EventCharacterMapLeave*)event;
			if (e->mapId != this->id)
				return;

			auto exists = players.find(e->characterId);
			if (exists == players.end()) {
				return;
			}

			Player* p = exists->second;
			removePlayer(p);
			delete p;
			return;
		}
		case SPELL_CAST_RESULT: {
			EventSpellCastResult* e = (EventSpellCastResult*)event;
			SpellInfo* si = EntityPrototypes::instance()->getSpellInfo(e->spellId);

			if (!si) return;

			if (e->entityCategory == PLAYER) {
				if (e->entityId == (int)player->getId()) {
					player->setCastingSpell(nullptr);
					if (e->result != SpellCastResultCode::SUCCESS) {
						GameMessage* m = new GameMessage();
						m->message = getStringSpellErrorByResultCode(e->result);
						m->displayTime = sf::seconds(1.5f);
						game->addGameMessage(m);
					}
				}

				auto playerEntity = players.find(e->entityId);
				if (playerEntity != players.end()) {
					if (e->result == SpellCastResultCode::SUCCESS) {
						AttributesComponent* attributes = playerEntity->second->getAttributesComponent();
						attributes->modifyAttribute(EntityAttributeType::MP, -si->manaCost);
					}
				}

				if (si->hasEntity && e->result == SpellCastResultCode::SUCCESS) {
					Spell* spell = EntityPrototypes::instance()->createSpell(si->id);

					bool create = true;
					switch (e->target) {
						case SpellTarget::PLAYER: {
							auto found = players.find(e->targetId);
							if (found == players.end()) {
								create = false;
								break;
							}

							spell->setTarget(found->second);
							break;
						}
						case SpellTarget::NPC: {
							auto found = npcs.find(e->targetId);
							if (found == npcs.end()) {
								create = false;
								break;
							}

							spell->setTarget(found->second);
							break;
						}
						default:
							create = false;
							break;
					}

					if (create) {
						spell->getPositionComponent()->setPosition(e->startPosition);
						addSpell(spell);
					} else {
						delete spell;
					}
				}
			}


		}
		default:
			break;
	}

}


void Map::addPlayer(Player* player) {
	players.insert(std::make_pair(player->getId(), player));
	entities.push_back(player);

	if (player->isControlledByPlayer()) {
		this->player = player;
	}

	sf::Vector2f position = player->getPosition();
	sf::Vector2f size = player->getSize();
	Box2DTools::addCircle(b2_dynamicBody, position.x, position.y, size.x, player, this, player->getEntityCategory(),
	                      player->getCollisionMask());

	b2Body* playerBody = player->getBody();

	float radius = 32 * PIXTOMET;
	b2Vec2 vertices[8];
	vertices[0].Set(0, 0);
	for (int i = 0; i < 7; i++) {
		float angle = i / 6.0f * 180.f * DEGTORAD;
		vertices[i + 1].Set(radius * cosf(angle), radius * sinf(angle));
	}

	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 8);

	b2FixtureDef melleViewDef;
	melleViewDef.shape = &polygonShape;
	melleViewDef.filter.categoryBits = PLAYER_SENSOR;
	melleViewDef.filter.maskBits = ENEMY_PLAYER | GAME_OBJECT;
	melleViewDef.isSensor = true;
	b2Fixture* melleView = playerBody->CreateFixture(&melleViewDef);
	player->setMelleView(melleView);

	b2CircleShape circleShape;
	circleShape.m_radius = radius;

	b2FixtureDef melleRangeDef;
	melleRangeDef.shape = &circleShape;
	melleRangeDef.filter.categoryBits = PLAYER_SENSOR;
	melleRangeDef.filter.maskBits = ENEMY_PLAYER | GAME_OBJECT;
	melleRangeDef.isSensor = true;

	b2Fixture* melleRange = playerBody->CreateFixture(&melleRangeDef);
	player->setMelleRange(melleRange);

}

void Map::removePlayer(Player* player) {
	world->DestroyBody(player->getBody());
	player->setBody(nullptr);
	entities.erase(std::remove(entities.begin(), entities.end(), player), entities.end());
	players.erase(player->getId());
}

void Map::addGameObject(GameObject* gameObject) {
	PositionComponent* po = gameObject->getPositionComponent();
	if (po == nullptr) {
		return;
	}

	entities.push_back(gameObject);
	sf::Vector2f position = po->getPosition();
	sf::Vector2f size = po->getSize();
	BodyType bodyType = po->getBodyType();
	if (bodyType == BodyType::RECTANGLE) {
		Box2DTools::addBox(b2_kinematicBody, position.x, position.y, size.x, size.y, gameObject, this,
		                   gameObject->getEntityCategory(), gameObject->getCollisionMask());
	}
	else {
		Box2DTools::addCircle(b2_kinematicBody, position.x, position.y, size.x, gameObject, this,
		                      gameObject->getEntityCategory(), gameObject->getCollisionMask());
	}
}

void Map::addCollider(Collider* collider) {
	PositionComponent* po = (PositionComponent*)collider->getComponent(ComponentType::POSITION);
	if (po == nullptr) {
		return;
	}

	entities.push_back(collider);
	sf::Vector2f position = po->getPosition();
	sf::Vector2f size = po->getSize();
	BodyType bodyType = po->getBodyType();
	if (bodyType == BodyType::RECTANGLE) {
		Box2DTools::addBox(b2_staticBody, position.x, position.y, size.x, size.y, collider, this,
		                   collider->getEntityCategory(), collider->getCollisionMask());
	}
	else {
		Box2DTools::addCircle(b2_staticBody, position.x, position.y, size.x, collider, this,
		                      collider->getEntityCategory(),
		                      collider->getCollisionMask());
	}
}


void Map::removeGameObject(GameObject* gameObject) {
	world->DestroyBody(gameObject->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), gameObject), entities.end());
}

void Map::addNpc(Npc* npc) {

	sf::Vector2f position = npc->getPosition();
	sf::Vector2f size = npc->getSize();
	Box2DTools::addCircle(b2_dynamicBody, position.x, position.y, size.x, npc, this, npc->getEntityCategory(),
	                      npc->getCollisionMask());

	entities.push_back(npc);
	npcs.insert(std::make_pair(npc->getId(), npc));
}

void Map::removeNpc(Npc* npc) {
	world->DestroyBody(npc->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), npc), entities.end());
	npcs.erase(npc->getId());
}

void Map::addSpell(Spell* spell) {
	PositionComponent* po = spell->getPositionComponent();
	if (po == nullptr) {
		return;
	}

	entities.push_back(spell);
	sf::Vector2f position = po->getPosition();
	sf::Vector2f size = po->getSize();
	BodyType bodyType = po->getBodyType();
	if (bodyType == BodyType::RECTANGLE) {
		Box2DTools::addBox(b2_kinematicBody, position.x, position.y, size.x, size.y, spell, this,
		                   spell->getEntityCategory(), spell->getCollisionMask());
	}
	else {
		Box2DTools::addCircle(b2_kinematicBody, position.x, position.y, size.x, spell, this,
		                      spell->getEntityCategory(), spell->getCollisionMask());
	}
}

void Map::removeSpell(Spell* spell) {
	world->DestroyBody(spell->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), spell), entities.end());
	delete spell;
}

void Map::loadFromFile(int id) {
	debugDrawInstance = (b2Draw*)new SFMLDebugDraw(*game->window);
	world = new b2World(b2Vec2(0.f, 0.f));
	world->SetAllowSleeping(true);

	world->SetDebugDraw(debugDrawInstance);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;
	debugDrawInstance->SetFlags(flags);

	json mapData = JsonLoader::instance()->loadJson("Maps/" + std::to_string(id));
	json mapProperties = mapData["properties"].get<json::object_t>();

	this->id = (int)mapProperties["id"].get<json::number_integer_t>();
	width = (int)mapData["width"].get<json::number_integer_t>();
	height = (int)mapData["height"].get<json::number_integer_t>();

	fields = new Matrix<Field>(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			fields->set(i, j, new Field());
		}
	}

	grid = new MapGrid(width * (int)FIELD_SIZE, height * (int)FIELD_SIZE);

	Collider* collider = new Collider(-1);
	PositionComponent* position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(width * FIELD_SIZE / 2, -2));
	position->setSize(sf::Vector2f(width * FIELD_SIZE, 2));
	this->addCollider(collider);

	collider = new Collider(-2);
	position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(-2, FIELD_SIZE * height / 2));
	position->setSize(sf::Vector2f(2, FIELD_SIZE * height));
	this->addCollider(collider);

	collider = new Collider(-3);
	position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(width * FIELD_SIZE, FIELD_SIZE * height / 2));
	position->setSize(sf::Vector2f(2, FIELD_SIZE * height));
	this->addCollider(collider);

	collider = new Collider(-4);
	position = collider->getPositionComponent();
	position->setBodyType(BodyType::RECTANGLE);
	position->setPosition(sf::Vector2f(width * FIELD_SIZE / 2, height * FIELD_SIZE));
	position->setSize(sf::Vector2f(FIELD_SIZE * width, 2));
	this->addCollider(collider);


	json jsonTilesets = mapData["tilesets"].get<json::array_t>();
	std::map<int, TileSet> tilesets;

	for (json::iterator jsonTilesetit = jsonTilesets.begin(); jsonTilesetit != jsonTilesets.end(); ++jsonTilesetit) {
		json jsonTileset = *jsonTilesetit;
		int firstgid = (int)jsonTileset["firstgid"].get<json::number_integer_t>();
		std::string path = jsonTileset["source"].get<json::string_t>();

		std::vector<std::string> splittedPath = Utils::splitPath(path, {'/'});
		path = Utils::removeExtension(splittedPath.back());

		TileSet tileSet = TileSetsHolder::instance()->getTileSet(path);
		tilesets.insert(std::make_pair(firstgid, tileSet));
	}

	TileSet empty;
	empty.valid = false;
	tilesets.insert(std::make_pair(99999999, empty));

	json layers = mapData["layers"].get<json::array_t>();
	for (json::iterator layerIterator = layers.begin(); layerIterator != layers.end(); layerIterator++) {
		json layer = *layerIterator;

		std::string layerType = layer["type"].get<json::string_t>();
		std::string layerName = layer["name"].get<json::string_t>();

		if (layerType == "tilelayer") {
			int width = (int)layer["width"].get<json::number_integer_t>();
			int height = (int)layer["width"].get<json::number_integer_t>();

			json data = layer["data"].get<json::array_t>();

			int counter = 0;
			for (json::iterator fieldValueIterator = data.begin(); fieldValueIterator != data.end(); fieldValueIterator
			     ++) {
				int value = *fieldValueIterator;
				if (value > 0) {
					int x = counter % width;
					int y = counter / width;


					TileSet foundTileSet;
					foundTileSet.valid = false;
					foundTileSet.columns = 1;

					int tilesetIdOffset = 0;

					for (std::map<int, TileSet>::iterator tilesetIterator = tilesets.begin(); tilesetIterator !=
					     tilesets.end();
					     tilesetIterator++) {
						if (tilesetIdOffset <= value) {
							if (value < tilesetIterator->first) {
								break;
							}
						}
						foundTileSet = tilesetIterator->second;
						tilesetIdOffset = tilesetIterator->first;
					}

					if (foundTileSet.valid) {
						value = value - tilesetIdOffset;
						int tileX = value % foundTileSet.columns;
						int tileY = value / foundTileSet.columns;

						RenderSprite* sprite = new RenderSprite();
						sprite->load(foundTileSet.path, sf::Vector2i((int)FIELD_SIZE, (int)FIELD_SIZE),
						             sf::Vector2i(tileX * (int)FIELD_SIZE, tileY * (int)FIELD_SIZE));

						fields->get(x, y)->addLayer(sprite);
					}

				}
				counter++;
			}
		}

		if (layerType == "objectgroup" && layerName == "gameobjects") {
			json gameObjects = layer["objects"].get<json::array_t>();

			for (json::iterator gameObjectIterator = gameObjects.begin(); gameObjectIterator != gameObjects.end();
			     gameObjectIterator++) {
				json gameObject = *gameObjectIterator;

				if (gameObject.count("point") && gameObject["point"].get<json::boolean_t>()) {

					std::string gameObjectType = gameObject["name"].get<json::string_t>();
					float positionX = (float)gameObject["x"].get<json::number_float_t>();
					float positionY = (float)gameObject["y"].get<json::number_float_t>();
					if (!gameObjectType.empty()) {
						GameObject* o = new GameObject(-1, gameObjectType);
						PositionComponent* po = o->getPositionComponent();
						po->setPosition(sf::Vector2f(positionX, positionY));

						if (po->getBodyType() == BodyType::CIRCLE) {
							grid->setWall(sf::Vector2f(positionX, positionY),
							              sf::Vector2f(po->getSize().x, po->getSize().x));
						}

						if (po->getBodyType() == BodyType::RECTANGLE) {
							grid->setWall(sf::Vector2f(positionX, positionY + po->getSize().y / 2), po->getSize());
						}


						addGameObject(o);
					}


				}
				else {
					float positionX = (float)gameObject["x"].get<json::number_float_t>();
					float positionY = (float)gameObject["y"].get<json::number_float_t>();
					float width = (float)gameObject["width"].get<json::number_float_t>();
					float height = (float)gameObject["height"].get<json::number_float_t>();

					grid->setWall(sf::Vector2f(positionX, positionY + height / 2), sf::Vector2f(width, height));

					if (width > 0 && height > 0) {
						Collider* c = new Collider(-1);
						c->getPositionComponent()->setPosition(
							sf::Vector2f(positionX + width / 2, positionY + height / 2));
						c->getPositionComponent()->setBodyType(BodyType::RECTANGLE);
						c->getPositionComponent()->setSize(sf::Vector2f(width, height));
						c->getPositionComponent()->setMovement(sf::Vector2f(0, 0));

						addCollider(c);
					}

				}
			}
		}
	}

	delete contactListener;
	contactListener = new MapContactListener();
	world->SetContactListener(contactListener);

	grid->initNeighbours();

	subscribe();
}

void Map::subscribe() {
	EventDispatcher<EventCharacterMapJoin>::addSubscriber(this);
	EventDispatcher<EventCharacterMapLeave>::addSubscriber(this);
	EventDispatcher<EventSpellCastResult>::addSubscriber(this);
}

void Map::unsubscribe() {
	EventDispatcher<EventCharacterMapJoin>::removeSubscriber(this);
	EventDispatcher<EventCharacterMapLeave>::removeSubscriber(this);
	EventDispatcher<EventSpellCastResult>::removeSubscriber(this);
}

Player* Map::getPlayer() const {
	return player;
}
