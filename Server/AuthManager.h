#pragma once
#include "EventLoginRequest.h"
#include "Manager.h"
#include "EventCharacterChoose.h"

class Server;
class Session;

class AuthManager: public Manager
{
public:
	AuthManager();
	~AuthManager();

	void handleEvent(EventLoginRequest* event, Session* playerSession, Server* s) const;
	void handleEvent(EventCharacterChoose* event, Session* playerSession, Server* s) const;
};

