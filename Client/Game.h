﻿#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "PacketManager.h"
#include "ClientSettings.h"
#include <queue>
#include "Map.h"
#include "Camera.h"
#include "Account.h"
#include "ImGuiFonts.h"
#include <unordered_set>

struct GameMessage;
class SceneManager;

class Game {
public:
	Game();
	virtual void run();
	bool isRunning() const;

	sf::RenderWindow* window;

	bool debugInfo;
	void print(const std::string& message);

	SceneManager* sceneManager;
	PacketManager* packet_manager;
	bool running;

	Map* getMap() const;
	void changeMap(Map* map);

	Camera* getCamera();
	Account* getAccount() const;

	void setAccount(Account* account);

	ImGuiFonts fonts;

	std::unordered_set<sf::Keyboard::Key> pressedKeys;
	std::unordered_set<sf::Mouse::Button> pressedButtons;

	void addGameMessage(GameMessage* message) const;

	sf::Time getGameTime() const {
		return gameTime.getElapsedTime();
	}

	sf::Time getLatency() const;
protected:
	void processEvents();
	virtual void update(sf::Time elapsedTime);
	virtual void render();
	virtual void afterStart();

	void updateStatistics(sf::Time elapsedTime);

	sf::Thread* recieveThread;

	sf::Mutex consoleMutex;
	static const sf::Time TimePerFrame;


	sf::Font mFont;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;

	Map* gameMap;
	Camera camera;
	Account* account;
	sf::Clock gameTime;
};

#endif
