#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "GameEvent.h"

class Subscriber {
public:
	virtual void handleEvent(GameEvent* event) = 0;
protected:
	Subscriber();
	~Subscriber();
};

#endif
