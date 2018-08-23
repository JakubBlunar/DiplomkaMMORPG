#ifndef EVENT_VISITOR_H
#define EVENT_VISITOR_H

#include "EventLoginRequest.h"
#include "EventCharacterChoose.h"
#include "EventCharacterChooseResponse.h"

class EventLoginResponse;
class EventMovementChange;

class EventVisitor {
public:
	EventVisitor();

	virtual void visit(EventMovementChange* e);
	virtual void visit(EventLoginRequest* e);
	virtual void visit(EventLoginResponse* e);
	virtual void visit(EventCharacterChoose* e);
	virtual void visit(EventCharacterChooseResponse* e);
};


#endif // !EVENT_VISITOR_H
