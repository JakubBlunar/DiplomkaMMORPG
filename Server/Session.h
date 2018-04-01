#ifndef SESSION_H
#define SESSION_H

#include "SFML\Network.hpp"

class Session
{
public:
	Session();
	~Session();
	sf::TcpSocket* socket;
};

#endif // !SESSION_H

