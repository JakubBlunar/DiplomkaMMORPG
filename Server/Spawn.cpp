#include "Spawn.h"
#include "Location.h"
#include <spdlog/spdlog.h>
#include "Random.h"
#include "EventNpcChanges.h"
#include "EventNpcStatusChanged.h"
#include "EventNpcAttributesChanged.h"

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

			npc->setLocation(location);
			npc->setPosition(location->generateRandomPoint());

			location->getMap()->addNpc(npc);

			int prob = Random::instance()->randomUniformInt(0, 100);
			if (prob < 20) {
				npc->setDeadTimestamp(sf::seconds(s->getServerTime().asSeconds() - npc->getRespawnTime().asSeconds() * Random::instance()->randomUniformFloat(0.1f, 0.9f)));
				npc->setNpcState(NpcState::DEAD);
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
			sf::Time respawnTime = npc->getRespawnTime();
			sf::Time deadTimestamp = npc->getDeadTimestamp();
			if (serverTime.asSeconds() - respawnTime.asSeconds() >= deadTimestamp.asSeconds()) {
				npc->setNpcState(NpcState::IDLE);
				npc->setDeadTimestamp(sf::Time::Zero);

				float hp = npc->getAttribute(EntityAttributeType::BASE_HP);
				float mp = npc->getAttribute(EntityAttributeType::BASE_MP);

				npc->setAttribute(EntityAttributeType::HP, hp);
				npc->setAttribute(EntityAttributeType::MP, mp);

				EventNpcAttributesChanged attributeChanges;
				attributeChanges.spawnId = npc->getSpawnId();
				attributeChanges.setChange(EntityAttributeType::HP, hp);
				attributeChanges.setChange(EntityAttributeType::MP, mp);
				l->getMap()->sendEventToAllPlayers(&attributeChanges);

				EventNpcStatusChanged statusChange;
				statusChange.spawnId = npc->getSpawnId();
				statusChange.npcState = npc->getNpcState();
				l->getMap()->sendEventToAllPlayers(&statusChange);
			}
		}
	}

}
