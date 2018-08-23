#pragma once
#include <string>
#include "GameEvent.h"

class EventLoginRequest :
	public GameEvent {
public:
	EventLoginRequest();
	EventLoginRequest(std::string name, std::string password);
	~EventLoginRequest();
	std::string name;
	std::string password;

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;
};
