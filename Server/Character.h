#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>
#include "../Shared/CharacterConstants.h"
#include <vector>
#include "../Server/json.hpp"
#include <SFML/System/Vector2.hpp>
#include <Box2D/Dynamics/b2Body.h>

using json = nlohmann::json;

namespace s
{
	class Account;
}

namespace s
{
	class Character
	{
		Account* account;
	public:
		Character();
		~Character();

		void setAccount(Account* account);
		Account* getAccount() const;

		int id;
		std::string name;
		CharacterType type;
		CharacterFaction faction;

		sf::Vector2f movement;

		sf::Vector2f position;
		int mapId;

		b2Body* body;

		bool save() const;
		json toJson() const;

		static Character* getCharacterById(int characterId);
		static std::vector<Character*>* getAccountCharacters(int accountId);
	};
}


#endif 