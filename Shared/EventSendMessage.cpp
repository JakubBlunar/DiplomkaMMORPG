#include "stdafx.h"
#include "EventSendMessage.h"


EventSendMessage::EventSendMessage()
{
	id = SEND_MESSAGE;
	playerId = -1;
}


EventSendMessage::~EventSendMessage()
{
}

bool EventSendMessage::loadFromPacket(sf::Packet* p) {
	sf::Uint8 typeNum;

	if (*p >> message >> time >> typeNum) {
		messageType = static_cast<MessageType>(typeNum);

		if (messageType == MessageType::SAY) {
			if (*p >> playerId) {
				return true;
			}
			return false;
		} else if (messageType == MessageType::COMBAT_LOG) {
			sf::Uint8 numEntityType;
			if (*p >> playerId >> targetId >> numEntityType >> combatPopup) {
				entityType = static_cast<EntityType>(numEntityType);
				return true;
			}
			return false;
		}
		return true;
	}
	return false;
}

sf::Packet* EventSendMessage::toPacket() {
	sf::Packet* p = new sf::Packet();
	std::string e = "Cannot convert to packet";
	if (*p << id << message << time << static_cast<sf::Uint8>(messageType)) {
		if (messageType == MessageType::SAY) {
			if (*p << playerId) {
				return p;
			}
			throw e;
		} else if(messageType == MessageType::COMBAT_LOG) {
			if (*p << playerId << targetId << static_cast<sf::Uint8>(entityType) << combatPopup) {
				return p;
			}
			throw e;
		}
		return p;
	}
	throw e;
}
