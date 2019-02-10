#include "NpcLuaConnector.h"
#include "Npc.h"
#include "NpcCommandStay.h"
#include "NpcCommandMoveTo.h"
#include "Location.h"
#include <spdlog/spdlog.h>
#include "../Client/GamePlayScene.h"


s::NpcLuaConnector::NpcLuaConnector(Npc* npc) {
	this->npc = npc;
	server = npc->getServer();
}


s::NpcLuaConnector::~NpcLuaConnector() {}

void s::NpcLuaConnector::connect() {
	npc->lock();
	npc->luaState["npc"] = npc;
	npc->luaState.new_usertype<Npc>("Npc",
	                                "getSpawnId", &Npc::getSpawnId,
	                                "getAttribute", &Npc::getAttribute
	);

	npc->luaState.set_function("sendNpcToRandomPositionInLocation", &NpcLuaConnector::sendNpcToRandomPositionInLocation,
	                           this);
	npc->luaState.set_function("sendNpcToPosition", &NpcLuaConnector::sendNpcToPosition, this);
	npc->luaState.set_function("makeNpcStay", &NpcLuaConnector::makeNpcStay, this);
	npc->luaState.set_function("castRandomSpell", &NpcLuaConnector::castRandomSpell, this);
	npc->luaState.set_function("castSpell", &NpcLuaConnector::castSpell, this);
	npc->luaState.set_function("canCastSpell", &NpcLuaConnector::canCastSpell, this);
	npc->luaState.set_function("doNothing", &NpcLuaConnector::doNothing, this);
	npc->luaState.set_function("getHp", &NpcLuaConnector::getHp, this);
	npc->luaState.set_function("getMaxHp", &NpcLuaConnector::getMaxHp, this);
	npc->luaState.set_function("getMp", &NpcLuaConnector::getMp, this);
	npc->luaState.set_function("isInSpawnPosition", &NpcLuaConnector::isInSpawnPosition, this);
	npc->luaState.set_function("goToSpawnPosition", &NpcLuaConnector::goToSpawnPosition, this);

	sol::protected_function_result scriptResult = npc->luaState.script(npc->npc_script);
	if (!scriptResult.valid()) {
		sol::error err = scriptResult;
		std::string what = err.what();
		spdlog::get("log")->error("Lua script failed, npc {}, Exception: {}", npc->getType(), what);
	}

	sol::function f = npc->luaState["handleEvent"];
	if (f == sol::nil) {
		spdlog::get("log")->error("Function not found");
	}
	npc->unlock();
}

void s::NpcLuaConnector::sendNpcToRandomPositionInLocation(float maxCommandDuration) const {
	npc->lock();
	Location* l = npc->position.getLocation();
	NpcCommand* previousCommand = npc->getNpcCommand();
	if (l) {
		NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(l->generateRandomPoint(), npc,
		                                                             npc->position.getMap(), npc->getServer(),
		                                                             sf::seconds(maxCommandDuration));
		commandMoveToRandom->init();
		npc->setNpcCommand(commandMoveToRandom);
	}
	else {
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(),
		                                      sf::seconds(maxCommandDuration)));
	}
	delete previousCommand;
	npc->unlock();
}

void s::NpcLuaConnector::sendNpcToPosition(float x, float y, float maxCommandDuration) const {
	npc->lock();
	Location* l = npc->position.getLocation();
	NpcCommand* previousCommand = npc->getNpcCommand();
	if (l) {
		NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(sf::Vector2f(x, y), npc, npc->position.getMap(),
		                                                             npc->getServer(), sf::seconds(maxCommandDuration));
		commandMoveToRandom->init();
		npc->setNpcCommand(commandMoveToRandom);
	}
	else {
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(),
		                                      sf::seconds(maxCommandDuration)));
	}
	delete previousCommand;
	npc->unlock();
}

void s::NpcLuaConnector::makeNpcStay(float duration) const {
	npc->lock();
	NpcCommand* previousCommand = npc->getNpcCommand();
	NpcCommandStay* commandStay = new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(),
	                                                 sf::seconds(duration));
	npc->setNpcCommand(commandStay);
	delete previousCommand;
	npc->unlock();
}

void s::NpcLuaConnector::castRandomSpell() const {
	
	std::map<int, SpellInfo*>* availableSpells = npc->spells.getAvailableSpells();
	SpellInfo* found = nullptr;
	for (auto it = availableSpells->begin(); it != availableSpells->end(); ++it) {
		if (canCastSpell(it->second)) {
			found = it->second;
			break;
		}
	}

	if (found) {
		npc->getServer()->spellManager.handleNpcCast(npc, found);
	}
	else {
		doNothing(2000);
	}
}

void s::NpcLuaConnector::castSpell(int spellType, bool onSelf) const {
	SpellInfo* spell = npc->spells.getSpell(spellType);

	switch (spell->targetRestriction) {
		case SpellTargetRestriction::SELF: {
			if (!onSelf) {
				doNothing(2000);
				return;
			}
			break;
		}
		case SpellTargetRestriction::ENEMY: {
			if (npc->combat.target->getEntityType() != EntityType::PLAYER) {
				doNothing(2000);
				return;
			}
		}
		case SpellTargetRestriction::FRIENDLY: {
			if (!onSelf) {
				doNothing(2000);
				return;
			}
		}
	}

	
	if (spell) {
		npc->getServer()->spellManager.handleNpcCast(npc, spell);
		return;
	}
	doNothing(2000);
}

void s::NpcLuaConnector::doNothing(int ms) const {
	sf::Time serverTime = npc->getServer()->getServerTime();
	npc->setDelayedDecision(serverTime + sf::milliseconds(ms));
}

bool s::NpcLuaConnector::isInSpawnPosition() const {
	sf::Vector2f spawnPosition = npc->getSpawnPosition();

	if (spawnPosition.x < 5) {
		return true;
	}

	sf::Vector2f npcPosition = npc->position.getPosition();
	float distance = b2Distance(b2Vec2(npcPosition.x, npcPosition.y), b2Vec2(spawnPosition.x, spawnPosition.y));
	if (distance > 10) {
		return false;
	}
	return true;
}

void s::NpcLuaConnector::goToSpawnPosition(float maxCommandDuration) const {
	npc->lock();
	NpcCommand* previousCommand = npc->getNpcCommand();
	sf::Vector2f spawnPosition = npc->getSpawnPosition();
	if (spawnPosition.x > 5 && spawnPosition.y > 5) {
		NpcCommandMoveTo* commandMoveToSpawn = new NpcCommandMoveTo(spawnPosition, npc,
		                                                             npc->position.getMap(), npc->getServer(),
		                                                             sf::seconds(maxCommandDuration));
		commandMoveToSpawn->init();
		npc->setNpcCommand(commandMoveToSpawn);
	}
	else {
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(),
		                                      sf::seconds(maxCommandDuration)));
	}
	delete previousCommand;
	npc->unlock();
}

bool s::NpcLuaConnector::canCastSpell(SpellInfo* spell) const {
	sf::Time serverTime = npc->getServer()->getServerTime();
	float mana = npc->getAttribute(EntityAttributeType::MP, true);

	if (npc->spells.hasCooldown(spell->id, serverTime)) {
		return false;
	}

	if (mana < spell->manaCost) {
		return false;
	}

	sf::Vector2f npcPosition = npc->position.getPosition();
	sf::Vector2f targetPosition = npc->combat.target->getPosition();
	float distance = b2Distance(b2Vec2(npcPosition.x, npcPosition.y), b2Vec2(targetPosition.x, targetPosition.y));
	if (distance > spell->maxRange) {
		return false;
	}

	return true;
}

float s::NpcLuaConnector::getHp() const {
	return npc->getAttribute(EntityAttributeType::HP, true);
}

float s::NpcLuaConnector::getMaxHp() const {
	return npc->getAttribute(EntityAttributeType::BASE_HP, true);
}

float s::NpcLuaConnector::getMp() const {
	return npc->getAttribute(EntityAttributeType::MP, true);
}
