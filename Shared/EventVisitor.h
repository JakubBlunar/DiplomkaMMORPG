#ifndef EVENT_VISITOR_H
#define EVENT_VISITOR_H

#include "EventLoginRequest.h"
#include "EventCharacterChoose.h"
#include "EventCharacterChooseResponse.h"
#include "EventCharacterMapJoin.h"
#include "EventNpcMovementChange.h"

class EventNpcsMovementChange;
class EventCharacterMapLeave;
class EventLoginResponse;
class EventMovementChange;
class EventCharacterLogout;

class EventVisitor {
public:
	EventVisitor();

	virtual void visit(EventMovementChange* e);
	virtual void visit(EventLoginRequest* e);
	virtual void visit(EventLoginResponse* e);
	virtual void visit(EventCharacterChoose* e);
	virtual void visit(EventCharacterChooseResponse* e);
	virtual void visit(EventCharacterMapJoin* e);
	virtual void visit(EventCharacterMapLeave* e);
	virtual void visit(EventCharacterLogout* e);
	virtual void visit(EventNpcMovementChange* e);
	virtual void visit(EventNpcsMovementChange* e);
};

#endif // !EVENT_VISITOR_H
