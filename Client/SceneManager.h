#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <map>
#include <string>
#include "Game.h"
#include "Scene.h"

class SceneManager {
	std::map<SceneType, Scene*> scenes;
	Scene* actualScene;
	Scene* sceneToChange;

	void switchScene(Scene* scene);
public:
	SceneManager(Game* g);
	~SceneManager();

	SceneType getTypeOfActualScene() const;
	Scene* getActualScene() const;
	void changeScene(SceneType sceneType);
	void render() const;
	void update(sf::Time elapsedTime);
};

#endif
