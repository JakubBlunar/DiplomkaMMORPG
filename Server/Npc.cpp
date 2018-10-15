#include "Npc.h"
#include "JsonLoader.h"
#include "ServerGlobals.h"
#include "EventNpcMovementChange.h"

s::Npc::Npc() {
	body = nullptr;
	name = "";
	spawnId = -1;
	speed = 0;
	type = 0;
	size = sf::Vector2i(0, 0);
	movement = sf::Vector2f(0, 0);
	map = nullptr;
	body = nullptr;
}


s::Npc::~Npc() {
	
}

s::NpcCommand* s::Npc::getNpcCommand() const {
	return command;
}
void s::Npc::setNpcCommand(NpcCommand* command) {
	this->command = command;
}

void s::Npc::setSpawnId(int id)
{
	this->spawnId = id;
}

int s::Npc::getSpawnId() const {
	return spawnId;
}

void s::Npc::loadFromJson(std::string file)
{
	json jsonData = JsonLoader::instance()->loadJson(file);

	name = jsonData["name"].get<json::string_t>();
	type = (int) jsonData["type"].get<json::number_integer_t>();
	speed = (float) jsonData["speed"].get<json::number_float_t>();

	std::string renderFile = jsonData["render"].get<json::string_t>();
	json renderData = JsonLoader::instance()->loadJson("Graphics/Npcs/" + renderFile);

	int width = (int)renderData["width"].get<json::number_integer_t>();

	size = sf::Vector2i(width, width);
	movement = sf::Vector2f(0, 0);

}

s::Npc* s::Npc::clone() const {
	Npc* copy = new Npc();

	copy->setSize(size);
	copy->setMovement(sf::Vector2f(0, 0));
	copy->setType(type);
	copy->setName(name);
	copy->setSpeed(speed);
	copy->setBody(nullptr);
	copy->setMap(nullptr);

	return copy;
}

json s::Npc::toJson() const {
	json json;

	json["spawnId"] = spawnId;
	json["name"] = name;
	json["type"] = type;
	json["movementX"] = movement.x;
	json["movementY"] = movement.y;
	json["positionX"] = position.x;
	json["positionY"] = position.y;
	json["speed"] = speed;
	if (map) {
		json["mapId"] = map->getId();
	}

	return json;
}

void s::Npc::setBody(b2Body* body) {
	this->body = body;
}

b2Body* s::Npc::getBody() const {
	return body;
}

void s::Npc::setSpeed(float speed) {
	this->speed = speed;
}

float s::Npc::getSpeed() const {
	return speed;
}

void s::Npc::setMap(Map* map) {
	this->map = map;
}

s::Map* s::Npc::getMap() const {
	return map;
}

void s::Npc::setName(string name) {
	this->name = name;
}

string s::Npc::getName() const {
	return name;
}

void s::Npc::setMovement(sf::Vector2f movement) {
	if (lastMovement == movement) {
		return;
	}

	this->lastMovement = this->movement;
	this->movement = movement;

	if (body) {
		body->SetLinearVelocity(b2Vec2(movement.x * PIXTOMET, movement.y * PIXTOMET));

		if (map) {
			EventNpcMovementChange e;
			e.spawnId = spawnId;
			e.speed = speed;
			e.velX = this->movement.x;
			e.velY = this->movement.y;
			e.x = position.x;
			e.y = position.y;

			map->sendEventToAllPlayers(&e);
		}
	}
}
sf::Vector2f s::Npc::getMovement() const {
	return movement;
}

void s::Npc::setSize(sf::Vector2i size) {
	this->size = size;
}
sf::Vector2i s::Npc::getSize() const {
	return size;
}

void s::Npc::setPosition(sf::Vector2f position) {
	this->position = position;
}

sf::Vector2f s::Npc::getPosition() const {
	return position;
}

void s::Npc::setType(int type) {
	this->type = type;
}

int s::Npc::getType() const
{
	return type;
}

sf::Vector2f s::Npc::getLastMovement() const {
	return lastMovement;
}

void s::Npc::setMovementDirection(sf::Vector2f direction, float speed) {
	this->speed = speed;
	this->setMovement(sf::Vector2f(direction.x * speed, direction.y * speed));
}

void s::Npc::setLocation(Location* l) {
	this->location = l;
}
s::Location* s::Npc::getLocation() const {
	return location;
}
