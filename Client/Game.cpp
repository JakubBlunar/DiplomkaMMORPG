#include "Game.h"
#include "PacketTypes.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "PacketManager.h"
#include <limits.h>

const float Game::PlayerSpeed = 30.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game(ClientSettings* clientSettings)
	: mWindow(sf::VideoMode(800, 600), "SFML Application", sf::Style::Close | sf::Style::Resize)
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
{
	this->clientSettings = clientSettings;
	this->packet_manager = new PacketManager(this, true);

	mWindow.setFramerateLimit(120);

	mPlayer.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	mPlayer.setRadius(15);
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

	/*IGChat* c = new IGChat();
	IGConsole* con = new IGConsole();

	igManager.addWindow("chat", c);
	igManager.addWindow("console", con);

	igManager.OpenAll();*/

	mWindow.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(mWindow);
	mWindow.resetGLStates();

	recieveThread = new sf::Thread(&PacketManager::startRecieve, packet_manager);
	recieveThread->launch();


	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		const sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		ImGui::SFML::Update(mWindow, timeSinceLastUpdate);
		updateStatistics(elapsedTime);
		render();
	}

	ImGui::SFML::Shutdown();
	running = false;
	recieveThread->wait();
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
			break;
		}

		ImGui::SFML::ProcessEvent(event);

		/*if (igManager.AnyWindowFocused())
		{
			continue;
		}*/

		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Resized:
			mWindow.setView(sf::View(sf::FloatRect(0, 0, 800, 600)));
		default:;
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
	sendPlayerPosition();
}

void Game::render()
{
	mWindow.clear(bgColor);


	mWindow.draw(mPlayer);


	//igManager.drawAll();
	ImGui::SFML::Render(mWindow);

	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateStatistics(const sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us\n" +
			"Latency = " + std::to_string(packet_manager->statistics.latency())
		);

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

bool Game::isRunning() const
{
	return running;
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
	
	if (key == sf::Keyboard::W)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight = isPressed;

}

sf::Vector2f Game::playerPosition() const
{
	return mPlayer.getPosition();
}


void Game::sendPlayerPosition()
{
	sf::Vector2f position = playerPosition();
	if (lastMovement != movement)
	{
		sf::Packet packet;

		packet << pt::POSITION << position.x << position.y << movement.x << movement.y;

		packet_manager->sendPacket(&packet, -1);

		lastMovement = movement;
	}

}