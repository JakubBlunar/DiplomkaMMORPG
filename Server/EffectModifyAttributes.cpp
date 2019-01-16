#include "EffectModifyAttributes.h"
#include "JsonLoader.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include "EventAttributesChanged.h"
#include "Character.h"
#include "Npc.h"


s::EffectModifyAttributes::EffectModifyAttributes()
{
}


s::EffectModifyAttributes::~EffectModifyAttributes()
{
}

void s::EffectModifyAttributes::apply(Entity* caster, Entity* target) {
	this->caster = caster;
	this->target = target;

	spdlog::get("log")->info("applying effects of {}", name);
	if (!casterModify.empty()) {
		switch(caster->getEntityType()) {
			case EntityType::PLAYER:
				modifyCharacterAttributes((Character*) caster, &casterModify);
			break;
			case EntityType::NPC:
				modifyNpcAttributes((Npc*) caster, &casterModify);
			break;
			default: break;
		}
	}

	if (!targetModify.empty()) {
		switch(target->getEntityType()) {
			case EntityType::PLAYER:
				modifyCharacterAttributes((Character*) target, &targetModify);
			break;
			case EntityType::NPC:
				modifyNpcAttributes((Npc*) target, &targetModify);
			break;
			default: break;
		}
	}
}

s::Effect* s::EffectModifyAttributes::clone() {
	EffectModifyAttributes* clone = new EffectModifyAttributes();

	clone->setId(id);
	clone->setName(name);
	clone->setServer(server);

	for (auto && cm : casterModify) {
		clone->addCasterModify(cm.first, cm.second);
	}
	for (auto && tm : targetModify) {
		clone->addTargetModify(tm.first, tm.second);
	}
	return clone;
}

void s::EffectModifyAttributes::addCasterModify(EntityAttributeType attribute, float value) {
	this->casterModify.insert(std::make_pair(attribute, value));
}

void s::EffectModifyAttributes::addTargetModify(EntityAttributeType attribute, float value) {
	this->targetModify.insert(std::make_pair(attribute, value));
}

void s::EffectModifyAttributes::loadFromJson(json jsonData) {
	id = (int)jsonData["id"].get<json::number_integer_t>();
	name = jsonData["name"].get<json::string_t>();

	if (jsonData.find("target") != jsonData.end()) {
		json targetAttributes = jsonData["target"].get<json::object_t>();
		for (auto& el : targetAttributes.items()) {
			EntityAttributeType attribute = static_cast<EntityAttributeType>(std::stoi(el.key()));
			float value = (float) el.value().get<json::number_float_t>();
			addTargetModify(attribute, value);
		}
	}

	if (jsonData.find("caster") != jsonData.end()) {
		json casterAttributes = jsonData["caster"].get<json::object_t>();
		for (auto& el : casterAttributes.items()) {
			EntityAttributeType attribute = static_cast<EntityAttributeType>(std::stoi(el.key()));
			float value = (float) el.value().get<json::number_float_t>();
			addCasterModify(attribute, value);
		}
	}
}

void s::EffectModifyAttributes::loadFromFile(std::string filename) {
	json data = JsonLoader::instance()->loadJson(filename);
	loadFromJson(data);
}

void s::EffectModifyAttributes::modifyCharacterAttributes(Character* character,
	std::map<EntityAttributeType, float>* modifiers) {
	
	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = PLAYER;
	e->spawnId = character->id;

	for (auto && modifier : *modifiers) {
		float actual = character->attributes.getAttribute(modifier.first, true);
		float newValue = actual + modifier.second;

		character->attributes.setAttribute(modifier.first, newValue);
		e->setChange(modifier.first, newValue);
	}

	character->position.getMap()->sendEventToAllPlayers(e);
}

void s::EffectModifyAttributes::modifyNpcAttributes(Npc* npc, std::map<EntityAttributeType, float>* modifiers) {
	if (!npc->isAlive())
		return;
	
	EventAttributesChanged* e = new EventAttributesChanged();
	e->entityType = NPC;
	e->spawnId = npc->getSpawnId();

	for (auto && modifier : *modifiers) {
		float actual = npc->attributes.getAttribute(modifier.first, true);
		float newValue = actual + modifier.second;

		npc->attributes.setAttribute(modifier.first, newValue);
		e->setChange(modifier.first, newValue);
	}

	npc->position.getMap()->sendEventToAllPlayers(e);

	if (npc->attributes.getAttribute(EntityAttributeType::HP, true) <= 0) {
		server->npcManager.npcDied(npc, caster);
	}
}
