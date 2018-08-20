#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <mysql.h>
#include "Database.h"
#include <vector>
#include "Character.h"

namespace s
{
	class Account
	{
	private:
		Character* character;

	public:
		Account();

		std::string email;
		int id;
		std::string login;
		std::string password;

		std::vector<Character*>* characters;

		Character* getCharacter() const;
		void setCharacter(Character* character);
		
		bool checkPassword(std::string password) const;
		bool save() const;

		std::string toJsonString();

		static Account* getById(int id);
		static Account* getByLogin(std::string login);
	};
}


#endif