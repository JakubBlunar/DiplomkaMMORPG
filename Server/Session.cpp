#include "Session.h"
#include "Account.h"


s::Session::Session(): socket(nullptr) {
}


s::Session::~Session() {
}

void s::Session::setSocket(sf::TcpSocket* socket) {
	sf::Lock lock(sessionLock);
	this->socket = socket;
}
void s::Session::sendPacket(sf::Packet* p) {
	sf::Lock lock(sessionLock);
	if (socket) {
		socket->send(*p);
	}
}

bool s::Session::isUserLoggedIn() const {
	return account != nullptr;
}

s::Account* s::Session::getAccount() const {
	return account;
}

void s::Session::setAccount(s::Account* account) {
	delete this->account;
	this->account = account;
}
