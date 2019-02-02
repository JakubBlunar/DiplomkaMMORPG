#include "SpellHolder.h"

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

#include <regex>
#include <spdlog/spdlog.h>
#include "Spell.h"
#include "MovableSpell.h"
#include "JsonLoader.h"

s::SpellHolder::SpellHolder():
	idManager(0, 214748364) {
	prototypes.clear();
	
}

s::SpellHolder::~SpellHolder() {
	
}

void s::SpellHolder::init(Server* server)
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
		si->cooldownTime = sf::milliseconds((int)jsonData["cooldown"].get<json::number_integer_t>());
		si->globalCooldownTime = sf::milliseconds((int)jsonData["gCooldown"].get<json::number_integer_t>());
		si->maxRange =  (float)jsonData["maxRange"].get<json::number_float_t>();
		si->type = (SpellType) jsonData["type"].get<json::number_integer_t>();
		si->levelNeeded = (int) jsonData["levelNeeded"].get<json::number_integer_t>();
		spellInfos.insert(std::make_pair(si->id, si));

		auto foundVector = spellInfoByLevel.find(si->levelNeeded);
		if (foundVector == spellInfoByLevel.end()) {
			std::vector<SpellInfo*>* container = new std::vector<SpellInfo*>();
			container->push_back(si);
			spellInfoByLevel.insert(std::make_pair(si->levelNeeded, container));
		} else {
			foundVector->second->push_back(si);
		}
		
		Spell* spell;
		if (jsonData.find("entityAnimation") != jsonData.end()) {
			spell = new MovableSpell();
			si->spellCategory = EntityType::MOVABLE_SPELL;
		} else {
			spell = new Spell();
			si->spellCategory = EntityType::SPELL;
		}

		spell->setServer(server);
		spell->spellInfo = *si;
		spell->loadFromJson(jsonData);


		prototypes.insert(std::make_pair(si->id, spell));
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

s::MovableSpell* s::SpellHolder::createMovableSpell(int type)
{
	sf::Lock mutexLock(lock);

	auto found = prototypes.find(type);
	if (found != prototypes.end()) {
		if (dynamic_cast<MovableSpell*>(found->second) == nullptr) {
			spdlog::get("log")->critical("Spell with type {} is not movable", type);
			throw "Spell with type" + std::to_string(type) + "is not movable"; 
		}

		MovableSpell* spell = (MovableSpell*)found->second->clone();
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

std::vector<s::SpellInfo*>* s::SpellHolder::getSpellsWithLevel(int level) {
	auto found = spellInfoByLevel.find(level);
	if (found != spellInfoByLevel.end()) {
		return found->second;
	}
	return nullptr;
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
