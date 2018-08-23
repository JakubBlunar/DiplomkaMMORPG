#ifndef LOGIN_SCENE_H
#define LOGIN_SCENE_H

#include <string>
#include "Scene.h"

class LoginScene :
	public Scene {
public:
	LoginScene(SceneType sceneType);
	~LoginScene();

	void beforeChange(Game* g) override;
	void afterChange(Game* g) override;
	void update(Game* g, sf::Time elapsedTime) override;
	void render(Game* g) override;
};

#endif
