#include "Spawn.h"
#include "NpcHolder.h"
#include "Location.h"
#include <spdlog/spdlog.h>

s::Spawn::Spawn(int npcType, int maxSpawnedNpcs, Location* l) {
	this->npcType = npcType;
	this->location = l;
	this->maxSpawnedNpcs = maxSpawnedNpcs;
}


s::Spawn::~Spawn()
{
}

void s::Spawn::init()
{
	spawnedNpcs.clear();
	spawnedNpcs.reserve(maxSpawnedNpcs);

	NpcHolder* nh = NpcHolder::instance();

	try {
		for (int i = 0; i < maxSpawnedNpcs; i++) {	
			Npc* npc = nh->createNpc(npcType);
			spawnedNpcs.push_back(npc);

			npc->setPosition(location->generateRandomPoint());

			location->getMap()->addNpc(npc);
		}
	} catch(...) {
		spawnedNpcs.clear();
		spdlog::get("log")->error("Initing spawn failed! NpcType:{}", npcType);
	}
}

void s::Spawn::update(sf::Time elapsedTime, s::Server* s, Location* l) {
	lastUpdate += elapsedTime;

	


}
