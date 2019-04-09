#ifndef EVENT_LOGIN_RESPONSE_H
#define EVENT_LOGIN_RESPONSE_H

#include "GameEvent.h"

class EventLoginResponse :
	public GameEvent {
public:
	EventLoginResponse();
	~EventLoginResponse();

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;

	bool status;
	std::string account;
	std::string message;

};

#endif
