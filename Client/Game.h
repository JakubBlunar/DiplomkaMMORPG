#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "PacketManager.h"
#include "ClientSettings.h"

class Game
{
public:
	Game(ClientSettings* settings);
	void					run();

	bool					isRunning() const;

	sf::RenderWindow		mWindow;
	sf::Color				bgColor;
	ClientSettings*			clientSettings;

	void					print(std::string message);

private:
	void					processEvents();
	void					update(sf::Time elapsedTime);
	void					render();

	void					updateStatistics(sf::Time elapsedTime);
	void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	sf::Thread*				recieveThread;
	PacketManager*			packet_manager;

	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

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
	bool					running;

	void				sendPlayerPosition();
	sf::Vector2f		playerPosition() const;

	sf::Vector2f lastMovement;

	sf::Mutex consoleMutex;
};

#endif