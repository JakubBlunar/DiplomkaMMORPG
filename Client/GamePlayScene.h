#pragma once
#include "Scene.h"
#include "VisibleObjectsCast.h"

class GamePlayScene :
	public Scene {
public:
	GamePlayScene(SceneType sceneType);
	~GamePlayScene();

	void beforeChange(Game* g) override;
	void afterChange(Game* g) override;
	void update(Game* g, sf::Time elapsedTime) override;
	void render(Game* g) override;

private:
	bool escPressed;
	sf::Texture BoxTexture;
	sf::Font mFont;

	VisibleObjectsCast queryCallback;
	b2AABB aabb;

	sf::Text nameOfScene;

	bool drawDebugData;
	bool fonePressed;
	sf::Sprite Sprite;
};
