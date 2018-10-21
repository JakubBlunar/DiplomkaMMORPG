#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "IGManager.h"
#include "Game.h"

enum class SceneType {
	LOGIN,
	GAMEPLAY,
	CHARACTER_CHOOSE
};

class Scene {
protected:
	Scene(SceneType sceneType, Game* g);
	~Scene();
	IGManager* windowManager;
	Game* game;

	bool isLeftClicked;
	bool isRightClicked;
	
	virtual void onClick(sf::Mouse::Button event, sf::Vector2f position);
public:
	bool canChange;
	std::string name;
	SceneType type;
	sf::FloatRect playerBorders;
	


	SceneType getType() const;
	IGManager* getWindowManager() const;

	virtual void beforeChange();
	virtual void afterChange();
	virtual void update(sf::Time elapsedTime);
	virtual void render();

	
};

#endif
