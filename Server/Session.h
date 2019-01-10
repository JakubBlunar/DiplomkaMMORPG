#ifndef SESSION_H
#define SESSION_H

#include "SFML\Network.hpp"

namespace s {
	class Account;
}


namespace s {
	class Session {
		Account* account = nullptr;
		sf::Mutex sessionLock;
		sf::TcpSocket* socket;		
	public:
		Session();
		~Session();
		sf::TcpSocket* getSocket() const {
			return socket;
		}
		void setSocket(sf::TcpSocket* socket);

		bool isUserLoggedIn() const;

		Account* getAccount() const;
		void setAccount(s::Account* account);

		void sendPacket(sf::Packet* p);
	};

}

#endif // !SESSION_H
