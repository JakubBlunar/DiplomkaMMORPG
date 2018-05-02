#pragma once
#include "EventLoginRequest.h"
#include "Manager.h"

class Server;
class Session;

class AuthManager: public Manager
{
public:
	AuthManager();
	~AuthManager();

	void handleEvent(EventLoginRequest* event, Session* playerSession, Server* s) const;
};

