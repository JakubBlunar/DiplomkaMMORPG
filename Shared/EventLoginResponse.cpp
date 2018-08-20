#include "stdafx.h"
#include "EventLoginResponse.h"


EventLoginResponse::EventLoginResponse()
{
	this->id = EventId::LOGINRESPONSE;
	this->status = false;
	this->account = "";
	this->message = "";
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
	if(*p >> status >> message >> account)
	{
		return true;
	}
	return false;
}

sf::Packet* EventLoginResponse::toPacket()
{
	sf::Packet* p = new sf::Packet();
	*p << id << status << message << account;
	return p;
}

std::string EventLoginResponse::toString() const
{
	return "Login Response Status(" + std::to_string(status) + ") Message:" + message + " Account: " + account;
}
