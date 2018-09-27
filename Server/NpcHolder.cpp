#include "NpcHolder.h"
#include <windows.h>
#include <regex>
#include <spdlog/spdlog.h>


s::NpcHolder::NpcHolder() {
	npcPrototypes.clear();

	
}

s::NpcHolder::~NpcHolder() {
	
}

void s::NpcHolder::init()
{
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