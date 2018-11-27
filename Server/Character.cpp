#include "Character.h"
#include <mysql.h>
#include "Database.h"
#include "../Server/json.hpp"
#include "Map.h"

using json = nlohmann::json;

s::Character::Character() {
	movement = sf::Vector2f(0,0);
}

void s::Character::setAccount(s::Account* account) {
	this->account = account;
}

s::Account* s::Character::getAccount() const {
	return account;
}

bool s::Character::save() const {
	std::string query = "UPDATE characters SET";
	query.append(" name='" + Database::escapeString(name) + "',");
	query.append(" faction=" + Database::escapeString(std::to_string(static_cast<int>(faction))) + ",");
	query.append(" type=" + Database::escapeString(std::to_string(static_cast<int>(type))) + ",");
	query.append(" mapId=" + Database::escapeString(std::to_string(mapId)) + ",");
	query.append(" positionX=" + Database::escapeString(std::to_string(position.x)) + ",");
	query.append(" positionY=" + Database::escapeString(std::to_string(position.y)));
	query.append(" WHERE id=" + std::to_string(id) + ";");

	bool success = Database::i()->executeModify(query) > 0;
	return success;
}

json s::Character::toJson() const {
	json jsonData({});

	jsonData["id"] = this->id;
	jsonData["name"] = this->name;
	jsonData["faction"] = static_cast<int>(faction);
	jsonData["type"] = static_cast<int>(type);
	jsonData["mapId"] = mapId;
	jsonData["positionX"] = position.x;
	jsonData["positionY"] = position.y;
	jsonData["movementX"] = movement.x;
	jsonData["movementY"] = movement.y;
	jsonData["speed"] = speed;
	jsonData["attributes"] = json(attributes);
	return jsonData;
}

s::Character* s::Character::getCharacterById(int characterId) {
	std::string query = "SELECT id, name, faction, type, mapId, positionX, positionY FROM characters WHERE id='" + Database
		::escapeString(std::to_string(characterId)) + "';";

	MYSQL_RES* res = Database::i()->executeQuery(query);

	bool found = mysql_num_rows(res) > 0;
	if (!found) {
		return nullptr;
	}

	Character* character = new Character();
	MYSQL_ROW row = mysql_fetch_row(res);

	character->id = std::stoi(row[0]);
	character->name = row[1];
	character->faction = static_cast<CharacterFaction>(std::stoi(row[2]));
	character->type = static_cast<CharacterType>(std::stoi(row[3]));
	character->mapId = std::stoi(row[4]);
	character->position = sf::Vector2f(std::stof(row[5]), std::stof(row[6]));
	character->movement = sf::Vector2f(0, 0);
	character->speed = 48;
	character->isBot = false;
	mysql_free_result(res);

	return character;
}

std::vector<s::Character*>* s::Character::getAccountCharacters(int accountId) {
	std::vector<Character*>* characters = new std::vector<Character*>();
	std::string query = "SELECT id FROM characters WHERE accountId='" + Database::escapeString(std::to_string(accountId)) +
		"';";

	MYSQL_RES* res = Database::i()->executeQuery(query);
	MYSQL_ROW row;

	while ((row = mysql_fetch_row(res))) {
		int id = std::stoi(row[0]);
		Character* character = getCharacterById(id);
		if (character) {
			characters->push_back(character);
		}
	}

	mysql_free_result(res);

	return characters;
}
