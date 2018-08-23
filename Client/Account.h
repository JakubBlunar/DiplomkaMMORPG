#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Player.h"
#include "../Client/json.hpp"

using json = nlohmann::json;

class Account {
public:
	Account();
	~Account();

	void setPlayerEntity(Player* playerEntity);
	Player* getPlayerEntity() const;

	void setId(int id);
	int getId() const;
	json getJsonData() const;
	void initFromJson(json data);
private:
	int id;
	std::string email;
	std::string login;

	json jsonData;

	Player* playerEntity;
};

#endif
