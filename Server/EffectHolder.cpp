#include "EffectHolder.h"

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include <regex>
#include <spdlog/spdlog.h>
#include "EffectModifyAttributes.h"


s::EffectHolder::EffectHolder():
	idManager(0, 214748364) {
	prototypes.clear();

}

s::EffectHolder::~EffectHolder() { }

void s::EffectHolder::init() {
	lock.lock();

	prototypes.clear();

	spdlog::get("log")->info("Started loading effect prototypes");

	std::vector<std::string> files;
	std::string directory = "./Data/Effects/*.json";

	read_directory(directory, files);
	for (const std::string& f : files) {
		std::string file = f;

		file = std::regex_replace(file, std::regex("\\.json"), "");
		json jsonData = JsonLoader::instance()->loadJson("Effects/" + file);

		Effect* effect;
		int id = (int)jsonData["type"].get<json::number_integer_t>();
		switch (id) {
			case 1:
				effect = new EffectModifyAttributes();
				break;
			default:
				throw "Cannot load effect from file " + file;

		}

		effect->loadFromJson(jsonData);
		prototypes.insert(std::make_pair(effect->getId(), effect));
	}

	spdlog::get("log")->info("Loading effects prototypes done");

	lock.unlock();
}

int s::EffectHolder::generateInstanceId() {
	lock.lock();
	int id = idManager.getId();
	lock.unlock();
	return id;
}

void s::EffectHolder::freeInstanceId(int id) {
	lock.lock();
	idManager.freeId(id);
	lock.unlock();
}

s::Effect* s::EffectHolder::createEffect(int type) {
	lock.lock();

	auto found = prototypes.find(type);
	if (found != prototypes.end()) {
		Effect* effect = found->second->clone();
		lock.unlock();
		return effect;
	}

	lock.unlock();
	throw "EffectHolder: EffectHolder not found " + std::to_string(type);
}

void s::EffectHolder::read_directory(std::string pattern, std::vector<std::string>& v) const {
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
