#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Player.h"

class Account
{
public:
	Account();
	~Account();

	void setPlayerEntity(Player* playerEntity);
	Player* getPlayerEntity() const;

private:
	Player* playerEntity;
};

#endif