#ifndef EVENT_CHARACTER_LOGOUT
#define EVENT_CHARACTER_LOGOUT

#include "GameEvent.h"
class EventCharacterLogout :
	public GameEvent
{
public:
	EventCharacterLogout();
	~EventCharacterLogout();

	int characterId;

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;
};

#endif