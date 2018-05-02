#include "stdafx.h"
#include "EventLoginResponse.h"


EventLoginResponse::EventLoginResponse(bool status)
{
	this->id = EventId::LOGINRESPONSE;
	this->status = status;
}


EventLoginResponse::~EventLoginResponse()
{
}

void EventLoginResponse::accept(EventVisitor *v)
{
	v->visit(this);
}

bool EventLoginResponse::loadFromPacket(sf::Packet* p)
{
	if(*p >> status)
	{
		return true;
	}
	return false;
}

sf::Packet* EventLoginResponse::toPacket()
{
	sf::Packet* p = new sf::Packet();
	*p << id << status;
	return p;
}

std::string EventLoginResponse::toString() const
{
	return "Login Response Status(" + std::to_string(status) + ")";
}
