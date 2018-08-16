#pragma once
#include "IGWindow.h"
class IGLoginCredentials :
	public IGWindow
{
public:
	IGLoginCredentials();
	~IGLoginCredentials();

	void render(Game* g, IGManager* manager) override;
	void beforeRender(Game* g) override;
private:
	char buffLogin[256];
	char buffPassword[256];
};

