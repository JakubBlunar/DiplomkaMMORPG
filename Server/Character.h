#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "../Shared/CharacterConstants.h"
#include <vector>
#include "json.hpp"
#include "EntityAttributes.h"
#include "EntityPosition.h"
#include "Spell.h"
#include "EntitySpells.h"

using json = nlohmann::json;

namespace s {
	class Account;
	class Map;
}

namespace s {
	class Character :public Entity {
		Account* account;
	public:
		Character();
		virtual ~Character();
		void setAccount(Account* account);
		Account* getAccount() const;

		EntityAttributes attributes;
		EntityPosition position;
		EntitySpells spells;

		int id;
		std::string name;
		CharacterType type;
		CharacterFaction faction;
		std::vector<SpellInfo*> availableSpells;

		bool isBot;

		bool save() const;
		json toJson() const;

		void update(sf::Time deltaTime, Server* s, Map* map);


		static Character* getCharacterById(int characterId);
		static std::vector<Character*>* getAccountCharacters(int accountId);
	};
}


#endif
