#include "SpellEventNpcExecute.h"
#include "Npc.h"
#include "EventSpellCastResult.h"
#include "ServerGlobals.h"
#include <spdlog/spdlog.h>
#include "SpellEventApplyEffects.h"


s::SpellEventNpcExecute::SpellEventNpcExecute() {}


s::SpellEventNpcExecute::~SpellEventNpcExecute() {}

void s::SpellEventNpcExecute::setNpc(Npc* npc) {
	this->npc = npc;
}

void s::SpellEventNpcExecute::execute(Server* s) {
	npc->spells.setCastingSpell(nullptr);

	if (!npc->isAlive()) return;

	Map* map = npc->position.getMap();
	float actualMana = npc->attributes.getAttribute(EntityAttributeType::MP, true);

	sf::Time serverTime = s->getServerTime();
	bool error = false;
	if (npc->spells.hasCooldown(spellInfo->id, serverTime)) {
		error = true;
	}

	if (!error && actualMana - spellInfo->manaCost < 0) {
		error = true;
	}

	if (!error && targetCharacter) {
		EntityToEntityRayCast* res = map->makeRayCast(npc, targetCharacter);
		if (!res->canSee) {
			error = true;
		}
		else {
			if (res->closestEntityDistance * METTOPIX > spellInfo->maxRange) {
				error = true;
			}
		}
	}

	if (!error && targetNpc && targetNpc != npc) {
		EntityToEntityRayCast* res = map->makeRayCast(npc, targetNpc);
		if (!res->canSee) {
			error = true;
		}
		else {
			if (res->closestEntityDistance * METTOPIX > spellInfo->maxRange) {
				error = true;
			}
		}
	}

	if (error) {
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

	spdlog::get("log")->info("Casting spell NPC {} {}: {}", npc->getName(), npc->getSpawnId(), spellInfo->name);
	if (targetEntity) {
		if (spellInfo->spellCategory == EntityType::MOVABLE_SPELL) {
			MovableSpell* spell = SpellHolder::instance()->createMovableSpell(spellInfo->id);
			spell->position.setPosition(npc->position.getPosition());
			spell->setOwner(npc);
			spell->setTarget(targetEntity);
			map->addSpell(spell);
		}
		else {
			Spell* spell = SpellHolder::instance()->createSpell(spellInfo->id);
			spell->setOwner(npc);
			spell->setTarget(targetEntity);

			SpellEventApplyEffects* e = new SpellEventApplyEffects(spell, targetEntity);
			e->executeTime = s->getServerTime();
			s->spellManager.queueEvent(e);
		}
	}

	npc->spells.setCooldown(spellInfo->id, serverTime + spellInfo->cooldownTime);
	npc->attributes.setAttribute(EntityAttributeType::MP, actualMana - spellInfo->manaCost);

	EventSpellCastResult* e = new EventSpellCastResult();
	e->entityId = npc->getSpawnId();
	e->entityCategory = NPC;
	e->spellId = spellInfo->id;
	e->result = SpellCastResultCode::SUCCESS;
	e->startPosition = npc->position.getPosition();
	e->target = spellTarget;

	switch (spellTarget) {
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
