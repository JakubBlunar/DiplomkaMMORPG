#include "NpcManager.h"
#include "NpcHolder.h"
#include "ServerGlobals.h"
#include "Random.h"
#include "NpcCommandMoveTo.h"
#include "Location.h"
#include "NpcCommandStay.h"


s::NpcManager::NpcManager()
{
	dynamic = false;
}


s::NpcManager::~NpcManager()
{
}

s::Npc* s::NpcManager::createNpc(int npcType) {
	lock.lock();
	NpcHolder* nh = NpcHolder::instance();
	Npc* npc;
	try {
		npc = nh->createNpc(npcType);
		int spawnId = npc->getSpawnId();
		if (npcs.find(spawnId) == npcs.end()) {
			npcs.insert(std::make_pair(spawnId, npc));
		} else {
			throw "Duplicate spawn id" + std::to_string(spawnId);
		}
	} catch(...) {
		lock.unlock();
		throw "Cannot create npc" + std::to_string(npcType);
	}

	lock.unlock();
	return npc;
}

s::Npc* s::NpcManager::findNpc(int spawnId)
{
	lock.lock();
	auto find = npcs.find(spawnId);
	if (find != npcs.end()) {
		lock.unlock();
		return find->second;
	}
	lock.unlock();
	return nullptr;
}

void s::NpcManager::updateNpc(sf::Time elapsedTime, Npc* npc, Server * s, NpcUpdateEvents * npcUpdateEvents) const {
	if (!npc->isAlive()) {
		return;
	}
	b2Body* body = npc->getBody();
	b2Vec2 position = body->GetPosition();
	
	sf::Vector2f transformedPosition = sf::Vector2f(position.x * METTOPIX, position.y * METTOPIX); 

	npc->setPosition(transformedPosition);

	NpcCommand* command = npc->getNpcCommand();
	if (!command || command->isFinished()) {
		delete command;

		Random* rand = Random::instance();
		if (rand->randomUniformFloat(0, 100) < 45) {
			Location * l = npc->getLocation();
			if (l) {
				NpcCommandMoveTo* newCommand = new NpcCommandMoveTo(l->generateRandomPoint(), npc, npc->getMap(), s,  sf::seconds(30));
				newCommand->init();
				npc->setNpcCommand(newCommand);
			}
		} else {
			NpcCommandStay* newCommand = new NpcCommandStay(npc, npc->getMap(), s,  sf::seconds(rand->randomUniformFloat(10, 30)));
			npc->setNpcCommand(newCommand);
		}
	} else {
		command->update(elapsedTime, npcUpdateEvents);
	}
}


