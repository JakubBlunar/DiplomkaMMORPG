#pragma once
#include "IGWindow.h"
#include "Player.h"

class IGCharacterInfo :
	public IGWindow
{
public:
	IGCharacterInfo();
	virtual ~IGCharacterInfo();
	void setPlayer(Player* player) {
		this->player = player;
		this->attributes = player->getAttributesComponent();
	}

protected:
	void render(Game* g, IGManager* manager) override;

	Player* player;
	AttributesComponent* attributes;

};

