#pragma once
#include "Scene.h"
#include "VisibleObjectsCast.h"
#include "Astar.h"
#include "IGEntityInfo.h"
#include "IGActionBar.h"

class GamePlayScene :
	public Scene {
public:
	GamePlayScene(SceneType sceneType, Game* g);
	~GamePlayScene();

	void beforeChange() override;
	void afterChange() override;
	void update(sf::Time elapsedTime) override;
	void render() override;
	void onKeyPress(sf::Keyboard::Key key) override;

	void onClick(sf::Mouse::Button event, sf::Vector2f position) override;
private:
	IGEntityInfo* playerInfoWindow;
	IGEntityInfo* targetInfoWindow;
	IGActionBar* actionBarWindow;

	sf::CircleShape targetArrow;
	Player* player;

	sf::Texture BoxTexture;
	sf::Font mFont;

	VisibleObjectsCast queryCallback;
	b2AABB aabb;

	bool drawDebugData;
	sf::Sprite Sprite;

	std::list<sf::Vector2f> path;
};
