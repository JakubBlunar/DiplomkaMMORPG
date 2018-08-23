#ifndef CLIENT_EVENT_ACTIONS_H
#define CLIENT_EVENT_ACTIONS_H

#include "EventVisitor.h"

class Game;

class ClientEventActions : public EventVisitor {
	Game* game;
public:
	ClientEventActions(Game* g);
	~ClientEventActions();


	void visit(EventMovementChange* e) override;
	void visit(EventLoginRequest* e) override;
	void visit(EventLoginResponse* e) override;
	void visit(EventCharacterChooseResponse* e) override;
};

#endif // !CLIENT_EVENT_ACTIONS_H
