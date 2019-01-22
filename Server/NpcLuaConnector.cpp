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
}

void s::NpcLuaConnector::sendNpcToRandomPositionInLocation(float maxCommandDuration) const {
	Location * l = npc->position.getLocation();
	if (l) {
		NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(l->generateRandomPoint(), npc, npc->position.getMap(), npc->getServer(),  sf::seconds(maxCommandDuration));
		commandMoveToRandom->init();
		npc->setNpcCommand(commandMoveToRandom);
	} else {
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(), sf::seconds(maxCommandDuration)));
	}
}

void s::NpcLuaConnector::sendNpcToPosition(float x, float y, float maxCommandDuration) const {
	Location * l = npc->position.getLocation();
	if (l) {
		NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(sf::Vector2f(x,y), npc, npc->position.getMap(), npc->getServer(),  sf::seconds(maxCommandDuration));
		commandMoveToRandom->init();
		npc->setNpcCommand(commandMoveToRandom);
	} else {
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(), sf::seconds(maxCommandDuration)));
	}
}

void s::NpcLuaConnector::makeNpcStay(float duration) const {
	NpcCommandStay* commandStay = new NpcCommandStay(npc, npc->position.getMap(), npc->getServer(),  sf::seconds(duration));
	npc->setNpcCommand(commandStay);
}
