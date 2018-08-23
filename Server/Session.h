#ifndef SESSION_H
#define SESSION_H

#include "SFML\Network.hpp"

namespace s {
	class Account;
}


namespace s {
	class Session {
		Account* account = nullptr;
	public:
		Session();
		~Session();
		sf::TcpSocket* socket;

		bool isUserLoggedIn() const;

		Account* getAccount() const;
		void setAccount(s::Account* account);
	};

}

#endif // !SESSION_H
