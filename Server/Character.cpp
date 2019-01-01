#include "Character.h"
#include <mysql.h>
#include "Database.h"
#include "../Server/json.hpp"
#include "Map.h"
#include "SpellHolder.h"
#include <spdlog/spdlog.h>

using json = nlohmann::json;

s::Character::Character(): account(nullptr), id(0), type(), faction(), isBot(false) {
	movement = sf::Vector2f(0, 0);
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

	string queryAttr = "UPDATE character_attributes SET";
	queryAttr.append(" experience=" + std::to_string(getAttribute(EntityAttributeType::EXPERIENCE, false)) + ",");
	queryAttr.append(" money=" + std::to_string(getAttribute(EntityAttributeType::MONEY, false)) + ",");
	queryAttr.append(" stamina=" + std::to_string(getAttribute(EntityAttributeType::STAMINA, false)) + ",");
	queryAttr.append(" agility=" + std::to_string(getAttribute(EntityAttributeType::AGILITY, false)) + ",");
	queryAttr.append(" intelect=" + std::to_string(getAttribute(EntityAttributeType::INTELECT, false)) + ",");
	queryAttr.append(" spirit=" + std::to_string(getAttribute(EntityAttributeType::SPIRIT, false)) + ",");
	queryAttr.append(" strength=" + std::to_string(getAttribute(EntityAttributeType::STRENGTH, false)) + ",");
	queryAttr.append(" armor=" + std::to_string(getAttribute(EntityAttributeType::ARMOR, false)));
	queryAttr.append("WHERE characterId=" + std::to_string(id));

	bool success = Database::i()->executeModify(query) > 0;
	if (success) {
		success = Database::i()->executeModify(queryAttr) > 0;
	}


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

	jsonData["spells"] = json::array();
	for (SpellInfo* const spell : spells) {
		jsonData["spells"].push_back(spell->id);
	}

	return jsonData;
}

s::Character* s::Character::getCharacterById(int characterId) {
	std::string query = "SELECT id, name, faction, type, mapId, positionX, positionY FROM characters WHERE id='" +
		Database
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

	std::string attributeQuery =
		"SELECT experience, money, stamina, agility, intelect, spirit, armor FROM character_attributes WHERE characterId="
		+
		std::to_string(character->id) + ";";
	MYSQL_RES* resAttr = Database::i()->executeQuery(attributeQuery);
	MYSQL_ROW attributesRow = mysql_fetch_row(resAttr);


	character->setAttribute(EntityAttributeType::EXPERIENCE, std::stof(attributesRow[0]));
	character->setAttribute(EntityAttributeType::MONEY, std::stof(attributesRow[1]));
	character->setAttribute(EntityAttributeType::STAMINA, std::stof(attributesRow[2]));
	character->setAttribute(EntityAttributeType::AGILITY, std::stof(attributesRow[3]));
	character->setAttribute(EntityAttributeType::INTELECT, std::stof(attributesRow[4]));
	character->setAttribute(EntityAttributeType::SPIRIT, std::stof(attributesRow[5]));
	character->setAttribute(EntityAttributeType::ARMOR, std::stof(attributesRow[6]));

	character->recalcLevel();
	character->recalcMaxHealth();
	character->recalcMaxMana();
	character->setAttribute(EntityAttributeType::HP, character->getAttribute(EntityAttributeType::BASE_HP, false));
	character->setAttribute(EntityAttributeType::MP, character->getAttribute(EntityAttributeType::BASE_MP, false));


	std::string spellsQuery = "SELECT spellType FROM character_spells WHERE characterId=" +
		std::to_string(character->id) + " ORDER BY spellType ASC;";
	MYSQL_RES* resSpells = Database::i()->executeQuery(spellsQuery);


	SpellHolder* sh = SpellHolder::instance();
	MYSQL_ROW spellsRow;
	while ((spellsRow = mysql_fetch_row(resSpells))) {
		if (spellsRow[0]) {
			int spellType = std::stoi(spellsRow[0]);
			try {
				SpellInfo* si = sh->getSpellInfo(spellType);
				character->spells.push_back(si);
			} catch (std::string& e) {
				spdlog::get("log")->error(e);
			} catch (...) {
				spdlog::get("log")->error("Error while loading character spells");
			}
		}
		
	}

	mysql_free_result(res);
	mysql_free_result(resAttr);
	mysql_free_result(resSpells);

	return character;
}

std::vector<s::Character*>* s::Character::getAccountCharacters(int accountId) {
	std::vector<Character*>* characters = new std::vector<Character*>();
	std::string query = "SELECT id FROM characters WHERE accountId='" + Database::escapeString(
			std::to_string(accountId)) +
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
