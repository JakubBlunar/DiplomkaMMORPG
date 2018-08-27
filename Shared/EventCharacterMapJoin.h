#ifndef EVENT_CHARACTER_MAP_JOIN
#define EVENT_CHARACTER_MAP_JOIN

#include "GameEvent.h"
class EventCharacterMapJoin :
	public GameEvent
{
public:
	EventCharacterMapJoin();
	~EventCharacterMapJoin();

	std::string characterData;
	int mapId;

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
	std::string toString() const override;
};

#endif
