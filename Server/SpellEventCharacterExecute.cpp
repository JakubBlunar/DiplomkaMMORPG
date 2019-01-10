#include "SpellEventCharacterExecute.h"
#include <spdlog/spdlog.h>
#include "Character.h"
#include "EventSpellCastResult.h"
#include "Npc.h"
#include "Map.h"

s::SpellEventCharacterExecute::SpellEventCharacterExecute(): character(nullptr), spellTarget(),
                                                             targetCharacter(nullptr),
                                                             targetNpc(nullptr) {}


s::SpellEventCharacterExecute::~SpellEventCharacterExecute() {}

void s::SpellEventCharacterExecute::setCharacter(Character* character) {
	this->character = character;
	character->setCastingSpell(this);
}

void s::SpellEventCharacterExecute::execute(Server* s) {
	character->setCastingSpell(nullptr);
	Map* map = character->getMap();
	float actualMana = character->getAttribute(EntityAttributeType::MP, true);

	EventSpellCastResult* error = nullptr;
	if (actualMana - spellInfo->manaCost < 0) {
		error = new EventSpellCastResult();
		error->entityId = character->id;
		error->entityCategory = PLAYER;
		error->spellId = spellInfo->id;
		error->result = SpellCastResultCode::NOT_ENOUGH_MANA;
		spdlog::get("log")->info("Not enough mana spell {}: {}", character->name, spellInfo->name);
	}

	if (error) {
		map->sendEventToAllPlayers(error);
		return;
	}



	spdlog::get("log")->info("Casting spell {}: {}", character->name, spellInfo->name);


	character->setAttribute(EntityAttributeType::MP, actualMana - spellInfo->manaCost);

	EventSpellCastResult* e = new EventSpellCastResult();
	e->entityId = character->id;
	e->entityCategory = PLAYER;
	e->spellId = spellInfo->id;
	e->result = SpellCastResultCode::SUCCESS;
	e->startPosition = character->getPosition();
	e->target = spellTarget;

	switch(spellTarget) {
		case SpellTarget::POSITION: {
			e->targetPosition = targetPosition;
			break;
		}
		case SpellTarget::PLAYER: {
			e->targetId = targetCharacter->id;
			break;
		}
		case SpellTarget::NPC: {
			e->targetId = targetNpc->getSpawnId();
			break;
		default:
			return;
		}
	}
	
	map->sendEventToAllPlayers(e);
}

void s::SpellEventCharacterExecute::interrupt(bool sendInfo) {
	character->setCastingSpell(nullptr);

	if (!sendInfo) {
		return;
	}
	
	EventSpellCastResult* e = new EventSpellCastResult();
	e->entityId = character->id;
	e->entityCategory = PLAYER;
	e->spellId = spellInfo->id;
	e->result = SpellCastResultCode::INTERRUPTED;
	character->getMap()->sendEventToAllPlayers(e);
}
