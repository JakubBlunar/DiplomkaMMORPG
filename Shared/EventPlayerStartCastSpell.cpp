#include "stdafx.h"
#include "EventPlayerStartCastSpell.h"


EventPlayerStartCastSpell::EventPlayerStartCastSpell() : spellId(0), startCastTimestamp(0), target(), entityId(0) {
	id = PLAYER_START_CAST_SPELL;
}


EventPlayerStartCastSpell::~EventPlayerStartCastSpell() {}

bool EventPlayerStartCastSpell::loadFromPacket(sf::Packet* p) {
	sf::Uint8 numTarget;
	if (*p >> spellId >> numTarget >> startCastTimestamp) {
		target = static_cast<SpellTarget>(numTarget);
		switch (target) {
			case SpellTarget::PLAYER:
			case SpellTarget::NPC: {
				if (*p >> entityId) {
					return true;
				}
				break;
			}
			case SpellTarget::POSITION: {
				float x, y;
				if (*p >> x >> y) {
					position = sf::Vector2f(x, y);
					return true;
				}
				break;
			}
			default: break;
		}

	}
	return false;
}

sf::Packet* EventPlayerStartCastSpell::toPacket() {
	sf::Packet* p = new sf::Packet();
	*p << id << spellId << static_cast<sf::Uint8>(target) << startCastTimestamp;
	switch (target) {
		case SpellTarget::PLAYER:
		case SpellTarget::NPC: {
			if (*p << entityId) {
				return p;
			}
			break;
		}
		case SpellTarget::POSITION: {
			if (*p << position.x << position.y) {
				return p;
			}
		}
		default: break;
	}
	throw "Cannot convert this event to packet";
}
