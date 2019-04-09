#include "ServerSettings.h"

s::ServerSettings::ServerSettings() : port(0), max_threads(1), dbPort(0), logsEnabled(false) {}

s::ServerSettings::~ServerSettings() {

}
