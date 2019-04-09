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
#include "EntityCombat.h"
#include <Box2D/Dynamics/b2Body.h>

using json = nlohmann::json;

namespace s {
	class Account;
	class Map;
}

namespace s {
	class Character : public Entity {
		Account* account;
	public:
		Character();
		virtual ~Character();
		void setAccount(Account* account);
		Account* getAccount() const;

		EntityAttributes attributes;
		EntityPosition position;
		EntitySpells spells;
		EntityCombat combat;

		int id;
		std::string name;
		CharacterType type;
		CharacterFaction faction;

		bool isBot;

		bool save() const;
		json toJson();

		void update(sf::Time deltaTime, Server* s, Map* map);

		b2Body* getBody() const override;

		static Character* getCharacterById(int characterId);
		static std::vector<Character*>* getAccountCharacters(int accountId);
		sf::Vector2f getPosition() const override;
	};
}


#endif
