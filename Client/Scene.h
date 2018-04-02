#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "IGManager.h"
#include "Game.h"

class Scene
{
	IGManager* windowManager;

protected:
	Scene(std::string name);
	~Scene();

public:
	bool canChange;
	std::string name;
	virtual void beforeChange(Game* g);
	virtual void afterChange(Game* g);
	virtual void update(Game* g, sf::Time elapsedTime);
	virtual void render(Game* g);
};

#endif 