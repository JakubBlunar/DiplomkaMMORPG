#pragma once
#include "GameEvent.h"

class EventCharacterChoose :
	public GameEvent {
public:
	EventCharacterChoose();
	~EventCharacterChoose();

	int characterId;

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;
};
