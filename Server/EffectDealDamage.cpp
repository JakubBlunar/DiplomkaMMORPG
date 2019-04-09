#include "EffectDealDamage.h"
#include <utility>
#include "Random.h"
#include "Character.h"
#include "Utils.h"
#include "EventAttributesChanged.h"
#include "Account.h"
#include "Map.h"
#include "Npc.h"
#include "EventSendMessage.h"

s::EffectDealDamage::EffectDealDamage(SpellInfo spellInfo, float modifier) : Effect(std::move(spellInfo)) {
	this->modifier = modifier;
	name = "Damage";
}

s::EffectDealDamage::~EffectDealDamage() {}

void s::EffectDealDamage::dealDamage(Character* caster, Character* target) const {
	Random* random = Random::instance();
	bool miss = random->randomUniformFloat(0, 1) < target->attributes.getAttribute(
		EntityAttributeType::DODGE_CHANCE, true);
	if (miss) {
		EventSendMessage logEvent;
		logEvent.message = caster->name + " missed with " + spellInfo.name;
		logEvent.messageType = MessageType::COMBAT_LOG;
		logEvent.playerId = -1;
		logEvent.entityType = EntityType::PLAYER;
		logEvent.targetId = target->id;
		logEvent.combatPopup = "Miss";

		logEvent.time = Utils::getActualUtcTime();

		sf::Packet* p = logEvent.toPacket();
		caster->getAccount()->getSession()->sendPacket(p);
		target->getAccount()->getSession()->sendPacket(p);
		return;
	}

	sf::Vector2f dmgRange;
	if (spellInfo.type == SpellType::MELLE) {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::STRENGTH);
	}
	else {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::INTELECT);
	}

	float dmg = random->randomUniformFloat(dmgRange.x, dmgRange.y);
	dmg *= modifier;
	dmg *= (1.f - target->attributes.getAttribute(EntityAttributeType::RESISTANCE, true));
	dmg = ceil(dmg);

	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp - dmg;
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = PLAYER;
	e->spawnId = target->id;
	e->setChange(EntityAttributeType::HP, newHp);

	EventSendMessage logEvent;
	logEvent.message = caster->name + " hit " + target->name + " with " + spellInfo.name + " for " + ChatUtils::
		floatToString(dmg, 0) + "dmg";
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();

	logEvent.entityType = EntityType::PLAYER;
	logEvent.targetId = target->id;
	logEvent.combatPopup = "-" + ChatUtils::floatToString(dmg, 0) + "hp";

	sf::Packet* p = logEvent.toPacket();
	caster->getAccount()->getSession()->sendPacket(p);
	if (caster != target) {
		target->getAccount()->getSession()->sendPacket(p);
	}
	delete p;

	target->position.getMap()->sendEventToAllPlayers(e);
}

void s::EffectDealDamage::dealDamage(Character* caster, Npc* target) const {
	Random* random = Random::instance();
	bool miss = random->randomUniformFloat(0, 1) < caster->attributes.getAttribute(
		EntityAttributeType::DODGE_CHANCE, true);
	if (miss) {
		EventSendMessage logEvent;
		logEvent.message = caster->name + " missed with " + spellInfo.name;
		logEvent.messageType = MessageType::COMBAT_LOG;
		logEvent.playerId = -1;
		logEvent.time = Utils::getActualUtcTime();
		logEvent.entityType = EntityType::NPC;
		logEvent.targetId = target->getSpawnId();
		logEvent.combatPopup = "Miss";

		sf::Packet* p = logEvent.toPacket();
		caster->getAccount()->getSession()->sendPacket(p);
		return;
	}

	sf::Vector2f dmgRange;
	if (spellInfo.type == SpellType::MELLE) {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::STRENGTH);
	}
	else {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::INTELECT);
	}

	float dmg = random->randomUniformFloat(dmgRange.x, dmgRange.y);
	dmg *= modifier;
	dmg *= (1.f - target->attributes.getAttribute(EntityAttributeType::RESISTANCE, true));
	dmg = ceil(dmg);

	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp - dmg;
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = NPC;
	e->spawnId = target->getSpawnId();
	e->setChange(EntityAttributeType::HP, newHp);

	EventSendMessage logEvent;
	logEvent.message = caster->name + " hit " + target->getName() + " with " + spellInfo.name + " for " +
		ChatUtils::floatToString(dmg, 0) + "dmg";
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();

	logEvent.entityType = EntityType::NPC;
	logEvent.targetId = target->getSpawnId();
	logEvent.combatPopup = "-" + ChatUtils::floatToString(dmg, 0) + "hp";

	sf::Packet* p = logEvent.toPacket();
	caster->getAccount()->getSession()->sendPacket(p);
	delete p;
	target->position.getMap()->sendEventToAllPlayers(e);

	if (newHp <= 0) {
		server->npcManager.npcDied(target, caster);
	}
}

void s::EffectDealDamage::dealDamage(Npc* caster, Character* target) const {
	Random* random = Random::instance();

	bool miss = random->randomUniformFloat(0, 1) < target->attributes.getAttribute(
		EntityAttributeType::DODGE_CHANCE, true);
	if (miss) {
		EventSendMessage logEvent;
		logEvent.message = caster->getName() + " missed with " + spellInfo.name;
		logEvent.messageType = MessageType::COMBAT_LOG;
		logEvent.playerId = -1;
		logEvent.time = Utils::getActualUtcTime();

		logEvent.entityType = EntityType::PLAYER;
		logEvent.targetId = target->id;
		logEvent.combatPopup = "Miss";

		sf::Packet* p = logEvent.toPacket();
		target->getAccount()->getSession()->sendPacket(p);
		return;
	}

	sf::Vector2f dmgRange;
	if (spellInfo.type == SpellType::MELLE) {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::STRENGTH);
	}
	else {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::INTELECT);
	}

	float dmg = random->randomUniformFloat(dmgRange.x, dmgRange.y);
	dmg *= modifier;
	dmg *= (1.f - target->attributes.getAttribute(EntityAttributeType::RESISTANCE, true));
	dmg = ceil(dmg);

	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp - dmg;
	if (newHp < 1) {
		newHp = 1;
	}
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = PLAYER;
	e->spawnId = target->id;
	e->setChange(EntityAttributeType::HP, newHp);

	EventSendMessage logEvent;
	logEvent.message = caster->getName() + " hit " + target->name + " with " + spellInfo.name + " for " +
		ChatUtils::floatToString(dmg, 0) + "dmg";
	logEvent.messageType = MessageType::COMBAT_LOG;
	logEvent.playerId = -1;
	logEvent.time = Utils::getActualUtcTime();

	logEvent.entityType = EntityType::PLAYER;
	logEvent.targetId = target->id;
	logEvent.combatPopup = "-" + ChatUtils::floatToString(dmg, 0) + "hp";

	sf::Packet* p = logEvent.toPacket();
	target->getAccount()->getSession()->sendPacket(p);
	delete p;

	target->position.getMap()->sendEventToAllPlayers(e);
}

void s::EffectDealDamage::dealDamage(Npc* caster, Npc* target) const {
	Random* random = Random::instance();
	bool miss = random->randomUniformFloat(0, 1) < target->attributes.getAttribute(
		EntityAttributeType::DODGE_CHANCE, true);
	if (miss) {
		EventSendMessage logEvent;
		logEvent.message = caster->getName() + " missed with " + spellInfo.name;
		logEvent.messageType = MessageType::COMBAT_LOG;
		logEvent.playerId = -1;
		logEvent.time = Utils::getActualUtcTime();
		return;
	}

	sf::Vector2f dmgRange;
	if (spellInfo.type == SpellType::MELLE) {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::STRENGTH);
	}
	else {
		dmgRange = caster->attributes.getMinMaxDmg(EntityAttributeType::INTELECT);
	}

	float dmg = random->randomUniformFloat(dmgRange.x, dmgRange.y);
	dmg *= modifier;
	dmg *= (1.f - target->attributes.getAttribute(EntityAttributeType::RESISTANCE, true));
	dmg = ceil(dmg);

	float actualHp = target->attributes.getAttribute(EntityAttributeType::HP, true);
	float newHp = actualHp - dmg;
	target->attributes.setAttribute(EntityAttributeType::HP, newHp);

	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = NPC;
	e->spawnId = target->getSpawnId();
	e->setChange(EntityAttributeType::HP, newHp);

	target->position.getMap()->sendEventToAllPlayers(e);

	if (newHp <= 0) {
		server->npcManager.npcDied(target, caster);
	}
}




void s::EffectDealDamage::apply(Entity* caster, Entity* target) {
	if (caster->getEntityType() == EntityType::PLAYER) {
		if (target->getEntityType() == EntityType::PLAYER) {
			dealDamage((Character*)caster, (Character*)target);
		}
		else {
			dealDamage((Character*)caster, (Npc*)target);
		}
	}
	else if (caster->getEntityType() == EntityType::NPC) {
		if (target->getEntityType() == EntityType::PLAYER) {
			dealDamage((Npc*)caster, (Character*)target);
		}
		else {
			dealDamage((Npc*)caster, (Npc*)target);
		}
	}
}

s::Effect* s::EffectDealDamage::clone() {
	EffectDealDamage* clone = new EffectDealDamage(spellInfo, modifier);
	clone->setServer(server);
	return clone;
}

void s::EffectDealDamage::loadFromJson(json jsonData) {
	modifier = (float)jsonData["modifier"].get<json::number_float_t>();
}
