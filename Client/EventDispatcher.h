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
		ClientSettings::instance()->eventsMutex.lock();
		if (find(subscribers.begin(), subscribers.end(), sub) == subscribers.end()) {
			subscribers.push_back(sub);
		}
		ClientSettings::instance()->eventsMutex.unlock();
	}

	static void removeSubscriber(Subscriber* sub) {
		ClientSettings::instance()->eventsMutex.lock();
		subscribers.erase(remove(subscribers.begin(), subscribers.end(), sub), subscribers.end());
		ClientSettings::instance()->eventsMutex.unlock();
	}

	static void dispatchEvent(GameEvent* g) {
		ClientSettings::instance()->eventsMutex.lock();
		for (auto subscriber = subscribers.begin(); subscriber != subscribers.end(); ++subscriber) {
			(*subscriber)->handleEvent(g);
		}
		delete g;
		ClientSettings::instance()->eventsMutex.unlock();
	}
};

template <typename T>
vector<Subscriber*> EventDispatcher<T>::subscribers;

#endif
