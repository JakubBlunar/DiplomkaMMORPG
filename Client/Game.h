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

class SceneManager;

class Game:
	public Subscriber
{
public:
	Game();
	void					run();
	bool					isRunning() const;

	sf::RenderWindow		window;

	void					print(const std::string& message);

	SceneManager*			sceneManager;
	KeyboardManager*		keyboardManager;
	PacketManager*			packet_manager;
	bool					running;

	Map* getMap() const;
	Camera* getCamera();

	void handleEvent(GameEvent* event) override;
private:
	void					processEvents();
	void					update(sf::Time elapsedTime);
	void					render();
	void					subscribe();

	void					updateStatistics(sf::Time elapsedTime);
	void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	sf::Thread*				recieveThread;

	sf::Mutex consoleMutex;
	static const sf::Time	TimePerFrame;


	sf::CircleShape			mPlayer;
	sf::Font				mFont;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;

	//void				sendPlayerPosition();
	sf::Vector2f		playerPosition() const;


	
private:
	sf::Vector2f lastMovement;
	
	sf::Mutex					eventQueueMutex;
	std::queue<GameEvent*>		eventQueue;

	ClientEventActions*			eventActions;

	Map * gameMap;
	Camera camera;
};

#endif