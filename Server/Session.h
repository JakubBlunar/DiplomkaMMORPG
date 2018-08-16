#ifndef SESSION_H
#define SESSION_H

#include "SFML\Network.hpp"
#include "Account.h"

class Session
{
	s::Account* account = nullptr;
public:
	Session();
	~Session();
	sf::TcpSocket* socket;

	bool isUserLoggedIn() const;

	s::Account* getAccount() const;
	void setAccount(s::Account* account);
};

#endif // !SESSION_H

