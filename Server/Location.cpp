#include "Location.h"
#include "Box2D/Box2D.h"
#include "Random.h"


s::Location::Location(int id, b2Vec2* vertices, int verticesCount)
{
	this->id = id;
	this->vertices = vertices;
	this->verticesCount = verticesCount;
  
	shape = new b2PolygonShape();
	shape->Set(this->vertices, this->verticesCount);
	transform.Set(b2Vec2(0, 0), 0);

	shape->ComputeAABB(&rect, transform,  1);
}


s::Location::~Location()
{
	delete shape;
	delete vertices;
}

sf::Vector2f s::Location::generateRandomPoint() const {
	if(!shape) {
		return sf::Vector2f(-1000, -1000);
	}

	Random* rand = Random::instance();

	b2Vec2 point;
	do {
		point.x = rand->randomUniformFloat(rect.lowerBound.x, rect.upperBound.x);
		point.y = rand->randomUniformFloat(rect.lowerBound.y, rect.upperBound.y);
	} while(shape->TestPoint(transform, point));

	return sf::Vector2f(point.x, point.y);
}
