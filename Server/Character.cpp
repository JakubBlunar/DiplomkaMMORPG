#include "Character.h"
#include <mysql.h>
#include "Database.h"
#include "../Server/json.hpp"
#include "Map.h"
#include "SpellHolder.h"
#include <spdlog/spdlog.h>
#include "ServerGlobals.h"

using json = nlohmann::json;

s::Character::Character(): account(nullptr), attributes(), spells(), id(0), type(), faction(), isBot(false) {
	entityType = EntityType::PLAYER;
}

s::Character::~Character() {}

void s::Character::setAccount(s::Account* account) {
	this->account = account;
}

s::Account* s::Character::getAccount() const {
	return account;
}

bool s::Character::save() const {
	sf::Vector2f characterPosition = position.getPosition();
	std::string query = "UPDATE characters SET";
	query.append(" name='" + Database::escapeString(name) + "',");
	query.append(" faction=" + Database::escapeString(std::to_string(static_cast<int>(faction))) + ",");
	query.append(" type=" + Database::escapeString(std::to_string(static_cast<int>(type))) + ",");
	query.append(" mapId=" + Database::escapeString(std::to_string(position.getMapId())) + ",");
	query.append(" positionX=" + Database::escapeString(std::to_string(characterPosition.x)) + ",");
	query.append(" positionY=" + Database::escapeString(std::to_string(characterPosition.y)));
	query.append(" WHERE id=" + std::to_string(id) + ";");

	std::string queryAttr = "UPDATE character_attributes SET";
	queryAttr.append(" experience=" + std::to_string(attributes.getAttribute(EntityAttributeType::EXPERIENCE, false)) + ",");
	queryAttr.append(" money=" + std::to_string(attributes.getAttribute(EntityAttributeType::MONEY, false)) + ",");
	queryAttr.append(" stamina=" + std::to_string(attributes.getAttribute(EntityAttributeType::STAMINA, false)) + ",");
	queryAttr.append(" agility=" + std::to_string(attributes.getAttribute(EntityAttributeType::AGILITY, false)) + ",");
	queryAttr.append(" intelect=" + std::to_string(attributes.getAttribute(EntityAttributeType::INTELECT, false)) + ",");
	queryAttr.append(" spirit=" + std::to_string(attributes.getAttribute(EntityAttributeType::SPIRIT, false)) + ",");
	queryAttr.append(" strength=" + std::to_string(attributes.getAttribute(EntityAttributeType::STRENGTH, false)) + ",");
	queryAttr.append(" armor=" + std::to_string(attributes.getAttribute(EntityAttributeType::ARMOR, false)));
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
	jsonData["mapId"] = position.getMapId();
	jsonData["positionX"] = position.getPosition().x;
	jsonData["positionY"] = position.getPosition().y;
	jsonData["movementX"] = position.getMovement().x;
	jsonData["movementY"] = position.getMovement().y;
	jsonData["attributes"] = json::array();

	int attributesCount = attributes.getCount();
	for(int i = 0; i < attributesCount; i++) {
		jsonData["attributes"].push_back(attributes.getAttributeByIndex(i));
	}

	jsonData["spells"] = json::array();
	for (SpellInfo* const spell : availableSpells) {
		jsonData["spells"].push_back(spell->id);
	}

	return jsonData;
}

void s::Character::update(sf::Time deltaTime, Server* s, Map* map) {
	b2Body* characterBody = this->position.getBody();
	b2Vec2 position = characterBody->GetPosition();
	b2Vec2 velocity = characterBody->GetLinearVelocity();

	this->position.setPosition(sf::Vector2f(position.x * METTOPIX, position.y * METTOPIX));
	this->position.setMovement(sf::Vector2f(velocity.x * METTOPIX, velocity.y * METTOPIX));
	
	if (this->position.getMovement() != sf::Vector2f(0, 0)) {
		SpellEvent* castingSpell = this->spells.getEventWithCastingSpell();
		if (castingSpell && castingSpell->spellInfo->castingTime != sf::Time::Zero) {
			s->spellManager.interruptSpellCast(castingSpell, true);
		}
	}
}

b2Body* s::Character::getBody() const {
	return position.getBody();
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
	character->position.setMapId(std::stoi(row[4]));
	character->position.setPosition(sf::Vector2f(std::stof(row[5]), std::stof(row[6])));
	character->position.setMovement(sf::Vector2f(0, 0));
	
	character->isBot = false;

	std::string attributeQuery =
		"SELECT experience, money, stamina, agility, intelect, spirit, armor FROM character_attributes WHERE characterId="
		+
		std::to_string(character->id) + ";";
	MYSQL_RES* resAttr = Database::i()->executeQuery(attributeQuery);
	MYSQL_ROW attributesRow = mysql_fetch_row(resAttr);


	character->attributes.setAttribute(EntityAttributeType::EXPERIENCE, std::stof(attributesRow[0]));
	character->attributes.setAttribute(EntityAttributeType::MONEY, std::stof(attributesRow[1]));
	character->attributes.setAttribute(EntityAttributeType::STAMINA, std::stof(attributesRow[2]));
	character->attributes.setAttribute(EntityAttributeType::AGILITY, std::stof(attributesRow[3]));
	character->attributes.setAttribute(EntityAttributeType::INTELECT, std::stof(attributesRow[4]));
	character->attributes.setAttribute(EntityAttributeType::SPIRIT, std::stof(attributesRow[5]));
	character->attributes.setAttribute(EntityAttributeType::ARMOR, std::stof(attributesRow[6]));
	character->attributes.setAttribute(EntityAttributeType::SPEED, 48.f);

	character->attributes.recalcLevel();
	character->attributes.recalcMaxHealth();
	character->attributes.recalcMaxMana();
	character->attributes.setAttribute(EntityAttributeType::HP, character->attributes.getAttribute(EntityAttributeType::BASE_HP, false));
	character->attributes.setAttribute(EntityAttributeType::MP, character->attributes.getAttribute(EntityAttributeType::BASE_MP, false));


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
				character->availableSpells.push_back(si);
			}
			catch (std::string& e) {
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
