#ifndef Box2DTools_H
#define Box2DTools_H

class Map;
class Entity;

class Box2DTools
{
public:
	static void addDynamicBox(float x, float y, Entity* entity, Map* map);
	static void addDynamicBox(float x, float y, float width, float height, Entity* entity, Map* map);
	static void addStaticBox(float x, float y, Entity* entity, Map* map);
	static void addStaticBox(float x, float y, float width, float height, Entity* entity, Map* map);
	static void addDynamicCircle(float x, float y, Entity* entity, Map* map);
	static void addDynamicCircle(float x, float y, float radius, Entity* entity, Map* map);
	static void addStaticCircle(float x, float y, Entity* entity, Map* map);
	static void addStaticCircle(float x, float y, float radius, Entity* entity, Map* map);
};

#endif