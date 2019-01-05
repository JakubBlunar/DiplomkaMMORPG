#ifndef ENTITY_PROTOTYPES_H
#define ENTITY_PROTOTYPES_H

#include "json.hpp"
#include "Spell.h"
using json = nlohmann::json;

class EntityPrototypes {
public:
	void init();

	EntityPrototypes(const EntityPrototypes&) = delete;
	EntityPrototypes& operator=(const EntityPrototypes) = delete;

	static EntityPrototypes* instance() {
		static EntityPrototypes instance;
		return &instance;
	}

	Spell* createSpell(int type);
	SpellInfo* getSpellInfo(int type);

	void read_directory(std::string pattern, std::vector<std::string>& v) const;
private:
	EntityPrototypes();
	~EntityPrototypes();

	std::map<int, Spell*> spellPrototypes;
	std::map<int, SpellInfo*> spellInfoPrototypes;
};



#endif
