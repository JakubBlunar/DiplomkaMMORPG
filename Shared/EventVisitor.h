#ifndef EVENT_VISITOR_H
#define EVENT_VISITOR_H

#include "EventLoginRequest.h"

class EventLoginResponse;
class EventMovementChange;

class EventVisitor
{
public:
	EventVisitor();

	virtual void visit(EventMovementChange* e) = 0;
	virtual void visit(EventLoginRequest* e) = 0;
	virtual void visit(EventLoginResponse* e) = 0;
};


#endif // !EVENT_VISITOR_H
