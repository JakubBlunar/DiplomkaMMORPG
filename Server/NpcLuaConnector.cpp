#include "NpcLuaConnector.h"
#include "Npc.h"
#include "NpcCommandStay.h"
#include "NpcCommandMoveTo.h"
#include "Location.h"
#include <spdlog/spdlog.h>


s::NpcLuaConnector::NpcLuaConnector(Npc* npc)
{
	this->npc = npc;
}


s::NpcLuaConnector::~NpcLuaConnector()
{
}

void s::NpcLuaConnector::connect() {
	npc->lock();
	npc->luaState["npc"] = npc;
	npc->luaState.new_usertype<Npc>("Npc",
	                           "getName", &Npc::getName,
	                           "getSpawnId", &Npc::getSpawnId,
	                           "getAttribute", &Npc::getAttribute,
	                           "getState", &Npc::getNpcState
	);

	npc->luaState.set_function("sendNpcToRandomPositionInLocation", &NpcLuaConnector::sendNpcToRandomPositionInLocation, this);
	npc->luaState.set_function("sendNpcToPosition", &NpcLuaConnector::sendNpcToPosition, this);
	npc->luaState.set_function("makeNpcStay", &NpcLuaConnector::makeNpcStay, this);
	npc->luaState.set_function("castRandomSpell", &NpcLuaConnector::castRandomSpell, this);

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
	Location * l = npc->position.getLocation();
	NpcCommand* previousCommand = npc->getNpcCommand();
	if (l) {
		NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(l->generateRandomPoint(), npc, npc->position.getMap(), npc->getServer(),  sf::seconds(maxCommandDuration));
		commandMoveToRandom->init();
		npc->setNpcCommand(commandMoveToRandom);
	} else {
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(), sf::seconds(maxCommandDuration)));
	}
	delete previousCommand;
	npc->unlock();
}

void s::NpcLuaConnector::sendNpcToPosition(float x, float y, float maxCommandDuration) const {
	npc->lock();
	Location * l = npc->position.getLocation();
	NpcCommand* previousCommand = npc->getNpcCommand();
	if (l) {
		NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(sf::Vector2f(x,y), npc, npc->position.getMap(), npc->getServer(),  sf::seconds(maxCommandDuration));
		commandMoveToRandom->init();
		npc->setNpcCommand(commandMoveToRandom);
	} else {
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(), sf::seconds(maxCommandDuration)));
	}
	delete previousCommand;
	npc->unlock();
}

void s::NpcLuaConnector::makeNpcStay(float duration) const {
	npc->lock();
	NpcCommand* previousCommand = npc->getNpcCommand();
	NpcCommandStay* commandStay = new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(),  sf::seconds(duration));
	npc->setNpcCommand(commandStay);
	delete previousCommand;
	npc->unlock();
}

void s::NpcLuaConnector::castRandomSpell() const {
	

	sf::Time serverTime = npc->getServer()->getServerTime();
	if (npc->spells.hasAllSpellCooldown(serverTime)) {
		return;
	}

	std::map<int, SpellInfo*>* availableSpells = npc->spells.getAvailableSpells();
	SpellInfo* found = nullptr;
	for ( auto it = availableSpells->begin(); it != availableSpells->end(); ++it) {
		if (!npc->spells.hasCooldown(it->first, serverTime)) {
			found = it->second;
		} 
	}

	if (found) {
		npc->getServer()->spellManager.handleNpcCast(npc, found);	
	}
	
}
