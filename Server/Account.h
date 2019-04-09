#ifndef S_ACCOUNT_H
#define S_ACCOUNT_H
#include <string>
#include <vector>

namespace s {
	class Character;
	class Session;
}

namespace s {
	class Account {
	private:
		Character* character;
		Session* session;
	public:
		Account();
		~Account();

		std::string email;
		int id;
		std::string login;
		std::string password;
		bool isBot;

		std::vector<Character*>* characters;

		Character* getCharacter() const;
		void setCharacter(Character* character);

		bool checkPassword(std::string password) const;
		bool save() const;

		std::string toJsonString();

		void setSession(Session* session);
		Session* getSession() const;

		static Account* getById(int id);
		static Account* getByLogin(std::string login);
	};
}


#endif
