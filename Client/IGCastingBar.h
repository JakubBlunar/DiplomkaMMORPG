#ifndef IG_CASTING_BAR_H
#define IG_CASTING_BAR_H

#include "IGWindow.h"

class Player;
class IGCastingBar :
	public IGWindow
{
public:
	IGCastingBar();
	virtual ~IGCastingBar();

	void setPlayer(Player* player) {
		this->player = player;
	}

protected:
	void render(Game* g, IGManager* manager) override;
	void beforeRender(Game* game) override;

	Player* player;
};

#endif
