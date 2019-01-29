#include "SpellManager.h"
#include "Utils.h"
#include "SpellHolder.h"
#include "Character.h"
#include "EventPlayerStartCastSpell.h"
#include "Server.h"
#include "Account.h"
#include "Map.h"
#include "Npc.h"
#include "EventSpellCastResult.h"
#include "SpellEventNpcExecute.h"

s::SpellManager::SpellManager(): spellIds(0, 214748364), effectIds(0, 214748364) {
	dynamic = true;
	spellHolder = SpellHolder::instance();
	runningThreads = 0;
	numOfThreads = 1;
}


s::SpellManager::~SpellManager() { }

void s::SpellManager::init(Server* s) {
	runningThreads = 0;
	numOfThreads = s->serverSettings->maxSpellThreads;
	executingThreads.resize(numOfThreads, nullptr);
	threadEnded.resize(numOfThreads, true);
}

void s::SpellManager::update(sf::Time elapsedTime, s::Server* s) {
	sf::Time serverTime = s->getServerTime();

	sf::Lock lock(eventQueueMutex);

	while (!eventQueue.empty() && runningThreads < numOfThreads && eventQueue.top()->executeTime < serverTime) {

		for (int i = 0; i < numOfThreads; i++) {
			if (threadEnded[i]) {
				delete executingThreads[i];
				threadEnded[i] = false;
				runningThreads++;

				SpellEvent* e = eventQueue.top();
				eventQueue.pop();

				executingThreads[i] = new sf::Thread(std::bind(&SpellManager::threadFunc, this, e, s, i));
				executingThreads[i]->launch();
				break;
			}
		}

	}

}

void s::SpellManager::interruptSpellCast(SpellEvent* e, bool sendInfo) {
	sf::Lock lockThread(eventQueueMutex);
	e->interrupt(sendInfo);
	eventQueue.remove(e);

	delete e;
}


void s::SpellManager::threadFunc(SpellEvent* e, s::Server* s, int threadIndex) {
	e->execute(s);
	delete e;

	eventQueueMutex.lock();
	threadEnded[threadIndex] = true;
	runningThreads--;
	eventQueueMutex.unlock();
}


void s::SpellManager::handleEvent(EventPlayerStartCastSpell* event, s::Session* playerSession, s::Server* s) {
	time_t actualTime = Utils::getActualUtcTime();
	double delay = difftime(event->startCastTimestamp, actualTime);

	try {
		SpellInfo* si = spellHolder->getSpellInfo(event->spellId);
		sf::Time eventCastEndTime = s->getServerTime() + si->castingTime + sf::seconds((float)delay);

		Character* character = playerSession->getAccount()->getCharacter();
		Map* characterMap = character->position.getMap();

		SpellEvent* existCast = character->spells.getEventWithCastingSpell();
		if (existCast) {
			interruptSpellCast(existCast, false);
		}

		SpellEventCharacterExecute* e = new SpellEventCharacterExecute();
		e->setCharacter(playerSession->getAccount()->getCharacter());
		e->spellInfo = si;
		e->executeTime = eventCastEndTime;
		e->spellTarget = event->target;

		EventSpellCastResult* error = nullptr;

		switch (event->target) {
			case SpellTarget::PLAYER: {
				if (character->id == event->entityId) {
					e->targetCharacter = character;
					break;
				} 
				
				if (character->id != event->entityId) {
					Character* targetCharacter = characterMap->getCharacterById(event->entityId);
					if (targetCharacter) {
						if (targetCharacter->attributes.getAttribute(EntityAttributeType::HP, true) <= 0) {
							error = new EventSpellCastResult();
							error->entityId = character->id;
							error->entityCategory = PLAYER;
							error->spellId = si->id;
							error->result = SpellCastResultCode::TARGET_IS_DEAD;
							break;
						}
						e->targetCharacter = targetCharacter;
						break;
					}
				}
				error = new EventSpellCastResult();
				error->entityId = character->id;
				error->entityCategory = PLAYER;
				error->spellId = si->id;
				error->result = SpellCastResultCode::TARGET_NOT_EXISTS;
				break;
			}
			case SpellTarget::NPC: {
				Npc* targetNpc = characterMap->getNpcBySpawnId(event->entityId);
				if (targetNpc) {
					if (targetNpc->attributes.getAttribute(EntityAttributeType::HP, true) <= 0) {
						error = new EventSpellCastResult();
						error->entityId = character->id;
						error->entityCategory = PLAYER;
						error->spellId = si->id;
						error->result = SpellCastResultCode::TARGET_IS_DEAD;
						break;
					}
					e->targetNpc = targetNpc;
					break;
				}

				error = new EventSpellCastResult();
				error->entityId = character->id;
				error->entityCategory = PLAYER;
				error->spellId = si->id;
				error->result = SpellCastResultCode::TARGET_NOT_EXISTS;
				break;
			}
			default: {
				delete e;
				return;
			}
		}

		if (error) {
			sf::Packet* p = error->toPacket();
			playerSession->sendPacket(p);
			delete p;
			delete error;
			return;
		}
		
		eventQueueMutex.lock();
		eventQueue.push(e);
		eventQueueMutex.unlock();
	}
	catch (...) {}
}

void s::SpellManager::handleNpcCast(Npc* npc, SpellInfo* spellInfo) {
	
	try {
		sf::Time eventCastEndTime = npc->getServer()->getServerTime() + spellInfo->castingTime;

		SpellEventNpcExecute* e = new SpellEventNpcExecute();
		e->setNpc(npc);
		e->spellInfo = spellInfo;
		e->executeTime = eventCastEndTime;

		EntityType targetType = npc->combat.target->getEntityType();
		switch (targetType) {
			case EntityType::PLAYER: {
				Character* character = (Character*) npc->combat.target;
				e->targetCharacter = character;
				e->spellTarget = SpellTarget::PLAYER;
				if (character->attributes.getAttribute(EntityAttributeType::HP, true) <= 0) {
					delete e;
					return;
				}
				break;
			}
			default: {
				delete e;
				return;
			}
		}
		
		npc->spells.setCastingSpell(e);
		eventQueueMutex.lock();
		eventQueue.push(e);
		eventQueueMutex.unlock();
	}
	catch (...) {}

}

void s::SpellManager::queueEvent(SpellEvent* spellEvent) {
		eventQueueMutex.lock();
		eventQueue.push(spellEvent);
		eventQueueMutex.unlock();
}
