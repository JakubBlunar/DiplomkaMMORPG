#ifndef BOT_MANAGER_H
#define BOT_MANAGER_H

#include "IDManager.h"
#include <SFML/Graphics.hpp>
#include "Manager.h"

namespace s {
	class Map;
	class Server;
	class Account;
	class Character;

	class BotManager : public Manager
	{
		IDManager<sf::Int32> idCharacterManager;
		IDManager<sf::Int32> idAccountManager;
	public:
		BotManager();
		~BotManager();

		Account* createBotAccount();
		void destroyBotAccount(Account* account);

		Character* createBotCharacter();
		void destroyBotCharacter(Character* character);

	};
}

#endif
