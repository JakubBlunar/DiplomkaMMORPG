#ifndef IG_ACTION_BAR_H
#define IG_ACTION_BAR_H

#include "IGWindow.h"
#include "Player.h"

class IGActionBar :
	public IGWindow
{
public:
	IGActionBar();
	virtual ~IGActionBar();

	void setPlayer(Player* player) {
		this->player = player;
		this->spells = player->getSpells(); 
	} 
protected:
	void render(Game* g, IGManager* manager) override;
	void beforeRender(Game* game) override;

	Player* player;
	std::vector<SpellInfo*>* spells;
};

#endif