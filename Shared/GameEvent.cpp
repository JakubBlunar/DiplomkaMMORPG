#include "stdafx.h"
#include "GameEvent.h"
#include "EventVisitor.h"


GameEvent::GameEvent() {
	id = EventId::WRONGPACKETTYPE;
}

// ReSharper disable once CppMemberFunctionMayBeConst
EventId GameEvent::getId() {
	return id;
}
