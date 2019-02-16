#pragma once
#include "Scene.h"
#include "VisibleObjectsCast.h"
#include "Astar.h"
#include "IGEntityInfo.h"
#include "IGActionBar.h"
#include "IGCharacterInfo.h"
#include "IGConsole.h"
#include "IGHelp.h"
#include "IGCastingBar.h"
#include "IGLearnNewSpell.h"

class GamePlayScene :
	public Scene, public Subscriber {
public:
	GamePlayScene(SceneType sceneType, Game* g);
	~GamePlayScene();

	void beforeChange() override;
	void afterChange() override;
	void update(sf::Time elapsedTime) override;
	void render() override;
	void onKeyPress(sf::Keyboard::Key key) override;

	void onClick(sf::Mouse::Button event, sf::Vector2f position) override;
	void handleEvent(GameEvent* event) override;
private:
	IGEntityInfo* playerInfoWindow;
	IGEntityInfo* targetInfoWindow;
	IGActionBar* actionBarWindow;
	IGCharacterInfo * characterInfoWindow;
	IGConsole* console;
	IGHelp* helpWindow;
	IGCastingBar* castingBarWindow;
	IGLearnNewSpell* learnNewSpellWindow;

	sf::CircleShape targetArrow;
	Player* player;

	sf::Texture BoxTexture;
	sf::Font mFont;

	VisibleObjectsCast queryCallback;
	b2AABB aabb;

	sf::Sprite Sprite;

	std::list<sf::Vector2f> path;
};
