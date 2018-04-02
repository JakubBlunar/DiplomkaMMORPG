#include "Game.h"

#include <imgui-SFML.h>
#include "IGWindow.h"

const float Game::PlayerSpeed = 30.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: igManager(this)
	, window(sf::VideoMode(1360, 768), "SFML Application", sf::Style::Close| sf::Style::Resize)
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
	, running(false)
{
	window.setFramerateLimit(120);

	mPlayer.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	mPlayer.setSize(sf::Vector2f(32.f, 64.f));
	mPlayer.setPosition(100.f, 100.f);

	mFont.loadFromFile("Data/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(12);
	mStatisticsText.setFillColor(sf::Color::Blue);
	bgColor = sf::Color::White;
}

void Game::run()
{
	running = true;

	igManager.OpenAll();

	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);
	window.resetGLStates();



	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (window.isOpen())
	{
		const sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		ImGui::SFML::Update(window, timeSinceLastUpdate);
		updateStatistics(elapsedTime);
		render();
	}

	ImGui::SFML::Shutdown();
	running = false;
}

void Game::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
		{
			window.close();
			break;
		}


		ImGui::SFML::ProcessEvent(event);

		if (igManager.AnyWindowFocused())
		{
			continue;
		}

		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Resized:
			window.setView(sf::View(sf::FloatRect(0, 0, 1360, 768)));
		default: ;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{


	movement = sf::Vector2f(0.f, 0.f);
	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
		movement.x += PlayerSpeed;

	mPlayer.move(movement * elapsedTime.asSeconds());

}

void Game::render()
{
	window.clear(bgColor);

	window.draw(mPlayer);

	igManager.drawAll();
	ImGui::SFML::Render(window);

	window.draw(mStatisticsText);
	window.display();
}

void Game::updateStatistics(const sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::handlePlayerInput(const sf::Keyboard::Key key, const bool isPressed)
{
	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;
	else if (key == sf::Keyboard::R)
	{
		igManager.Open("chat");
	}
}

sf::Vector2f Game::playerPosition() const
{
	return mPlayer.getPosition();
}


bool Game::isRunning() const
{
	return running;
}
