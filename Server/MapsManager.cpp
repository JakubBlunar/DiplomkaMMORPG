#include "MapsManager.h"
#include "Map.h"
#include <spdlog/spdlog.h>
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include <regex>

s::MapsManager::MapsManager() {
	dynamic = true;
}


s::MapsManager::~MapsManager() {
}

s::Map* s::MapsManager::getMap(int id) {
	auto it = maps.find(id);
	if (it != maps.end()) {
		return it->second;
	}
	return nullptr;
}

void s::MapsManager::update(sf::Time elapsedTime, s::Server* s) {
	for (std::map<int, s::Map*>::iterator it = maps.begin(); it != maps.end(); ++it) {
		it->second->update(elapsedTime, s);
	}
}

void s::MapsManager::init(Server *s) {
	spdlog::get("log")->info("Started loading maps");

	std::vector<std::string> files;
	std::string directory = "./Data/Maps/*.json";

	read_directory(directory, files);
	for (const std::string& f : files) {
		std::string file = f;

		file = std::regex_replace(file, std::regex("\\.json"), "");
		Map* m = new Map();
		m->loadFromJson("Maps/" + file, s);

		maps.insert(std::make_pair(m->getId(), m));
	}

	spdlog::get("log")->info("Loading maps done");
}

void s::MapsManager::read_directory(const std::string pattern, std::vector<std::string>& v) const {
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
