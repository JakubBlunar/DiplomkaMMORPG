#include "stdafx.h"
#include "EventSpellCastResult.h"


EventSpellCastResult::EventSpellCastResult() : entityId(0), entityCategory(), spellId(0), result(), target(),
targetId(0) {
	id = SPELL_CAST_RESULT;
}


EventSpellCastResult::~EventSpellCastResult() {}

bool EventSpellCastResult::loadFromPacket(sf::Packet* p) {
	sf::Uint32 entityCategoryNum;
	sf::Uint8 castResultNum;

	if (*p >> entityId >> entityCategoryNum >> spellId >> castResultNum) {
		result = static_cast<SpellCastResultCode>(castResultNum);
		entityCategory = static_cast<EntityCategory>(entityCategoryNum);
		if (result == SpellCastResultCode::SUCCESS) {
			sf::Uint8 targetNum;
			if (*p >> targetNum >> startPosition.x >> startPosition.y) {
				target = static_cast<SpellTarget>(targetNum);
				if (target == SpellTarget::POSITION) {
					if (*p >> targetPosition.x >> targetPosition.y) {
						return true;
					}
					return false;
				}

				if (*p >> targetId) {
					return true;
				}
				return false;
			}
			return false;
		}
		return true;
	}
	return false;
}

sf::Packet* EventSpellCastResult::toPacket() {
	sf::Packet* p = new sf::Packet();

	if (*p << id << entityId << static_cast<sf::Uint32>(entityCategory) << spellId << static_cast<sf::Uint8>(result)) {
		if (result == SpellCastResultCode::SUCCESS) {
			if (*p << static_cast<sf::Uint8>(target) << startPosition.x << startPosition.y) {
				if (target == SpellTarget::POSITION) {
					if (*p << targetPosition.x << targetPosition.y) {
						return p;
					}
					throw "Cannot convert to packet";
				}
				if (*p << targetId) {
					return p;
				}
				throw "Cannot convert to packet";

			}
			throw "Cannot convert to packet";
		}
		return p;
	}
	throw "Cannot convert to packet";
}
