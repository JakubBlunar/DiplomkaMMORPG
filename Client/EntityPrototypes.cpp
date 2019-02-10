#include "EntityPrototypes.h"

#define NOMINMAX
#include <windows.h>
#include <regex>
#include "ResourceHolder.h"
#include "JsonLoader.h"
#undef NOMINMAX

EntityPrototypes::EntityPrototypes()
{
}


EntityPrototypes::~EntityPrototypes()
{
}


void EntityPrototypes::init() {
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
		si->description = jsonData["desc"].get<json::string_t>();
		si->globalCooldownTime = sf::milliseconds((int)jsonData["gCooldown"].get<json::number_integer_t>());
		si->maxRange =  (float)jsonData["maxRange"].get<json::number_float_t>();
		si->levelNeeded = (int)jsonData["levelNeeded"].get<json::number_integer_t>();
		si->targetRestriction = static_cast<SpellTargetRestriction>(jsonData["targetRestriction"].get<json::number_integer_t>());

		std::string icon = jsonData["icon"].get<json::string_t>();
		si->icon.load("Graphics/Icons/"+ icon, sf::Vector2i(32,32), sf::Vector2i(0,0));
		spellInfoPrototypes.insert(std::make_pair(si->id, si));

		auto animation = jsonData.find("entityAnimation");
		if (animation != jsonData.end()) {
			si->hasEntity = true;
			Spell* spell = new Spell(-1);
			spell->loadFromJson(jsonData);
			spellPrototypes.insert(std::make_pair(si->id, spell));
		}
	}

}

Spell * EntityPrototypes::createSpell(int type)
{
	auto found = spellPrototypes.find(type);
	if (found != spellPrototypes.end()) {
		return found->second->clone();
	}

	throw "Cannot create spell";
}

SpellInfo * EntityPrototypes::getSpellInfo(int type)
{
	auto found = spellInfoPrototypes.find(type);
	if (found != spellInfoPrototypes.end()) {
		return found->second;
	}

	throw "Spell info not found: " + type;
}

void EntityPrototypes::read_directory(std::string pattern, std::vector<std::string>& v) const {
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
