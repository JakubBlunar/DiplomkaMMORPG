#ifndef EVENT_PLAYER_START_CAST_SPELL
#define EVENT_PLAYER_START_CAST_SPELL

#include "GameEvent.h"
#include "SpellConstants.h"

class EventPlayerStartCastSpell :
	public GameEvent
{
public:
	EventPlayerStartCastSpell();
	~EventPlayerStartCastSpell();

	int spellId;
	time_t startCastTimestamp;
	SpellTarget target;
	sf::Vector2f position;
	int entityId;

	void accept(EventVisitor*) override;
	bool loadFromPacket(sf::Packet* p) override;
	sf::Packet* toPacket() override;
};

#endif

