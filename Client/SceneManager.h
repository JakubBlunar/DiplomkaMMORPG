#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <map>
#include <string>
#include "Game.h"

class Scene;

class SceneManager
{
	std::map<std::string, Scene*> scenes;
	Scene* actualScene;
public:
	SceneManager();
	~SceneManager();
		
	void changeScene(std::string name);
	void render(Game* g) const;
	void update(Game* g, sf::Time elapsedTime) const;
};

#endif