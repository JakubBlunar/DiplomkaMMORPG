#include "Npc.h"
#include "JsonLoader.h"

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

void s::Npc::loadFromJson(std::string file)
{
	json jsonData = JsonLoader::instance()->loadJson(file);

	name = jsonData["name"].get<json::string_t>();
	type = (int) jsonData["type"].get<json::number_integer_t>();
	speed = (float) jsonData["speed"].get<json::number_float_t>();

	std::string renderFile = jsonData["render"].get<json::string_t>();
	json renderData = JsonLoader::instance()->loadJson("Graphics/Npcs/" + renderFile);


	json animationData = JsonLoader::instance()->loadJson("Graphics/Characters/" + std::to_string(type));

	int width = (int)animationData["width"].get<json::number_integer_t>();

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
	this->movement = movement;	
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
