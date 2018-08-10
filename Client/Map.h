#pragma once
#include <Box2D/Dynamics/b2World.h>
#include <SFML/System/Time.hpp>
#include "Field.h"
#include "Matrix.h"
#include "Player.h"
#include "GameObject.h"

class Collider;
class Game;

class Map
{
	int width, height;
	Matrix<Field>* fields;
	b2World* world;
	Player* player;
	std::vector<Player*> players;
	std::vector<Entity*> entities;

	b2Draw* debugDrawInstance;
	


public:
	Map(Game* g);
	~Map();

	void init();

	void update(sf::Time elapsedTime, Game* game);

	void addPlayer(Player* player);
	void removePlayer(Player* player);
	void addGameObject(GameObject* gameObject);
	void addCollider(Collider * collider);
	void removeGameObject(GameObject* gameObject);

	Field* getField(int x, int y) const;
	int getWidth() const;
	int getHeight() const;
	b2World* getB2World() const;
};

