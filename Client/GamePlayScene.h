#pragma once
#include "Scene.h"
#include "VisibleObjectsCast.h"
#include "Astar.h"
#include "IGEntityInfo.h"

class GamePlayScene :
	public Scene {
public:
	GamePlayScene(SceneType sceneType, Game* g);
	~GamePlayScene();

	void beforeChange() override;
	void afterChange() override;
	void update(sf::Time elapsedTime) override;
	void render() override;
private:
	IGEntityInfo* targetInfoWindow;

	void onClick(sf::Mouse::Button event, sf::Vector2f position) override;

	bool escPressed;
	sf::Texture BoxTexture;
	sf::Font mFont;

	VisibleObjectsCast queryCallback;
	b2AABB aabb;

	sf::Text nameOfScene;

	bool drawDebugData;
	bool fonePressed;
	sf::Sprite Sprite;

	std::list<sf::Vector2f> path;

	bool mousePressed;
};
