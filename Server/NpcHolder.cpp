#include "NpcHolder.h"

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include <regex>
#include <spdlog/spdlog.h>


s::NpcHolder::NpcHolder():
	idManager(0, 214748364) {
	npcPrototypes.clear();
	
}

s::NpcHolder::~NpcHolder() {
	
}

void s::NpcHolder::init()
{
	lock.lock();

	npcPrototypes.clear();

	spdlog::get("log")->info("Started loading npc prototypes");

	std::vector<std::string> files;
	std::string directory = "./Data/Npcs/*.json";

	read_directory(directory, files);
	for (const std::string& f : files) {
		std::string file = f;

		file = std::regex_replace(file, std::regex("\\.json"), "");

		Npc* npc = new Npc();
		npc->loadFromJson("Npcs/" + file);

		npcPrototypes.insert(std::make_pair(npc->getType(), npc));
	}

	spdlog::get("log")->info("Loading npc prototypes done");

	lock.unlock();
}

int s::NpcHolder::generateSpawnId()
{
	lock.lock();
	int id = idManager.getId();
	lock.unlock();
	return id;
}

void s::NpcHolder::freeSpawnId(int id) {
	lock.lock();
	idManager.freeId(id);
	lock.unlock();
}

s::Npc * s::NpcHolder::createNpc(int type)
{
	lock.lock();

	auto found = npcPrototypes.find(type);
	if (found != npcPrototypes.end()) {
		Npc* npc = found->second->clone();
		npc->setSpawnId(idManager.getId());
		lock.unlock();
		return npc;
	}

	lock.unlock();
	throw "NpcHolder: Npc not found " + std::to_string(type);
}

void s::NpcHolder::read_directory(std::string pattern, std::vector<std::string>& v) const
{
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			v.push_back(data.cFileName);
		}
		while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}