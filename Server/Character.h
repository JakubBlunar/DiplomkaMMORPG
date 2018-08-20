#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>
#include "../Shared/CharacterConstants.h"
#include <vector>
#include "../Server/json.hpp"

using json = nlohmann::json;

class Character
{
public:
	Character();
	~Character();

	int id;
	std::string name;
	CharacterType type;
	CharacterFaction faction;

	bool save() const;
	json toJson() const;

	static Character* getCharacterById(int characterId);
	static std::vector<Character*>* getAccountCharacters(int accountId);
};

#endif 