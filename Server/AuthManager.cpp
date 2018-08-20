#include "AuthManager.h"
#include "Server.h"
#include "EventLoginResponse.h"
#include <spdlog/spdlog.h>
#include "EventCharacterChoose.h"
#include "Account.h"
#include "EventCharacterChooseResponse.h"

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

	bool result = false;
	std::string accData;
	std::string message;

	auto find = std::find_if(s->sessions.begin(), s->sessions.end(), [event](Session* sess)->bool
	{
		s::Account* account = sess->getAccount();
		if(!account)
			return false;
		return account->login == event->name;
	});

	if(find == s->sessions.end())
	{
		s::Account* account = s::Account::getByLogin(event->name);
		if(account)
		{
			if(account->checkPassword(event->password))
			{
				result = true;
				accData = account->toJsonString();
				playerSession->setAccount(account);
			}else
			{
				message = "Password does not match";
			}
		}else
		{
			message = "Account not found";
		}
	}else
	{
		message = "Account is already logged in";
	}

	EventLoginResponse res;
	res.status = result;
	res.message = message;
	res.account = accData;

	sf::Packet* p = res.toPacket();
	playerSession->socket->send(*p);
	delete p;
}


void AuthManager::handleEvent(EventCharacterChoose* event, Session * playerSession, Server * s) const
{
	spdlog::get("log")->info("EVENT: {}", event->toString());

	bool result = false;
	std::string characterData;
	std::string message;

	s::Account* account = playerSession->getAccount();
	if(account)
	{
		if(!account->getCharacter())
		{
			auto character = std::find_if(account->characters->begin(), account->characters->end(), [event](Character* character)->bool
			{
				if(character->id == event->characterId)
				{
					return true;
				}
				return false;
			});

			if(character != account->characters->end())
			{
				characterData = (*character)->toJson().dump();
				result = true;
			}else
			{
				message = "Character don't exists";
			}
		}else
		{
			message = "You already playing some character.";
		}
	}else
	{
		message = "You are not logged in";
	}

	EventCharacterChooseResponse res;
	res.success = result;
	res.message = message;
	res.characterData = characterData;

	sf::Packet* p = res.toPacket();
	playerSession->socket->send(*p);
	delete p;



}