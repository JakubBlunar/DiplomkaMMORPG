#ifndef NPC_MANAGER_H
#define NPC_MANAGER_H

#include <SFML/System/Mutex.hpp>
#include <map>
#include "Manager.h"
#include "EventNpcsMovementChange.h"

#include <queue>
#include "NpcEvent.h"
#include "Subscriber.h"

namespace sf {
	class Event;
}

namespace s {
	class Npc;
	class Server;

	struct NpcUpdateEvents {
		EventNpcsMovementChange* npcsMovementChange;
	};

	class NpcManager: public Manager, public Subscriber
	{
		Server* server;
		std::map<int, Npc*> npcs;
		std::queue<NpcEvent*> npcEventQueue;
		int runningThreads;
		int MAX_RUNNING_NPC_THREADS;

		std::vector<sf::Thread*> npcEventExecutuionThreads;
		std::vector<sf::Thread*> afterExecution;
		sf::Mutex lock;
	public:
		NpcManager();
		~NpcManager();

		void init(Server* s);

		void subscribe();
		void unsubscribe();

		Npc* createNpc(int npcType);
		Npc* findNpc(int spawnId);

		void updateNpc(sf::Time elapsedTime, Npc* npc, Server* s, NpcUpdateEvents * npcUpdateEvents) const;

		void handleEvent(GameEvent* event) override;

		void executeEvent(NpcEvent* npcEvent, int index);
		void threadEnded(NpcEvent* npcEvent, int index);
		void eventExecutionThread(NpcEvent* npcEvent, int index);
	};
}

#endif