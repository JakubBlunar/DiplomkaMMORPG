#pragma once
#include "IGWindow.h"
class IGGameMenu :
	public IGWindow
{
public:
	IGGameMenu();
	~IGGameMenu();
protected:
	void render(Game* g) override;
	void beforeRender(Game * game) override;
};

