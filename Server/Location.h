#ifndef S_LOCATION_H
#define S_LOCATION_H
#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>

namespace s {
	class Location
	{
		int id;
		b2PolygonShape* shape;
		b2Vec2* vertices;
		b2AABB rect;
		b2Transform transform;
		int verticesCount;
		
	public:
		Location(int id, b2Vec2* vertices, int verticesCount);
		~Location();

		sf::Vector2f generateRandomPoint() const;
	};
}
#endif