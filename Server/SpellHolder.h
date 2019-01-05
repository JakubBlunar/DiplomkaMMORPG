#ifndef S_SPELL_HOLDER_H
#define S_SPELL_HOLDER_H

#include "json.hpp"
#include "IDManager.h"
#include "SFML/System.hpp"

using json = nlohmann::json;

namespace s {
	class Spell;
	struct SpellInfo;

	class SpellHolder {
	public:
		void init();
		int generateInstanceId();
		void freeInstanceId(int id);

		SpellHolder(const SpellHolder&) = delete;
		SpellHolder& operator=(const SpellHolder) = delete;

		static SpellHolder* instance() {
			static SpellHolder instance;
			return &instance;
		}

		Spell* createSpell(int type);
		SpellInfo* getSpellInfo(int type);

		void read_directory(std::string pattern, std::vector<std::string>& v) const;
	private:
		SpellHolder();
		~SpellHolder();

		std::map<int, Spell*> prototypes;
		std::map<int, SpellInfo*> spellInfos;
		IDManager<sf::Int32> idManager;

		sf::Mutex lock;
	};
}


#endif
