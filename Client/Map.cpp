#include "Map.h"
#include "Game.h"
#include "Globals.h"
#include "b2GLDraw.h"
#include "Box2DTools.h"
#include "GameObject.h"
#include "JsonLoader.h"
#include "Collider.h"


Map::Map(Game* game) {
	this->game = game;
}


Map::~Map() {
	delete world;
}

void Map::init() {
	fields = new Matrix<Field>(width, height);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			fields->set(i, j, new Field());
		}
	}

	GameObject* gameObject = new GameObject(1, "GameObjects/largeTree");
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
}

void Map::update(sf::Time elapsedTime, Game* game) {
	world->Step(elapsedTime.asSeconds(), 6, 2);
	for (Entity* entity : entities) {
		entity->update(elapsedTime, this);
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


void Map::addPlayer(Player* player) {
	players.push_back(player);
	entities.push_back(player);
	sf::Vector2f position = player->getPosition();
	sf::Vector2f size = player->getSize();
	Box2DTools::addCircle(b2_dynamicBody, position.x, position.y, size.x, player, this);
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
		Box2DTools::addBox(b2_kinematicBody, position.x, position.y, size.x, size.y, gameObject, this);
	}
	else {
		Box2DTools::addCircle(b2_kinematicBody, position.x, position.y, size.x, gameObject, this);
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
		Box2DTools::addBox(b2_staticBody, position.x, position.y, size.x, size.y, collider, this);
	}
	else {
		Box2DTools::addCircle(b2_staticBody, position.x, position.y, size.x, collider, this);
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

	this->id = (int)mapData["id"].get<json::number_integer_t>();
	width = (int)mapData["width"].get<json::number_integer_t>();
	height = (int)mapData["height"].get<json::number_integer_t>();

	init();
}
