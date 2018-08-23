#include "Manager.h"


s::Manager::Manager() {
	dynamic = false;
}


s::Manager::~Manager() {
}

bool s::Manager::isDynamic() const {
	return dynamic;
}
