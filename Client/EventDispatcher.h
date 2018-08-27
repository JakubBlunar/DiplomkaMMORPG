#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <vector>
#include "GameEvent.h"
#include "Subscriber.h"
#include "ClientSettings.h"

using namespace std;

template <class T>
class EventDispatcher {
public:
	static vector<Subscriber *> subscribers;

	static void addSubscriber(Subscriber* sub) {
		if (find(subscribers.begin(), subscribers.end(), sub) == subscribers.end()) {
			subscribers.push_back(sub);
		}
	}

	static void removeSubscriber(Subscriber* sub) {
		subscribers.erase(remove(subscribers.begin(), subscribers.end(), sub), subscribers.end());
	}

	static void dispatchEvent(GameEvent* g) {
		ClientSettings::instance()->eventsMutex.lock();
		for (auto subscriber = subscribers.begin(); subscriber != subscribers.end(); ++subscriber) {
			(*subscriber)->handleEvent(g);
		}
		ClientSettings::instance()->eventsMutex.unlock();
		delete g;
	}
};

template <typename T>
vector<Subscriber*> EventDispatcher<T>::subscribers;

#endif
