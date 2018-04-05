#include "stdafx.h"
#include "GameEvent.h"
#include "EventVisitor.h"



GameEvent::GameEvent()
{
	id = EventId::WRONGPACKETTYPE;
}

EventId GameEvent::getId()
{
	return id;
}

