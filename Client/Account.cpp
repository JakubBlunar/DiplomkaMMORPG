#include "Account.h"

Account::Account():
	playerEntity(nullptr)
{

}

Account::~Account()
{
}

void Account::setPlayerEntity(Player* playerEntity)
{
	this->playerEntity = playerEntity;
}

Player* Account::getPlayerEntity() const
{
	return playerEntity;
}

void Account::setId(int id)
{
	this->id = id;
}

int Account::getId() const
{
	return id;
}

json Account::getJsonData() const
{
	return jsonData;
}

void Account::initFromJson(json data)
{
	id = (int) data["id"].get<json::number_integer_t>();
	login = data["login"].get<json::string_t>();
	email = data["email"].get<json::string_t>();
	jsonData = data;
}

