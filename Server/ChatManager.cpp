#include "ChatManager.h"

#include "spdlog/fmt/fmt.h"

#include "Server.h"
#include "Account.h"
#include "Character.h"
#include "Map.h"
#include "../Shared/Utils.h"


s::ChatManager::ChatManager()
{
}


s::ChatManager::~ChatManager()
{
}

void s::ChatManager::handleEvent(EventCharacterChoose* event, s::Session* playerSession, s::Server* s) const {
	std::string welcomeMessage = s->serverSettings->welcomeMessage;
	Character* character = playerSession->getAccount()->getCharacter();

	EventSendMessage e;
	e.message = fmt::format(welcomeMessage, character->name);
	e.messageType = MessageType::SERVER_ANNOUNCEMENT;
	e.playerId = -1;
	e.time = Utils::getActualUtcTime();

	sf::Packet* p = e.toPacket();
	playerSession->sendPacket(p);
	delete p;
}

void s::ChatManager::handleEvent(EventSendMessage* event, s::Session* playerSession, s::Server* s) {
	Character* character = playerSession->getAccount()->getCharacter();

	EventSendMessage* e = new EventSendMessage();
	e->messageType = MessageType::SAY;
	e->message = event->message;
	e->playerId = character->id;
	e->time = Utils::getActualUtcTime();

	character->position.getMap()->sendEventToAllPlayers(e);
}
