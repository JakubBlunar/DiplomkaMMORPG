#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "PacketManager.h"
#include "ClientSettings.h"
#include "KeyboardManager.h"
#include <queue>
#include "GameEvent.h"
#include "ClientEventActions.h"
#include "Subscriber.h"
#include "Map.h"
#include "Camera.h"
#include "Account.h"

class SceneManager;

class Game :
	public Subscriber {
public:
	Game();
	void run();
	bool isRunning() const;

	sf::RenderWindow window;

	void print(const std::string& message);

	SceneManager* sceneManager;
	KeyboardManager* keyboardManager;
	PacketManager* packet_manager;
	bool running;

	Map* getMap() const;
	void changeMap(Map* map);

	Camera* getCamera();
	Account* getAccount() const;

	void setAccount(Account* account);

	void handleEvent(GameEvent* event) override;

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();
	void subscribe();

	void updateStatistics(sf::Time elapsedTime);

	sf::Thread* recieveThread;

	sf::Mutex consoleMutex;
	static const sf::Time TimePerFrame;


	sf::Font mFont;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;

	//void				sendPlayerPosition();

	sf::Mutex eventQueueMutex;
	std::queue<GameEvent*> eventQueue;

	ClientEventActions* eventActions;

	Map* gameMap;
	Camera camera;
	Account* account;
};

#endif
