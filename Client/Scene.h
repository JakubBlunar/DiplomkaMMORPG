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

struct GameMessage {
	std::string message;
	sf::Time displayTime;
	sf::Time startTime;
	sf::Color color = sf::Color::Yellow;
	int size = 20;
};

class Scene {
	sf::Text messageText;
protected:
	Scene(SceneType sceneType, Game* g);
	~Scene();
	IGManager* windowManager;
	Game* game;

	std::queue<GameMessage*> waitingMessages;
	std::vector<GameMessage*> messages;

public:
	bool canChange;
	std::string name;
	SceneType type;
	sf::FloatRect playerBorders;

	virtual void onKeyPress(sf::Keyboard::Key key);
	virtual void onKeyRelease(sf::Keyboard::Key key);
	virtual void onClick(sf::Mouse::Button event, sf::Vector2f position);

	SceneType getType() const;
	IGManager* getWindowManager() const;

	virtual void beforeChange();
	virtual void afterChange();
	virtual void update(sf::Time elapsedTime);
	virtual void render();

	void addGameMessage(GameMessage* message);

};

#endif
