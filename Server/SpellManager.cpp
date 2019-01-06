#include "SpellManager.h"
#include "Utils.h"
#include "SpellHolder.h"
#include "Character.h"
#include "EventPlayerStartCastSpell.h"
#include "Server.h"
#include "Account.h"

s::SpellManager::SpellManager(): spellIds(0, 214748364), effectIds(0, 214748364) {
	dynamic = true;
	spellHolder = SpellHolder::instance();

	runningThreads = 0;
	numOfThreads = 6;
	executingThreads.resize(numOfThreads, nullptr);
	threadEnded.resize(numOfThreads, true);
}


s::SpellManager::~SpellManager() { }

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

void s::SpellManager::interruptSpellCast(SpellEventExecute * e)
{
	sf::Lock lockThread(eventQueueMutex);
	e->getCharacter()->setCastingSpell(nullptr);
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

		SpellEventExecute* existCast = character->getEventWithCastingSpell();
		if (existCast) {
			interruptSpellCast(existCast);
		}

		SpellEventExecute* e = new SpellEventExecute();
		e->setCharacter(playerSession->getAccount()->getCharacter());
		e->spellInfo = si;
		e->executeTime = eventCastEndTime;

		eventQueueMutex.lock();
		eventQueue.push(e);
		eventQueueMutex.unlock();

	}
	catch (...) {}
}
