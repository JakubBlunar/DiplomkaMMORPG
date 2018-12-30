#ifndef MAP_H
#define MAP_H

#include <Box2D/Dynamics/b2World.h>
#include <SFML/System/Time.hpp>
#include "Field.h"
#include "Matrix.h"
#include "Player.h"
#include "GameObject.h"
#include "MapContactListener.h"
#include "MapGrid.h"
#include "Npc.h"
#include "EntityToEntityRayCast.h"

class Spell;
class GameEvent;
class Collider;
class Game;

class Map : public Subscriber {
	Game* game;

	int width, height;
	Matrix<Field>* fields;
	b2World* world;
	std::vector<Player*> players;
	std::vector<Entity*> entities;
	std::vector<Npc*> npcs;

	b2Draw* debugDrawInstance;
	int id;

	Player* player;
	MapContactListener* contactListener;
	MapGrid* grid;
public:
	Map(Game* g);
	~Map();

	void init();

	void update(sf::Time elapsedTime, Game* game);

	void addPlayer(Player* player);
	void removePlayer(Player* player);
	void addGameObject(GameObject* gameObject);
	void addCollider(Collider* collider);
	void removeGameObject(GameObject* gameObject);
	void addNpc(Npc* npc);
	void removeNpc(Npc* npc);

	void addSpell(Spell* spell);
	void removeSpell(Spell* spell);

	void loadFromFile(int id);

	void subscribe();
	void unsubscribe();

	Player* getPlayer() const;

	Field* getField(int x, int y) const;
	int getWidth() const;
	int getHeight() const;
	b2World* getB2World() const;
	MapGrid* getGrid() const;

	EntityToEntityRayCast* makeRayCast(Entity* startEntity, Entity* endEntity) const;

	std::vector<Entity*>* getEntities();

	void handleEvent(GameEvent* event) override;
};

#endif
