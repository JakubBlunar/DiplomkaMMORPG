#include "SpellEventCharacterExecute.h"
#include <spdlog/spdlog.h>
#include "Character.h"
#include "EventSpellCastResult.h"
#include "Npc.h"
#include "Map.h"
#include "EntityToEntityRayCast.h"
#include "ServerGlobals.h"
#include "MovableSpell.h"
#include "SpellEventApplyEffects.h"

s::SpellEventCharacterExecute::SpellEventCharacterExecute(): character(nullptr), spellTarget(),
                                                             targetCharacter(nullptr),
                                                             targetNpc(nullptr) {}


s::SpellEventCharacterExecute::~SpellEventCharacterExecute() {}

void s::SpellEventCharacterExecute::setCharacter(Character* character) {
	this->character = character;
	character->spells.setCastingSpell(this);
}

void s::SpellEventCharacterExecute::execute(Server* s) {
	character->spells.setCastingSpell(nullptr);
	Map* map = character->position.getMap();
	float actualMana = character->attributes.getAttribute(EntityAttributeType::MP, true);

	EventSpellCastResult* error = nullptr;
	if (actualMana - spellInfo->manaCost < 0) {
		error = new EventSpellCastResult();
		error->entityId = character->id;
		error->entityCategory = PLAYER;
		error->spellId = spellInfo->id;
		error->result = SpellCastResultCode::NOT_ENOUGH_MANA;
	}

	if (!error && targetCharacter && targetCharacter != character) {
		EntityToEntityRayCast* res = map->makeRayCast(character, targetCharacter);
		if (!res->canSee) {
			error = new EventSpellCastResult();
			error->entityId = character->id;
			error->entityCategory = PLAYER;
			error->spellId = spellInfo->id;
			error->result = SpellCastResultCode::CANT_SEE_TARGET;
		}
	}

	if (!error && targetNpc) {
		EntityToEntityRayCast* res = map->makeRayCast(character, targetNpc);
		if (!res->canSee) {
			error = new EventSpellCastResult();
			error->entityId = character->id;
			error->entityCategory = PLAYER;
			error->spellId = spellInfo->id;
			error->result = SpellCastResultCode::CANT_SEE_TARGET;
		}
		else {
			if (res->closestEntityDistance * METTOPIX > spellInfo->maxRange) {
				error = new EventSpellCastResult();
				error->entityId = character->id;
				error->entityCategory = PLAYER;
				error->spellId = spellInfo->id;
				error->result = SpellCastResultCode::OUT_OF_RANGE;
			}
		}
	}

	if (error) {
		map->sendEventToAllPlayers(error);
		return;
	}

	Entity* targetEntity;
	switch (spellTarget) {
		case SpellTarget::PLAYER: {
			targetEntity = targetCharacter;
			break;
		}
		case SpellTarget::NPC: {
			targetEntity = targetNpc;
			break;
		default:
			return;
		}
	}


	spdlog::get("log")->info("Casting spell {}: {}", character->name, spellInfo->name);
	if (targetEntity) {
		if (spellInfo->spellCategory == EntityType::MOVABLE_SPELL) {
			MovableSpell* spell = SpellHolder::instance()->createMovableSpell(spellInfo->id);
			spell->position.setPosition(character->position.getPosition());
			spell->setOwner(character);
			spell->setTarget(targetEntity);
			map->addSpell(spell);
		}
		else {
			Spell* spell = SpellHolder::instance()->createSpell(spellInfo->id);
			spell->setOwner(character);
			spell->setTarget(targetEntity);

			SpellEventApplyEffects * e = new SpellEventApplyEffects(spell, targetEntity);
			e->executeTime = s->getServerTime();
			s->spellManager.queueEvent(e);
		}
	}

	character->attributes.setAttribute(EntityAttributeType::MP, actualMana - spellInfo->manaCost);

	EventSpellCastResult* e = new EventSpellCastResult();
	e->entityId = character->id;
	e->entityCategory = PLAYER;
	e->spellId = spellInfo->id;
	e->result = SpellCastResultCode::SUCCESS;
	e->startPosition = character->position.getPosition();
	e->target = spellTarget;

	switch (spellTarget) {
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
	character->spells.setCastingSpell(nullptr);

	if (!sendInfo) {
		return;
	}

	EventSpellCastResult* e = new EventSpellCastResult();
	e->entityId = character->id;
	e->entityCategory = PLAYER;
	e->spellId = spellInfo->id;
	e->result = SpellCastResultCode::INTERRUPTED;
	character->position.getMap()->sendEventToAllPlayers(e);
}