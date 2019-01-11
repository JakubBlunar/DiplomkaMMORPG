#include "BotManager.h"
#include "Account.h"
#include <spdlog/spdlog.h>
#include "Character.h"

s::BotManager::BotManager():
	idCharacterManager(214728364, 214748364),
	idAccountManager(214728364, 214748364)
{
	dynamic = false;
}
s::BotManager::~BotManager() {
	
}

s::Account* s::BotManager::createBotAccount() {
	Account* account = new Account();
	
	account->id = idAccountManager.getId();
	account->email = "email" + std::to_string(account->id) + "@bot.com";
	account->login = "bot" + std::to_string(account->id);
	account->password = "bot";
	account->characters = new std::vector<Character*>();
	account->isBot = true;
	account->characters->push_back(createBotCharacter());

	spdlog::get("log")->info("New Bot account {}", std::to_string(account->id));
	return account;
}

void s::BotManager::destroyBotAccount(Account* account) {
	idAccountManager.freeId(account->id);
	delete account;
}

s::Character* s::BotManager::createBotCharacter() {
	Character* c = new Character();

	c->id = idCharacterManager.getId();
	c->name = "Bot" + std::to_string(c->id);
	c->isBot = true;
	c->position.setPosition(sf::Vector2f(100, 100));
	c->position.setMapId(1);
	c->position.setMovement(sf::Vector2f(0,0));
	c->position.setSpeed(32.f);

	if (rand() % 100 < 49) {
		c->type = CharacterType::MALE_KNIGHT;
	} else {
		c->type = CharacterType::FEMALE_KNIGHT;
	}

	if (rand() % 100 < 49) {
		c->faction = CharacterFaction::LIGHT;
	} else {
		c->faction = CharacterFaction::DARK;
	}

	return c;
}

void s::BotManager::destroyBotCharacter(Character* character) {
	idCharacterManager.freeId(character->id);

	delete character;
}
