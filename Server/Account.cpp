#include "Account.h"
#include <mysql.h>
#include "Database.h"
#include "Crypto.h"
#include "../Server/json.hpp"
#include "Character.h"

using json = nlohmann::json;

s::Account::Account(): id(-1) {
}

s::Character* s::Account::getCharacter() const {
	return character;
}

void s::Account::setCharacter(Character* character) {
	this->character = character;
}

bool s::Account::checkPassword(std::string password) const {
	std::string hash = Crypto::i()->hashWithSha256(password);
	if (hash == this->password) {
		return true;
	}
	return false;
}

bool s::Account::save() const {
	char* escaped = nullptr;

	std::string query = "UPDATE accounts SET ";
	mysql_escape_string(escaped, login.c_str(), login.length());
	query.append("login='" + std::string(escaped) + "'");
	mysql_escape_string(escaped, email.c_str(), email.length());
	query.append("email='" + std::string(escaped) + "'");
	query.append(" WHERE id=" + std::to_string(id) + ";");

	bool success = Database::i()->executeModify(query) > 0;

	return success;
}

std::string s::Account::toJsonString() {
	json data({});

	data["id"] = this->id;
	data["email"] = this->email;
	data["login"] = this->login;

	json charactersJson = json::array();
	std::for_each(characters->begin(), characters->end(), [&charactersJson](Character* ch) {
		charactersJson.push_back(ch->toJson());
	});
	data["characters"] = charactersJson;

	return data.dump();
}

void s::Account::setSession(Session* session) {
	this->session = session;
}

s::Session* s::Account::getSession() const {
	return session;
}

s::Account* s::Account::getById(int id) {
	std::string query = "SELECT id, login, email, password FROM accounts WHERE id=" + std::to_string(id) + ";";

	MYSQL_RES* res = Database::i()->executeQuery(query);

	bool found = mysql_num_rows(res) > 0;
	if (!found) {
		return nullptr;
	}

	Account* account = new Account();
	MYSQL_ROW row = mysql_fetch_row(res);

	account->id = std::stoi(row[0]);
	account->login = row[1];
	account->email = row[2];
	account->password = row[3];

	mysql_free_result(res);

	return account;
}

s::Account* s::Account::getByLogin(std::string login) {
	std::string query = "SELECT id, login, email, password FROM accounts WHERE login='" + Database::escapeString(login) +
		"';";

	MYSQL_RES* res = Database::i()->executeQuery(query);

	bool found = mysql_num_rows(res) > 0;
	if (!found) {
		return nullptr;
	}

	Account* account = new Account();
	MYSQL_ROW row = mysql_fetch_row(res);

	account->id = std::stoi(row[0]);
	account->login = row[1];
	account->email = row[2];
	account->password = row[3];

	mysql_free_result(res);

	account->characters = Character::getAccountCharacters(account->id);
	for_each(account->characters->begin(), account->characters->end(), [account](Character* character) {
		character->setAccount(account);
	});

	return account;
}
