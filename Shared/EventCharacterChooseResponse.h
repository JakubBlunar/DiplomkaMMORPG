#ifndef EVENT_CHARACTER_CHOOSE_RESPONSE
#define EVENT_CHARACTER_CHOOSE_RESPONSE

#include "GameEvent.h"
class EventCharacterChooseResponse :
	public GameEvent
{
public:
	EventCharacterChooseResponse();
	~EventCharacterChooseResponse();

	bool success;
	std::string message;
	std::string characterData;

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;
};

#endif