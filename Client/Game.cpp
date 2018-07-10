#include "Game.h"
#include "EventId.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "PacketManager.h"
#include <iostream>
#include "SceneManager.h"
#include "EventMovementChange.h"
#include "ClientEventActions.h"
#include "EventDispatcher.h"

const float Game::PlayerSpeed = 30.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: window(sf::VideoMode(1360, 768), "SFML Application", sf::Style::Close | sf::Style::Resize)
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
	this->packet_manager = new PacketManager(this);
	this->sceneManager = new SceneManager();
	this->keyboardManager = new KeyboardManager();
	this->eventActions = new ClientEventActions(this);

	window.setFramerateLimit(120);

	mPlayer.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
	mPlayer.setRadius(15);
	mPlayer.setPosition(100.f, 100.f);

	mFont.loadFromFile("Data/Sansation.ttf");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(12);
	mStatisticsText.setFillColor(sf::Color::Blue);
	subscribe();

	gameMap = new Map();
}

void Game::run()
{
	running = true;

	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);
	window.resetGLStates();

	recieveThread = new sf::Thread(&PacketManager::startRecieve, packet_manager);
	recieveThread->launch();

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (window.isOpen() && running)
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
	
	window.setView(*camera.getView());

	ImGui::SFML::Shutdown();
}

Map * Game::getMap() const
{
	return gameMap;
}

Camera* Game::getCamera()
{
	return &camera;
}


void Game::handleEvent(GameEvent* event)
{
	event->accept(eventActions);
}

void Game::processEvents()
{

	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			running = false;
			window.close();
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
			keyboardManager->handlePlayerInput(event.key.code, true);
			handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			keyboardManager->handlePlayerInput(event.key.code, false);
			handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Resized:
			window.setView(*camera.getView());
			break;
		case sf::Event::MouseWheelScrolled:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				camera.adjustScale(event.mouseWheelScroll.delta * -0.02);
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
			{
				camera.adjustRotation(event.mouseWheelScroll.delta * 10);
			}
			break;
		default:;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	ClientSettings::instance()->eventsMutex.lock();
	sceneManager->update(this, elapsedTime);
	camera.update(elapsedTime, this);

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
	ClientSettings::instance()->eventsMutex.unlock();
}

void Game::render()
{
	window.clear(sf::Color::Black);

	if (window.hasFocus()) {
		sceneManager->render(this);
	}

	ImGui::SFML::Render(window);

	if (window.hasFocus()) {
		window.draw(mStatisticsText);
	}
	
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
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us\n" +
			"Latency = " + std::to_string(packet_manager->statistics.lastLatency) + "us");

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

void Game::print(const std::string& message)
{
	consoleMutex.lock();
	
	std::cout << message << std::endl;

	consoleMutex.unlock();
}

sf::Vector2f Game::playerPosition() const
{
	return mPlayer.getPosition();
}


void Game::sendPlayerPosition()
{
	const sf::Vector2f position = playerPosition();

	if (lastMovement != movement)
	{
		EventMovementChange* e = new EventMovementChange();
		e->playerId = 2;
		e->velX = movement.x;
		e->velY = movement.y;
		e->x = position.x;
		e->y = position.y;
		lastMovement = movement;
		EventDispatcher<EventMovementChange>::dispatchEvent(e);
	}

}

void Game::subscribe()
{
	EventDispatcher<EventMovementChange>::addSubscriber(this);
	EventDispatcher<EventLoginRequest>::addSubscriber(this);
	EventDispatcher<EventLoginResponse>::addSubscriber(this);
}
