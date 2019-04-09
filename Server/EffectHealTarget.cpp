#include "EffectHealTarget.h"
#include <utility>
#include "Random.h"
#include "Character.h"
#include "Utils.h"
#include "EventAttributesChanged.h"
#include "Account.h"
#include "Map.h"
#include "Npc.h"
#include "EffectDealDamage.h"
#include "ChatConstants.h"
#include "EventSendMessage.h"

s::EffectHealTarget::EffectHealTarget(SpellInfo spellInfo, float modifier) : Effect(std::move(spellInfo)) {
	this->modifier = modifier;
	name = "Healing";
}

s::EffectHealTarget::~EffectHealTarget() {}

void s::EffectHealTarget::castHealing(Character* caster, Character* target) const {
	Random* random = Random::instance();

	sf::Vector2f healRange;
	if (spellInfo.type == SpellType::MELLE) {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::STRENGTH);
	}
	else {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::INTELECT);
	}

	float healing = random->randomUniformFloat(healRange.x, healRange.y);
	healing *= modifier;
	healing = ceil(healing);
	float maxHp = target->attributes.getAttribute(EntityAttributeType::BASE_HP, true);
	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp + healing;
	if (newHp > maxHp) {
		newHp = maxHp;
		healing = maxHp - actualHp;
	}
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = PLAYER;
	e->spawnId = target->id;
	e->setChange(EntityAttributeType::HP, newHp);

	EventSendMessage logEvent;
	logEvent.message = caster->name + " heals " + target->name + " with " + spellInfo.name + " for " + ChatUtils::
		floatToString(healing, 0) + " hp";
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();

	logEvent.entityType = EntityType::PLAYER;
	logEvent.targetId = target->id;
	logEvent.combatPopup = "+" + ChatUtils::floatToString(healing, 0) + "hp";

	sf::Packet* p = logEvent.toPacket();
	caster->getAccount()->getSession()->sendPacket(p);
	if (caster != target) {
		target->getAccount()->getSession()->sendPacket(p);
	}
	delete p;

	target->position.getMap()->sendEventToAllPlayers(e);
}

void s::EffectHealTarget::castHealing(Character* caster, Npc* target) const {
	if (!target->isAlive()) return;

	Random* random = Random::instance();

	sf::Vector2f healRange;
	if (spellInfo.type == SpellType::MELLE) {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::STRENGTH);
	}
	else {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::INTELECT);
	}

	float healing = random->randomUniformFloat(healRange.x, healRange.y);
	healing *= modifier;
	healing = ceil(healing);
	float maxHp = target->attributes.getAttribute(EntityAttributeType::BASE_HP, true);
	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp + healing;
	if (newHp > maxHp) {
		newHp = maxHp;
		healing = maxHp - actualHp;
	}
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = NPC;
	e->spawnId = target->getSpawnId();
	e->setChange(EntityAttributeType::HP, newHp);

	EventSendMessage logEvent;
	logEvent.message = caster->name + " heals " + target->getName() + " with " + spellInfo.name + " for " +
		ChatUtils::floatToString(healing, 0) + " hp";
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();
	logEvent.entityType = EntityType::NPC;
	logEvent.targetId = target->getSpawnId();
	logEvent.combatPopup = "+" + ChatUtils::floatToString(healing, 0) + "hp";

	sf::Packet* p = logEvent.toPacket();
	caster->getAccount()->getSession()->sendPacket(p);
	delete p;
	target->position.getMap()->sendEventToAllPlayers(e);
}

void s::EffectHealTarget::castHealing(Npc* caster, Character* target) const {
	Random* random = Random::instance();

	sf::Vector2f healRange;
	if (spellInfo.type == SpellType::MELLE) {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::STRENGTH);
	}
	else {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::INTELECT);
	}

	float healing = random->randomUniformFloat(healRange.x, healRange.y);
	healing *= modifier;
	healing = ceil(healing);
	float maxHp = target->attributes.getAttribute(EntityAttributeType::BASE_HP, true);
	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp + healing;
	if (newHp > maxHp) {
		newHp = maxHp;
		healing = maxHp - actualHp;
	}
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = PLAYER;
	e->spawnId = target->id;
	e->setChange(EntityAttributeType::HP, newHp);

	EventSendMessage logEvent;
	logEvent.message = caster->getName() + " heals " + target->name + " with " + spellInfo.name + " for " +
		ChatUtils::floatToString(healing, 0) + " hp";
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();

	logEvent.entityType = EntityType::PLAYER;
	logEvent.targetId = target->id;
	logEvent.combatPopup = "+" + ChatUtils::floatToString(healing, 0) + "hp";

	sf::Packet* p = logEvent.toPacket();
	target->getAccount()->getSession()->sendPacket(p);
	delete p;

	target->position.getMap()->sendEventToAllPlayers(e);
}

void s::EffectHealTarget::castHealing(Npc* caster, Npc* target) const {
	Random* random = Random::instance();

	if (!target->isAlive()) return;

	sf::Vector2f healRange;
	if (spellInfo.type == SpellType::MELLE) {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::STRENGTH);
	}
	else {
		healRange = caster->attributes.getMinMaxHealing(EntityAttributeType::INTELECT);
	}

	float healing = random->randomUniformFloat(healRange.x, healRange.y);
	healing *= modifier;
	healing = ceil(healing);
	float maxHp = target->attributes.getAttribute(EntityAttributeType::BASE_HP, true);
	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp + healing;
	if (newHp > maxHp) {
		newHp = maxHp;
	}
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = NPC;
	e->spawnId = target->getSpawnId();
	e->setChange(EntityAttributeType::HP, newHp);
	target->position.getMap()->sendEventToAllPlayers(e);
}


void s::EffectHealTarget::apply(Entity* caster, Entity* target) {
	if (caster->getEntityType() == EntityType::PLAYER) {
		if (target->getEntityType() == EntityType::PLAYER) {
			castHealing((Character*)caster, (Character*)target);
		}
		else {
			castHealing((Character*)caster, (Npc*)target);
		}
	}
	else if (caster->getEntityType() == EntityType::NPC) {
		if (target->getEntityType() == EntityType::PLAYER) {
			castHealing((Npc*)caster, (Character*)target);
		}
		else {
			castHealing((Npc*)caster, (Npc*)target);
		}
	}
}

s::Effect* s::EffectHealTarget::clone() {
	EffectHealTarget* clone = new EffectHealTarget(spellInfo, modifier);
	clone->setServer(server);
	return clone;
}

void s::EffectHealTarget::loadFromJson(json jsonData) {
	modifier = (float)jsonData["modifier"].get<json::number_float_t>();
}
