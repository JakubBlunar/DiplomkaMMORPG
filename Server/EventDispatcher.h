#ifndef S_EVENT_DISPATCHER_H
#define S_EVENT_DISPATCHER_H

#include <list>
#include "GameEvent.h"
#include "ServerSettings.h"
#include "../Shared/Subscriber.h"
#include "NpcEvent.h"
#include "Server.h"

namespace s {

	template <class T>
	class EventDispatcher {
	public:
		static std::list<Subscriber *> subscribers;

		static void addSubscriber(Subscriber* sub, Server* s) {
			s->serverSettings->eventMutex.lock();
			if (find(subscribers.begin(), subscribers.end(), sub) == subscribers.end()) {
				subscribers.push_back(sub);
			}
			s->serverSettings->eventMutex.unlock();
		}

		static void removeSubscriber(Subscriber* sub, Server* s) {
			s->serverSettings->eventMutex.lock();
			subscribers.erase(remove(subscribers.begin(), subscribers.end(), sub), subscribers.end());
			s->serverSettings->eventMutex.unlock();
		}

		static void dispatchEvent(GameEvent* g, Server* s) {
			s->serverSettings->eventMutex.lock();
			for (auto subscriber = subscribers.begin(); subscriber != subscribers.end(); ++subscriber) {
				(*subscriber)->handleEvent(g);
			}
 			if (dynamic_cast<NpcEvent*> (g) == nullptr) {
				delete g;
			}
			s->serverSettings->eventMutex.unlock();
		}
	};

	template <typename T>
	std::list<Subscriber*> EventDispatcher<T>::subscribers;
}
#endif
