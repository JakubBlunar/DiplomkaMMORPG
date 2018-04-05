#ifndef CLIENT_EVENT_ACTIONS_H
#define CLIENT_EVENT_ACTIONS_H

#include "EventVisitor.h"

class Game;

class ClientEventActions : public EventVisitor
{
	Game* game;
public:
	ClientEventActions(Game *g);
	~ClientEventActions();


	void visit(EventMovementChange* e);
};

#endif // !CLIENT_EVENT_ACTIONS_H