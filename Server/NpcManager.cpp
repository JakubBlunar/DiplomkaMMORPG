#include "NpcManager.h"
#include "NpcHolder.h"
#include "ServerGlobals.h"
#include "Random.h"
#include "NpcCommandMoveTo.h"
#include "Location.h"
#include "NpcCommandStay.h"
#include <spdlog/spdlog.h>
#include "EventDispatcher.h"
#include "NpcEventNpcIsIdle.h"

s::NpcManager::NpcManager(): runningThreads(0), MAX_RUNNING_NPC_THREADS(0) {
	dynamic = false;
}


s::NpcManager::~NpcManager()
{
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

void s::NpcManager::subscribe()
{
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
		int spawnId = npc->getSpawnId();
		if (npcs.find(spawnId) == npcs.end()) {
			npcs.insert(std::make_pair(spawnId, npc));
		} else {
			throw "Duplicate spawn id" + std::to_string(spawnId);
		}
	} catch(...) {
		lock.unlock();
		throw "Cannot create npc" + std::to_string(npcType);
	}

	lock.unlock();
	return npc;
}

s::Npc* s::NpcManager::findNpc(int spawnId)
{
	lock.lock();
	auto find = npcs.find(spawnId);
	if (find != npcs.end()) {
		lock.unlock();
		return find->second;
	}
	lock.unlock();
	return nullptr;
}

void s::NpcManager::updateNpc(sf::Time elapsedTime, Npc* npc, Server * s, NpcUpdateEvents * npcUpdateEvents) const {
	if (!npc->isAlive()) {
		return;
	}
	b2Body* body = npc->getBody();
	b2Vec2 position = body->GetPosition();
	
	sf::Vector2f transformedPosition = sf::Vector2f(position.x * METTOPIX, position.y * METTOPIX); 

	npc->setPosition(transformedPosition);
	NpcCommand* command = npc->getNpcCommand();
	if(command && !command->isFinished()) {
		command->update(elapsedTime, npcUpdateEvents);
	}


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
		} else {
			npcEventQueue.push(npcEvent);
		}
	}
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
	npc->lock();
	NpcCommand* c = npc->getNpcCommand();
	delete c;

	npc->luaState["event"] = sol::nullopt;
	npc->luaState["event"] = npc->luaState.create_table_with(
		"id", eventId
	);

	npc->unlock();

	const char* script = R"(
		printf = function(s,...)
           return io.write(s:format(...))
        end
		
		function tableHasKey(table,key)
		    return table[key] ~= nil
		end

		events = {}
		resultEvents = {}

		--[Npc is idle]--
		events[18] = function()
			rand = math.random()
			if rand < 0.5 then
				resultEvents[1] = {
					maxDuration = 30
				}
			else 
				resultEvents[0] = {
					duration = math.random(10, 20)
				}
			end	
		end
		
		if (tableHasKey(events, event.id))
		then
			events[event.id]()
		end
	)";

	npc->luaState.script(script);

	sol::table result = npc->luaState["resultEvents"];
	
	result.for_each([&](sol::object const& key, sol::object const& value) {
		int newEventId = key.as<int>();
		sol::table eventData = value.as<sol::table>();

		switch (newEventId) {
			case 0: {//stay param duration
				NpcCommandStay* commandStay = new NpcCommandStay(npc, npc->getMap(), server,  sf::seconds(eventData["duration"]));
				npc->setNpcCommand(commandStay);
				break;
			}
			case 1: { // move to random params maxDuration
				Location * l = npc->getLocation();
				if (l) {
					NpcCommandMoveTo* commandMoveToRandom = new NpcCommandMoveTo(l->generateRandomPoint(), npc, npc->getMap(), server,  sf::seconds(eventData["maxDuration"]));
					commandMoveToRandom->init();
					npc->setNpcCommand(commandMoveToRandom);
				} else {
					npc->setNpcCommand(new NpcCommandStay(npc, npc->getMap(), server,  sf::seconds(eventData["maxDuration"])));
				}
				break;
			}
			default:
			break;
		}
    });

	//creates handler for new thread that will wait into end of current thread and then replace it with new thread
	delete afterExecution[index];
	sf::Thread* t = new sf::Thread(std::bind(&NpcManager::threadEnded, this, npcEvent, index));
	afterExecution[index] = t;
	afterExecution[index]->launch();
}



