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

Map::Map(Game* game) {
	this->game = game;
}


Map::~Map() {
	unsubscribe();

	players.clear();
	entities.clear();
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

void Map::handleEvent(GameEvent* event) {
	switch (event->getId()) {
		case CHARACTER_MAP_JOIN: {
			EventCharacterMapJoin* e = (EventCharacterMapJoin*) event;
			if(e->mapId != id)
				return;

			json characterData = json::parse(e->characterData);
			Player* p = new Player(false);
			p->loadFromJson(characterData);

			auto exists = std::find_if(players.begin(), players.end(),
			                              [=](Player* player)-> bool {
				                              if (player->getId() == p->getId()) {
					                              return true;
				                              }
				                              return false;
			                              });
			if(exists != players.end()) {
				return;
			}

			addPlayer(p);
		}
		case CHARACTER_MAP_LEAVE: {
			EventCharacterMapLeave* e = (EventCharacterMapLeave*) event;
			if(e->mapId != id)
				return;

			auto exists = std::find_if(players.begin(), players.end(),
			                              [=](Player* player)-> bool {
				                              if (player->getId() == e->characterId) {
					                              return true;
				                              }
				                              return false;
			                              });
			if(exists == players.end()) {
				return;
			}

			Player* p = *exists;
			removePlayer(p);
			delete p;
		}
		default:
			break;
	}

}


void Map::addPlayer(Player* player) {
	players.push_back(player);
	entities.push_back(player);
	sf::Vector2f position = player->getPosition();
	sf::Vector2f size = player->getSize();
	Box2DTools::addCircle(b2_dynamicBody, position.x, position.y, size.x, player, this, player->getEntityCategory(), player->getCollisionMask());
}

void Map::removePlayer(Player* player) {
	world->DestroyBody(player->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), player), entities.end());
	players.erase(std::remove(players.begin(), players.end(), player), players.end());
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
		Box2DTools::addBox(b2_kinematicBody, position.x, position.y, size.x, size.y, gameObject, this, gameObject->getEntityCategory(), gameObject->getCollisionMask());
	}
	else {
		Box2DTools::addCircle(b2_kinematicBody, position.x, position.y, size.x, gameObject, this, gameObject->getEntityCategory(), gameObject->getCollisionMask());
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
		Box2DTools::addBox(b2_staticBody, position.x, position.y, size.x, size.y, collider, this, collider->getEntityCategory(), collider->getCollisionMask());
	}
	else {
		Box2DTools::addCircle(b2_staticBody, position.x, position.y, size.x, collider, this, collider->getEntityCategory(), collider->getCollisionMask());
	}
}


void Map::removeGameObject(GameObject* gameObject) {
	world->DestroyBody(gameObject->getBody());
	entities.erase(std::remove(entities.begin(), entities.end(), gameObject), entities.end());
}

void Map::loadFromFile(int id) {
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

	json mapData = JsonLoader::instance()->loadJson("Maps/" + std::to_string(id));
	json mapProperties = mapData["properties"].get<json::object_t>();

	id = (int)mapProperties["id"].get<json::number_integer_t>();
	width = (int)mapData["width"].get<json::number_integer_t>();
	height = (int)mapData["height"].get<json::number_integer_t>();

	fields = new Matrix<Field>(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			fields->set(i, j, new Field());
		}
	}

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

		vector<string> splittedPath = Utils::splitPath(path, {'/'});
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

		std::string type = layer["type"].get<json::string_t>();

		if (type == "tilelayer") {
			int width = (int) layer["width"].get<json::number_integer_t>();
			int height = (int) layer["width"].get<json::number_integer_t>();

			json data = layer["data"].get<json::array_t>();

			int counter = 0;
			for (json::iterator fieldValueIterator = data.begin(); fieldValueIterator != data.end(); fieldValueIterator++) {
				int value = *fieldValueIterator;
				if (value > 0) {
					int x = counter % width;
					int y = counter / width;

				
					TileSet foundTileSet;
					foundTileSet.valid = false;
					foundTileSet.columns = 1;

					int tilesetIdOffset = 0;

					for (std::map<int, TileSet>::iterator tilesetIterator = tilesets.begin(); tilesetIterator != tilesets.end(); tilesetIterator++)
					{
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
						sprite->load(foundTileSet.path, sf::Vector2i(FIELD_SIZE, FIELD_SIZE), sf::Vector2i(tileX * FIELD_SIZE, tileY * FIELD_SIZE));

						fields->get(x, y)->addLayer(sprite);
					}

				}
				counter++;
			}
		}
		
	}




	subscribe();
}

void Map::subscribe()
{
	EventDispatcher<EventCharacterMapJoin>::addSubscriber(this);
	EventDispatcher<EventCharacterMapLeave>::addSubscriber(this);
}

void Map::unsubscribe() {
	EventDispatcher<EventCharacterMapJoin>::removeSubscriber(this);
	EventDispatcher<EventCharacterMapLeave>::removeSubscriber(this);
}
