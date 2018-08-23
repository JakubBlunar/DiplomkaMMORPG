#include "AuthManager.h"
#include "Server.h"
#include "EventLoginResponse.h"
#include <spdlog/spdlog.h>
#include "EventCharacterChoose.h"
#include "Account.h"
#include "EventCharacterChooseResponse.h"
#include "Character.h"
#include "Map.h"

s::AuthManager::AuthManager() {
	dynamic = false;
}


s::AuthManager::~AuthManager() {
}

void s::AuthManager::handleEvent(EventLoginRequest* event, s::Session* playerSession, s::Server* s) const {
	spdlog::get("log")->info("EVENT: {}", event->toString());

	bool result = false;
	std::string accData;
	std::string message;

	auto find = std::find_if(s->sessions.begin(), s->sessions.end(), [event](s::Session* sess)-> bool {
		s::Account* account = sess->getAccount();
		if (!account)
			return false;
		return account->login == event->name;
	});

	if (find == s->sessions.end()) {
		s::Account* account = s::Account::getByLogin(event->name);
		if (account) {
			if (account->checkPassword(event->password)) {
				result = true;
				accData = account->toJsonString();
				playerSession->setAccount(account);
				account->setSession(playerSession);
			}
			else {
				message = "Password does not match";
			}
		}
		else {
			message = "Account not found";
		}
	}
	else {
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


void s::AuthManager::handleEvent(EventCharacterChoose* event, s::Session* playerSession, s::Server* s) const {
	spdlog::get("log")->info("EVENT: {}", event->toString());

	bool result = false;
	std::string characterData;
	std::string message;

	s::Account* account = playerSession->getAccount();
	if (account) {
		if (!account->getCharacter()) {
			auto character = std::find_if(account->characters->begin(), account->characters->end(),
			                              [event](s::Character* character)-> bool {
				                              if (character->id == event->characterId) {
					                              return true;
				                              }
				                              return false;
			                              });

			if (character != account->characters->end()) {

				Character* ch = *character;
				Map* m = s->mapsManager.getMap(ch->mapId);
				if (m) {
					ch->setAccount(playerSession->getAccount());
					playerSession->getAccount()->setCharacter(ch);

					m->addCharacter(ch);
					ch->setMap(m);

					json resData;
					resData["character"] = ch->toJson();
					resData["otherPlayers"] = m->getCharactersJson();


					characterData = resData.dump();
					result = true;
				}
				else {
					message = "Coul't load map";
				}
			}
			else {
				message = "Character don't exists";
			}
		}
		else {
			message = "You already playing some character.";
		}
	}
	else {
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
