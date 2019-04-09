#include "stdafx.h"
#include "GameEvent.h"

GameEvent::GameEvent() {
	id = EventId::WRONGPACKETTYPE;
}

// ReSharper disable once CppMemberFunctionMayBeConst
EventId GameEvent::getId() {
	return id;
}
