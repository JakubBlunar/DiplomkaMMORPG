#include "EntityPosition.h"
#include "Box2D/Box2D.h"
#include <SFML/System/Vector2.hpp>
#include "ServerGlobals.h"
#include "Map.h"


s::EntityPosition::EntityPosition()
{
	body = nullptr;
	position = sf::Vector2f(0, 0);
	lastMovement = sf::Vector2f(0, 0);
	location = nullptr;
	map = nullptr;
	movement = sf::Vector2f(0, 0);
	size = sf::Vector2i(0, 0);
	mapId = -1;
}

s::Map* s::EntityPosition::getMap() const {
	return map;
}

void s::EntityPosition::setMap(Map* map) {
	if (map) {
		mapId = map->getId();
	}
	this->map = map;
}

void s::EntityPosition::setBody(b2Body* body) {
	this->body = body;
}
b2Body* s::EntityPosition::getBody() const {
	return body;
}

void s::EntityPosition::setSize(sf::Vector2i size) {
	this->size = size;
}

sf::Vector2i s::EntityPosition::getSize() const {
	return size;
}

void s::EntityPosition::setPosition(sf::Vector2f position) {
	this->position = position;
	if (this->body) {
		this->body->SetTransform(b2Vec2(position.x * PIXTOMET, position.y * PIXTOMET), this->body->GetAngle());
	}
}
sf::Vector2f s::EntityPosition::getPosition() const {
	return position;
}

sf::Vector2f s::EntityPosition::getLastMovement() const {
	return lastMovement;
}

void s::EntityPosition::setLocation(Location* l) {
	this->location = l;
}
s::Location* s::EntityPosition::getLocation() const {
	return location;
}
