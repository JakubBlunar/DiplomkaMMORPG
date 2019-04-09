#ifndef EVENT_SEND_MESSAGE_H
#define EVENT_SEND_MESSAGE_H

#include "GameEvent.h"
#include "ChatConstants.h"

class EventSendMessage :
	public GameEvent
{
public:
	EventSendMessage();
	virtual ~EventSendMessage();

	std::string message;
	int playerId = -1;
	MessageType messageType;
	time_t time;

	int targetId = -1;
	std::string combatPopup;
	EntityType entityType;

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif
