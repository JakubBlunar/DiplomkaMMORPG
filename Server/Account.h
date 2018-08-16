#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <mysql.h>
#include "Database.h"

namespace s
{
	class Account
	{
	public:
		std::string email;
		int id;
		std::string login;
		std::string password;

		Account();

		bool checkPassword(std::string password) const;
		void save();

		std::string toJsonString();

		static Account* getById(int id);
		static Account* getByLogin(std::string login);
	};
}


#endif