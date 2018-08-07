#include "AuthManager.h"
#include "Server.h"
#include "ServerSettings.h"
#include "EventLoginResponse.h"


AuthManager::AuthManager()
{
	dynamic = false;
}


AuthManager::~AuthManager()
{
}

void AuthManager::handleEvent(EventLoginRequest* event, Session * playerSession, Server * s) const
{
	
	if(s->serverSettings->logsEnabled)
	{
		s->print(event->toString());
	}

	bool success = true;

	EventLoginResponse res(success);

	sf::Packet* p = res.toPacket();
	playerSession->socket->send(*p);
	delete p;
}
