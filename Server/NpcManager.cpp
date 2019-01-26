#include "NpcManager.h"
#include "NpcHolder.h"
#include "ServerGlobals.h"
#include "NpcCommandMoveTo.h"
#include "Location.h"
#include "NpcCommandStay.h"
#include <spdlog/spdlog.h>
#include "EventDispatcher.h"
#include "NpcEventNpcIsIdle.h"
#include "EventNpcStatusChanged.h"
#include "EventAttributesChanged.h"
#include "Account.h"
#include "Session.h"

s::NpcManager::NpcManager(): server(nullptr), runningThreads(0), MAX_RUNNING_NPC_THREADS(0) {
	dynamic = false;
}


s::NpcManager::~NpcManager() {
	unsubscribe();
	for (int i = 0; i < MAX_RUNNING_NPC_THREADS; i++) {
		if (npcEventExecutuionThreads[i]) {
			npcEventExecutuionThreads[i]->terminate();
			delete npcEventExecutuionThreads[i];
		}

		if (afterExecution[i]) {
			afterExecution[i]->terminate();
			delete afterExecution[i];
		}
	}
}

void s::NpcManager::init(Server* s) {
	server = s;
	MAX_RUNNING_NPC_THREADS = s->serverSettings->maxNpcThreads;
	npcEventExecutuionThreads.reserve(MAX_RUNNING_NPC_THREADS);
	npcEventExecutuionThreads.clear();
	for (int i = 0; i < MAX_RUNNING_NPC_THREADS; i++) {
		npcEventExecutuionThreads.push_back(nullptr);
		afterExecution.push_back(nullptr);
	}
	runningThreads = 0;
	subscribe();
}

void s::NpcManager::subscribe() {
	EventDispatcher<NpcEventNpcIsIdle>::addSubscriber(this, server);
}

void s::NpcManager::unsubscribe() {
	EventDispatcher<NpcEventNpcIsIdle>::removeSubscriber(this, server);
}

s::Npc* s::NpcManager::createNpc(int npcType) {
	lock.lock();
	NpcHolder* nh = NpcHolder::instance();
	Npc* npc;
	try {
		npc = nh->createNpc(npcType);
		npc->setServer(server);
		int spawnId = npc->getSpawnId();
		if (npcs.find(spawnId) == npcs.end()) {
			npcs.insert(std::make_pair(spawnId, npc));
		}
		else {
			throw "Duplicate spawn id" + std::to_string(spawnId);
		}
	}
	catch (...) {
		lock.unlock();
		throw "Cannot create npc" + std::to_string(npcType);
	}

	lock.unlock();
	return npc;
}

s::Npc* s::NpcManager::findNpc(int spawnId) {
	lock.lock();
	auto find = npcs.find(spawnId);
	if (find != npcs.end()) {
		lock.unlock();
		return find->second;
	}
	lock.unlock();
	return nullptr;
}

void s::NpcManager::updateNpc(sf::Time elapsedTime, Npc* npc, Server* s, NpcUpdateEvents* npcUpdateEvents) {
	if (!npc->isAlive()) {
		return;
	}
	npc->lock();

	b2Body* body = npc->position.getBody();
	b2Vec2 position = body->GetPosition();

	sf::Vector2f transformedPosition = sf::Vector2f(position.x * METTOPIX, position.y * METTOPIX);

	npc->position.setPosition(transformedPosition);
	NpcCommand* command = npc->getNpcCommand();
	if (command && !command->isFinished()) {
		command->update(elapsedTime, npcUpdateEvents);
	}
	npc->unlock();

	/*if (!command || command->isFinished()) {
		delete command;

		Random* rand = Random::instance();
		if (rand->randomUniformFloat(0, 100) < 45) {
			Location * l = npc->getLocation();
			if (l) {
				NpcCommandMoveTo* newCommand = new NpcCommandMoveTo(l->generateRandomPoint(), npc, npc->getMap(), s,  sf::seconds(30));
				newCommand->init();
				npc->setNpcCommand(newCommand);
			}
		} else {
			NpcCommandStay* newCommand = new NpcCommandStay(npc, npc->getMap(), s,  sf::seconds(rand->randomUniformFloat(10, 30)));
			npc->setNpcCommand(newCommand);
		}
	} else {
		command->update(elapsedTime, npcUpdateEvents);
	}*/
}

void s::NpcManager::handleEvent(GameEvent* event) {
	NpcEvent* npcEvent = dynamic_cast<NpcEvent*>(event);
	if (npcEvent) {
		sf::Lock mutexLock(lock);
		if (runningThreads < MAX_RUNNING_NPC_THREADS) {
			int index = -1;
			for (int i = 0; i < MAX_RUNNING_NPC_THREADS; i++) {
				if (!npcEventExecutuionThreads[i]) {
					index = i;
					break;
				}
			}

			if (index == -1) {
				throw "Should find index";
			}

			executeEvent(npcEvent, index);
		}
		else {
			npcEventQueue.push(npcEvent);
		}
	}
}

void s::NpcManager::npcDied(Npc* npc, Entity* caster) {
	npc->lock();
	npc->setDeadTimestamp(server->getServerTime());
	npc->setNpcState(NpcState::DEAD);
	npc->setNpcCommand(nullptr);


	EventNpcStatusChanged* e = new EventNpcStatusChanged();
	e->spawnId = npc->getSpawnId();
	e->npcState = NpcState::DEAD;

	npc->position.getMap()->sendEventToAllPlayers(e);
	for (Character* const ch : npc->combat.attackingCharacters) {
		server->characterManager.handleNpcKill(ch, npc);
	}

	npc->combat.reset();
	npc->unlock();
	delete e;
}

void s::NpcManager::executeEvent(NpcEvent* npcEvent, int index) {
	sf::Lock mutexLock(lock);
	sf::Thread* t = new sf::Thread(std::bind(&NpcManager::eventExecutionThread, this, npcEvent, index));

	npcEventExecutuionThreads[index] = t;
	runningThreads++;
	npcEventExecutuionThreads[index]->launch();
}

void s::NpcManager::threadEnded(NpcEvent* npcEvent, int index) {
	npcEventExecutuionThreads[index]->wait();
	sf::Lock mutexLock(lock);

	delete npcEvent;
	runningThreads--;

	delete npcEventExecutuionThreads[index];
	npcEventExecutuionThreads[index] = nullptr;

	if (!npcEventQueue.empty()) {
		NpcEvent* nextEvent = npcEventQueue.front();
		npcEventQueue.pop();
		executeEvent(nextEvent, index);
	}
}

void s::NpcManager::eventExecutionThread(NpcEvent* npcEvent, int index) {
	EventId eventId = npcEvent->getId();
	Npc* npc = npcEvent->npc;

	sol::protected_function eventHandler = npc->luaState["handleEvent"];
	sol::protected_function_result result = eventHandler(static_cast<int>(eventId));
	if (!result.valid()) {
		sol::error err = result;
		std::string what = err.what();
		spdlog::get("log")->error("Lua script failed, npc {}, Exception: {}", npc->getType(), what);
	}

	/*
	sol::protected_function_result scriptResult = npc->luaState.script(npc->npc_script);
    if (!scriptResult.valid()) {
        sol::error err = scriptResult;
        std::string what = err.what();
        spdlog::get("log")->error("Lua script failed, npc {}, Exception: {}", npc->getType(), what);
		npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), server,  sf::seconds(20)));
    } else {
	    sol::table result = npc->luaState["resultEvents"];
	
		result.for_each([&](sol::object const& key, sol::object const& value) {
			int newEventId = key.as<int>();
			sol::table eventData = value.as<sol::table>();

			switch (newEventId) {
				case 0: {//stay param duration
					NpcCommandStay* commandStay = new NpcCommandStay(npc, npc->position.getMap(), server,  sf::seconds(eventData["duration"]));
					npc->setNpcCommand(commandStay);
					break;
				}
				case 1: { // move to random params maxDuration
					Location * l = npc->position.getLocation();
					if (l) {
						NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(l->generateRandomPoint(), npc, npc->position.getMap(), server,  sf::seconds(eventData["maxDuration"]));
						commandMoveToRandom->init();
						npc->setNpcCommand(commandMoveToRandom);
					} else {
						npc->setNpcCommand(new NpcCommandStay(npc, npc->position.getMap(), server,  sf::seconds(eventData["maxDuration"])));
					}
					break;
				}
				default:
				break;
			}
	    });
    }
	*/


	//creates handler for new thread that will wait into end of current thread and then replace it with new thread
	delete afterExecution[index];
	sf::Thread* t = new sf::Thread(std::bind(&NpcManager::threadEnded, this, npcEvent, index));
	afterExecution[index] = t;
	afterExecution[index]->launch();
}
