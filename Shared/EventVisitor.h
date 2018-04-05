#ifndef EVENT_VISITOR_H
#define EVENT_VISITOR_H

class EventMovementChange;

class EventVisitor
{
public:
	EventVisitor();

	virtual void visit(EventMovementChange* e) = 0;
};


#endif // !EVENT_VISITOR_H
