#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "IGManager.h"
#include "Game.h"

enum class SceneType {
	LOGIN,
	GAMEPLAY
};

class Scene
{
protected:
	Scene(SceneType sceneType);
	~Scene();
	IGManager* windowManager;
public:
	bool canChange;
	std::string name;
	SceneType type;
	sf::FloatRect playerBorders;

	SceneType getType() const;
	IGManager* getWindowManager() const;

	virtual void beforeChange(Game* g);
	virtual void afterChange(Game* g);
	virtual void update(Game* g, sf::Time elapsedTime);
	virtual void render(Game* g);
};

#endif 