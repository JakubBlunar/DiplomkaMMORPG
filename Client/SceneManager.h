#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <map>
#include <string>
#include "Game.h"
#include "Scene.h"

class SceneManager {
	std::map<SceneType, Scene*> scenes;
	Scene* actualScene;
public:
	SceneManager();
	~SceneManager();

	SceneType getTypeOfActualScene() const;
	Scene* getActualScene() const;
	void changeScene(SceneType sceneType);
	void render(Game* g) const;
	void update(Game* g, sf::Time elapsedTime) const;
};

#endif
