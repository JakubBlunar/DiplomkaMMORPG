#ifndef EVENT_CHARACTER_MAP_LEAVE
#define EVENT_CHARACTER_MAP_LEAVE

#include "GameEvent.h"
class EventCharacterMapLeave :
	public GameEvent
{
public:
	EventCharacterMapLeave();
	~EventCharacterMapLeave();

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;

	int characterId;
	int mapId;

};

#endif
