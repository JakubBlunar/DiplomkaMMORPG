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
