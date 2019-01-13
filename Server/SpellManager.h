#ifndef S_SPELL_MANAGER_H
#define S_SPELL_MANAGER_H

#include "IDManager.h"
#include "Manager.h"
#include "PriorityQueue.h"
#include "SpellHolder.h"
#include "SpellEvent.h"

class EventPlayerStartCastSpell;

namespace s {
	class Session;

	struct SpellEventComparer
	{
	    bool operator()(const SpellEvent *t1, const SpellEvent *t2) const
	    {
	       return t1->executeTime < t2->executeTime;
	    }
	};


	class SpellManager : public s::Manager {
		IDManager<sf::Int32> spellIds;
		IDManager<sf::Int32> effectIds;
		
		SpellHolder* spellHolder;

		PriorityQueue<SpellEvent*, std::vector<SpellEvent*>, SpellEventComparer> eventQueue;

		std::vector<sf::Thread*> executingThreads;
		std::vector<bool> threadEnded;

		sf::Mutex eventQueueMutex;
		int numOfThreads;
		int runningThreads;

		void threadFunc(SpellEvent* e, s::Server* s, int threadIndex);
	public:
		SpellManager();
		virtual ~SpellManager();

		void update(sf::Time elapsedTime, s::Server* s) override;
		void interruptSpellCast(SpellEvent* e, bool sendInfo);
		void handleEvent(EventPlayerStartCastSpell* event, s::Session* playerSession, s::Server* s);
		void queueEvent(SpellEvent* spellEvent);
	};
}
#endif
