#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H


class GameEvent;

class Subscriber {
public:
	virtual void handleEvent(GameEvent* event) = 0;
protected:
	Subscriber();
};

#endif
