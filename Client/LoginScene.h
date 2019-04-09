#ifndef LOGIN_SCENE_H
#define LOGIN_SCENE_H

#include "Scene.h"

class LoginScene :
	public Scene, public Subscriber {
public:
	LoginScene(SceneType sceneType, Game* g);
	~LoginScene();

	void beforeChange() override;
	void afterChange() override;
	void update(sf::Time elapsedTime) override;
	void render() override;

	void onKeyPress(sf::Keyboard::Key key) override;
	void handleEvent(GameEvent* event) override;

	RenderSprite background;
};

#endif
