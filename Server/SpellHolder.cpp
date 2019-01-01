#include "SpellHolder.h"

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include <regex>
#include <spdlog/spdlog.h>


s::SpellHolder::SpellHolder():
	idManager(0, 214748364) {
	prototypes.clear();
	
}

s::SpellHolder::~SpellHolder() {
	
}

void s::SpellHolder::init()
{
	sf::Lock mutexLock(lock);

	prototypes.clear();

	spdlog::get("log")->info("Started loading spell prototypes");

	std::vector<std::string> files;
	std::string directory = "./Data/Spells/*.json";

	read_directory(directory, files);
	for (const std::string& f : files) {
		std::string file = f;

		file = std::regex_replace(file, std::regex("\\.json"), "");
		json jsonData = JsonLoader::instance()->loadJson("Spells/" + file);

		SpellInfo* si = new SpellInfo();
		si->id = (int)jsonData["id"].get<json::number_integer_t>();
		si->name = jsonData["name"].get<json::string_t>();
		si->manaCost = (float)jsonData["mana"].get<json::number_float_t>();
		si->castingTime = sf::milliseconds((int)jsonData["castingTime"].get<json::number_integer_t>());
		si->cooldownTime = sf::milliseconds((int)jsonData["cooldown"].get<json::number_integer_t>());

		spellInfos.insert(std::make_pair(si->id, si));

		Spell* spell = new Spell();
		spell->loadFromJson(jsonData);

		prototypes.insert(std::make_pair(spell->spellInfo.id, spell));
	}

	spdlog::get("log")->info("Loading spell prototypes done");
}

int s::SpellHolder::generateInstanceId()
{
	sf::Lock mutexLock(lock);
	int id = idManager.getId();
	return id;
}

void s::SpellHolder::freeInstanceId(int id) {
	sf::Lock mutexLock(lock);
	idManager.freeId(id);
}

s::Spell* s::SpellHolder::createSpell(int type)
{
	sf::Lock mutexLock(lock);

	auto found = prototypes.find(type);
	if (found != prototypes.end()) {
		Spell* spell = found->second->clone();
		spell->setInstanceId(idManager.getId());
		return spell;
	}

	throw "SpellHolder: Spell not found " + std::to_string(type);
}

s::SpellInfo * s::SpellHolder::getSpellInfo(int type)
{
	sf::Lock mutexLock(lock);

	auto found = spellInfos.find(type);
	if (found != spellInfos.end()) {
		return found->second;
	}

	throw "SpellHolder: SpellInfo not found " + std::to_string(type);

}

void s::SpellHolder::read_directory(std::string pattern, std::vector<std::string>& v) const
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