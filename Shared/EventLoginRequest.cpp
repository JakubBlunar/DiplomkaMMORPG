#include "stdafx.h"
#include "EventLoginRequest.h"


EventLoginRequest::EventLoginRequest() {
	id = LOGINREQUEST;
}

EventLoginRequest::EventLoginRequest(std::string name, std::string password) {
	id = EventId::LOGINREQUEST;
	this->name = name;
	this->password = password;
}


EventLoginRequest::~EventLoginRequest() {

}

bool EventLoginRequest::loadFromPacket(sf::Packet* p) {
	if (*p >> name >> password) {
		return true;
	}
	return false;
}

sf::Packet* EventLoginRequest::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << name << password;
	return p;
}

std::string EventLoginRequest::toString() const {
	return "Login request " + name + " " + password;
}
