#ifndef MAP_H
#define MAP_H

#include <Box2D/Dynamics/b2World.h>
#include <SFML/System/Time.hpp>
#include "Field.h"
#include "Matrix.h"
#include "Player.h"
#include "GameObject.h"
#include "MapGrid.h"
#include "Npc.h"
#include "EntityToEntityRayCast.h"
#include "Map.h"
#include <queue>
#include "MapContactListener.h"

class Spell;
class GameEvent;
class Collider;
class Game;

class Map : public Subscriber {
	Game* game;

	int width, height;
	Matrix<Field>* fields;
	b2World* world;
	std::map<sf::Uint32, Player*> players;
	std::vector<Entity*> entities;
	std::map<sf::Uint32, Npc*> npcs;
	std::queue<Spell*> spellsToRemove;

	b2Draw* debugDrawInstance;
	int id;

	Player* player;
	MapContactListener* contactListener;
	MapGrid* grid;

	bool pvp;
	sf::Mutex mapLock;
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
	Player* getPlayerById(int id) const;

	Field* getField(int x, int y);
	int getWidth() const;
	int getHeight() const;
	b2World* getB2World() const;
	MapGrid* getGrid() const;

	EntityToEntityRayCast* makeRayCast(Entity* startEntity, Entity* endEntity);

	std::vector<Entity*>* getEntities();

	void handleEvent(GameEvent* event) override;
	bool pvpEnabled() const {
		return pvp;
	}
};

#endif
