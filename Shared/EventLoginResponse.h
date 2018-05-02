#ifndef EVENT_LOGIN_RESPONSE_H
#define EVENT_LOGIN_RESPONSE_H

#include "GameEvent.h"
class EventLoginResponse :
	public GameEvent
{
public:
	EventLoginResponse(bool status);
	~EventLoginResponse();

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;

	bool status;



};

#endif