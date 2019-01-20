#include "Spawn.h"
#include "Location.h"
#include <spdlog/spdlog.h>
#include "Random.h"
#include "EventNpcChanges.h"
#include "EventNpcStatusChanged.h"
#include "EventAttributesChanged.h"
#include "NpcEventNpcIsIdle.h"
#include "EventDispatcher.h"

s::Spawn::Spawn(int npcType, int maxSpawnedNpcs, Location* l) {
	this->npcType = npcType;
	this->location = l;
	this->maxSpawnedNpcs = maxSpawnedNpcs;
}


s::Spawn::~Spawn()
{
}

void s::Spawn::init(Server *s)
{
	spawnedNpcs.clear();
	spawnedNpcs.reserve(maxSpawnedNpcs);

	try {
		for (int i = 0; i < maxSpawnedNpcs; i++) {	
			Npc* npc = s->npcManager.createNpc(npcType);
			spawnedNpcs.push_back(npc);

			npc->position.setLocation(location);
			npc->position.setPosition(location->generateRandomPoint());

			location->getMap()->addNpc(npc);

			int prob = Random::instance()->randomUniformInt(0, 100);
			if (true) {
				npc->setDeadTimestamp(sf::seconds(s->getServerTime().asSeconds() - npc->getRespawnTime().asSeconds() * Random::instance()->randomUniformFloat(0.1f, 0.9f)));
				npc->setNpcState(NpcState::DEAD);
			} else {
				NpcEventNpcIsIdle* e = new NpcEventNpcIsIdle();
				e->npc = npc;
				EventDispatcher<NpcEventNpcIsIdle>::dispatchEvent(e, s);
			}	
		}
	} catch(...) {
		spawnedNpcs.clear();
		spdlog::get("log")->error("Initing spawn failed! NpcType:{}", npcType);
	}
}

void s::Spawn::update(sf::Time elapsedTime, s::Server* s, Location* l) {
	sf::Time serverTime = s->getServerTime();

	for (Npc* npc : spawnedNpcs) {
		if (npc->getNpcState() == NpcState::DEAD) {
			l->checkNpcRespawn(npc, serverTime, s);
		}
	}

}
