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
#include "Account.h"

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

	sf::Time serverTime = s->getServerTime();
	EventSpellCastResult* error = nullptr;

	if (character->spells.hasCooldown(spellInfo->id, serverTime)) {
		error = new EventSpellCastResult();
		error->entityId = character->id;
		error->entityCategory = PLAYER;
		error->spellId = spellInfo->id;
		error->result = SpellCastResultCode::HAS_COOLDOWN;
	}

	if (!error && actualMana - spellInfo->manaCost < 0) {
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
	bool isAlive = true;
	switch (spellTarget) {
		case SpellTarget::PLAYER: {
			targetEntity = targetCharacter;
			break;
		}
		case SpellTarget::NPC: {
			targetEntity = targetNpc;
			isAlive = targetNpc->isAlive();
			break;
		default:
			return;
		}
	}

	EventSpellCastResult* e = new EventSpellCastResult();
	e->entityId = character->id;
	e->entityCategory = PLAYER;
	e->spellId = spellInfo->id;
	e->result = SpellCastResultCode::SUCCESS;
	e->startPosition = character->position.getPosition();
	e->target = spellTarget;

	if (!isAlive) {
		e->result = SpellCastResultCode::TARGET_IS_DEAD;
		sf::Packet* p = e->toPacket();
		character->getAccount()->getSession()->sendPacket(p);
		delete p;
		delete e;
		return;
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

	character->spells.setCooldown(spellInfo->id, serverTime + spellInfo->cooldownTime);
	character->attributes.setAttribute(EntityAttributeType::MP, actualMana - spellInfo->manaCost);
	

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
			targetNpc->lock();
			if (!targetNpc->combat.hasCombat()) {
				targetNpc->startCombat(character);
			} else {
				targetNpc->combat.setAttackingCharacter(character);
			}
			targetNpc->unlock();

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
