#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include "EventId.h"
#include "SFML/Network.hpp"

class GameEvent {
protected:
	EventId id;
public:
	virtual ~GameEvent() = default;
	GameEvent();

	EventId getId();
	virtual bool loadFromPacket(sf::Packet* p) = 0;
	virtual sf::Packet* toPacket() = 0;

	virtual std::string toString() const {
		return std::to_string(id);
	}
};

#endif
