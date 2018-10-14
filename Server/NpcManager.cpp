#include "NpcManager.h"
#include "NpcHolder.h"


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


