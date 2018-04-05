#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "PacketManager.h"
#include "ClientSettings.h"
#include "KeyboardManager.h"
#include <queue>
#include "GameEvent.h"
#include "ClientEventActions.h"

class SceneManager;

class Game
{
public:
	Game(ClientSettings* settings);
	void					run();
	bool					isRunning() const;

	sf::RenderWindow		window;
	ClientSettings*			clientSettings;

	void					print(std::string message);

	SceneManager*			sceneManager;
	KeyboardManager*		keyboardManager;
	PacketManager*			packet_manager;
	bool					running;
	void addEvent(GameEvent * e);
private:
	void					processEvents();
	void					update(sf::Time elapsedTime);
	void					render();

	void					updateStatistics(sf::Time elapsedTime);
	void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	sf::Thread*				recieveThread;

	sf::Mutex consoleMutex;
	static const sf::Time	TimePerFrame;

	static const float		PlayerSpeed;
	sf::Vector2f			movement;
	sf::CircleShape			mPlayer;
	sf::Font				mFont;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
	bool					mIsMovingUp;
	bool					mIsMovingDown;
	bool					mIsMovingRight;
	bool					mIsMovingLeft;
	void				sendPlayerPosition();
	sf::Vector2f		playerPosition() const;
	sf::Vector2f lastMovement;
	
	sf::Mutex					eventQueueMutex;
	std::queue<GameEvent*>		eventQueue;

	ClientEventActions*			eventActions;
};

#endif