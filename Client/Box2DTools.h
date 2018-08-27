#ifndef Box2DTools_H
#define Box2DTools_H
#include <Box2D/Dynamics/b2Body.h>

class Map;
class Entity;

class Box2DTools {
public:
	static void addBox(b2BodyType bodyType, float x, float y, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits);
	static void addBox(b2BodyType bodyType, float x, float y, float width, float height, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits);

	static void addStaticBox(float x, float y, Entity* entity, Map* map);
	static void addStaticBox(float x, float y, float width, float height, Entity* entity, Map* map);


	static void addCircle(b2BodyType bodyType, float x, float y, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits);
	static void addCircle(b2BodyType bodyType, float x, float y, float radius, Entity* entity, Map* map, int16 categoryBits, uint16 maskBits);
	static void addStaticCircle(float x, float y, Entity* entity, Map* map);
	static void addStaticCircle(float x, float y, float radius, Entity* entity, Map* map);
};

#endif
