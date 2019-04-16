#ifndef EVENT_CHARACTER_POSITION_CHANGED
#define EVENT_CHARACTER_POSITION_CHANGED

#include "GameEvent.h"
class EventCharacterPositionChanged :
	public GameEvent
{
public:
	EventCharacterPositionChanged();
	~EventCharacterPositionChanged();

	float positionX;
	float positionY;
	int characterId;

	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif
