#include "AuthManager.h"
#include "Server.h"
#include "ServerSettings.h"
#include "EventLoginResponse.h"
#include <spdlog/spdlog.h>
#include "Database.h"
#include <iostream>


AuthManager::AuthManager()
{
	dynamic = false;
}


AuthManager::~AuthManager()
{
}

void AuthManager::handleEvent(EventLoginRequest* event, Session * playerSession, Server * s) const
{
	spdlog::get("log")->info("EVENT: {}", event->toString());

	s::Account* account = s::Account::getByLogin(event->name);
	bool result = false;
	std::string accData;

	if(account)
	{
		if(account->checkPassword(event->password))
		{
			result = true;
			accData = account->toJsonString();
			playerSession->setAccount(account);
		}
	}

	EventLoginResponse res(result, accData);
	sf::Packet* p = res.toPacket();
	playerSession->socket->send(*p);
	delete p;
}
