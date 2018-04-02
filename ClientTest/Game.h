#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "IGManager.h"

class Game
{
public:
	Game();
	void					run();

	bool					isRunning() const;

	IGManager				igManager;
	sf::RenderWindow		window;
	sf::Color				bgColor;
private:
	void					processEvents();
	void					update(sf::Time elapsedTime);
	void					render();

	void					updateStatistics(sf::Time elapsedTime);
	void					handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	sf::Vector2f			playerPosition() const;

private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::Vector2f			movement;

	sf::RectangleShape		mPlayer;
	sf::Font				mFont;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;

	std::size_t				mStatisticsNumFrames;
	bool					mIsMovingUp;
	bool					mIsMovingDown;
	bool					mIsMovingRight;
	bool					mIsMovingLeft;
	bool					running;

	sf::Vector2f lastMovement;
};

#endif