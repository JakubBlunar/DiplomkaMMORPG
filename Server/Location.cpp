#include "Location.h"
#include "Box2D/Box2D.h"
#include "Random.h"
#include "Spawn.h"
#include "EventAttributesChanged.h"
#include "EventNpcStatusChanged.h"
#include "NpcEventNpcIsIdle.h"
#include "EventDispatcher.h"
#include "EventNpcMovementChange.h"


s::Location::Location(int id, b2Vec2* vertices, int verticesCount, sf::Vector2f position, Map* m)
{
	this->id = id;
	this->vertices = vertices;
	this->verticesCount = verticesCount;
	this->map = m;
	this->position = position;

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
		return sf::Vector2f(40, 40);
	}

	Random* rand = Random::instance();

	b2Vec2 point;
	do {
		point.x = rand->randomUniformFloat(rect.lowerBound.x, rect.upperBound.x);
		point.y = rand->randomUniformFloat(rect.lowerBound.y, rect.upperBound.y);
	} while(!shape->TestPoint(transform, point));

	return sf::Vector2f(point.x, point.y);
}

sf::Vector2f s::Location::getPosition() const {
	return position;
}

s::Map* s::Location::getMap() const {
	return map;
}

void s::Location::addSpawn(Spawn * spawn)
{
	spawns.push_back(spawn);
}

void s::Location::addNpc(Npc * npc)
{
	locationNpcs.push_back(npc);
	npc->position.setLocation(this);
}

void s::Location::update(sf::Time elapsedTime, s::Server * s, Map * map)
{
	sf::Time serverTime = s->getServerTime();

	for (Npc* npc : locationNpcs) {
		if (npc->getNpcState() == NpcState::DEAD) {
			checkNpcRespawn(npc, serverTime, s);
		}
	}

	for (Spawn* spawn : spawns) {
		spawn->update(elapsedTime, s, this);
	}
}

void s::Location::checkNpcRespawn(Npc* npc, sf::Time serverTime, Server* s) const {
	sf::Time respawnTime = npc->getRespawnTime();
	sf::Time deadTimestamp = npc->getDeadTimestamp();
	if (serverTime.asSeconds() - respawnTime.asSeconds() >= deadTimestamp.asSeconds()) {
		if (npc->hasSpawnPosition()) {
			sf::Vector2f spawnPosition = npc->getSpawnPosition();
			npc->position.setPosition(spawnPosition);
			npc->setMovement(0, 0);
			EventNpcMovementChange movChanged;
			movChanged.spawnId = npc->getSpawnId();
			movChanged.speed = npc->attributes.getAttribute(EntityAttributeType::SPEED, true);
			movChanged.velX = 0;
			movChanged.velY = 0;
			movChanged.x = spawnPosition.x;
			movChanged.y = spawnPosition.y;
			map->sendEventToAllPlayers(&movChanged);
		}

		float hp = npc->getAttribute(EntityAttributeType::BASE_HP, true);
		float mp = npc->getAttribute(EntityAttributeType::BASE_MP, true);

		npc->attributes.setAttribute(EntityAttributeType::HP, hp);
		npc->attributes.setAttribute(EntityAttributeType::MP, mp);

		EventAttributesChanged attributeChanges;
		attributeChanges.spawnId = npc->getSpawnId();
		attributeChanges.entityType = NPC;
		attributeChanges.setChange(EntityAttributeType::HP, hp);
		attributeChanges.setChange(EntityAttributeType::MP, mp);
		map->sendEventToAllPlayers(&attributeChanges);

		npc->setNpcState(NpcState::IDLE);
		npc->setDeadTimestamp(sf::Time::Zero);

		EventNpcStatusChanged statusChange;
		statusChange.spawnId = npc->getSpawnId();
		statusChange.npcState = NpcState::IDLE;
		map->sendEventToAllPlayers(&statusChange);

		NpcEventNpcIsIdle* e = new NpcEventNpcIsIdle();
		e->npc = npc;
		EventDispatcher<NpcEventNpcIsIdle>::dispatchEvent(e, s) ;
		//spdlog::get("log")->trace("Npc resurect :{}", npc->getSpawnId());
	}
}
