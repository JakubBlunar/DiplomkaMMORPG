#include "Game.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "PacketManager.h"
#include <iostream>
#include "SceneManager.h"
#include "EventMovementChange.h"
#include "EventDispatcher.h"
#include "ResourceHolder.h"
#include "EntityPrototypes.h"


const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game():
	mStatisticsNumFrames(0) {
	
	this->packet_manager = new PacketManager(this);
	this->sceneManager = new SceneManager(this);

	mStatisticsText.setFont(ResourceHolder<sf::Font>::instance()->get("Sansation.ttf"));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(16);
	mStatisticsText.setOutlineColor(sf::Color::Magenta);
	mStatisticsText.setFillColor(sf::Color::Magenta);
}

void Game::run() {
	running = true;
	debugInfo = false;

	EntityPrototypes::instance()->init();

	window = new sf::RenderWindow(sf::VideoMode(1360, 768), "SFML Application", sf::Style::Resize | sf::Style::Close),
	window->setFramerateLimit(60);
	window->setVerticalSyncEnabled(true);
	ImGui::SFML::Init(*window, false);

	window->resetGLStates();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear(); // clear fonts if you loaded some before (even if only default one was loaded)
	io.Fonts->AddFontFromFileTTF("Data/ProggyTiny.ttf", 100);
	fonts.init();
	ImGui::SFML::UpdateFontTexture();

	recieveThread = new sf::Thread(&PacketManager::startRecieve, packet_manager);
	recieveThread->launch();

	
	gameTime.restart();
	afterStart();

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (window->isOpen() && running) {
		const sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
		}
		
		update(elapsedTime);

		ImGui::SFML::Update(*window, timeSinceLastUpdate);

		updateStatistics(elapsedTime);
		render();
	}

	ImGui::SFML::Shutdown();
}

Map* Game::getMap() const {
	return gameMap;
}

void Game::changeMap(Map* map) {
	this->gameMap = map;
}

Camera* Game::getCamera() {
	return &camera;
}


Account* Game::getAccount() const {
	return account;
}

void Game::setAccount(Account* account) {
	this->account = account;
}

void Game::addGameMessage(GameMessage* message) const {
	sceneManager->getActualScene()->addGameMessage(message);
}

sf::Time Game::getLatency() const {
	return sf::microseconds(packet_manager->statistics.lastLatency);
}

void Game::processEvents() {

	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			running = false;
			window->close();
			break;
		}

		ImGui::SFML::ProcessEvent(event);

		/*if (igManager.AnyWindowFocused())
		{
			continue;
		}*/

		switch (event.type) {
		case sf::Event::KeyPressed: {
			sf::Keyboard::Key key = event.key.code;
			if (pressedKeys.find(key) == pressedKeys.end()) {
				sceneManager->onKeyPress(key);
				pressedKeys.insert(key);
			}
			if (key == sf::Keyboard::F1) {
				debugInfo = true;
			}
			break;
		}
		case sf::Event::KeyReleased: {
			sf::Keyboard::Key key = event.key.code;
			sceneManager->onKeyRelease(key);
			pressedKeys.erase(key);
			if (key == sf::Keyboard::F1) {
				debugInfo = false;
			}
			break;
		}
		case sf::Event::MouseButtonPressed: {
			sf::Mouse::Button button = event.mouseButton.button;
			if (pressedButtons.find(button) == pressedButtons.end() && window) {
				sf::Vector2i position = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
				sceneManager->onClick(button, window->mapPixelToCoords(position));
				pressedButtons.insert(button);
			}
			break;
		}
		case sf::Event::MouseButtonReleased: {
			pressedButtons.erase(event.mouseButton.button);
			break;
		}
		case sf::Event::Resized:
			window->setView(*camera.getView());
			break;
		case sf::Event::MouseWheelScrolled:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				camera.adjustScale(event.mouseWheelScroll.delta * -0.02f, this);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
				camera.adjustRotation(event.mouseWheelScroll.delta * 10.f, this);
			}
			break;
		default: ;
		}
	}
}

void Game::update(sf::Time elapsedTime) {
	ClientSettings::instance()->eventsMutex.lock();

	sceneManager->update(elapsedTime);
	camera.update(elapsedTime, this);

	ClientSettings::instance()->eventsMutex.unlock();
}

void Game::render() {
	window->clear();

	if(window->hasFocus()) {
		sceneManager->render();
	}

	ImGui::SFML::Render(*window);

	if (debugInfo) {
		sf::Vector2f cameraOffset = camera.getOffset();
		mStatisticsText.setPosition(cameraOffset.x + 5, cameraOffset.y + 5);
		window->draw(mStatisticsText);
	}

	window->display();
}

void Game::afterStart() {}

void Game::updateStatistics(const sf::Time elapsedTime) {
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
		mStatisticsText.setString(
			"Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us\n" +
			"Latency = " + std::to_string(packet_manager->statistics.lastLatency) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

bool Game::isRunning() const {
	return running;
}

void Game::print(const std::string& message) {
	consoleMutex.lock();

	std::cout << message << std::endl;

	consoleMutex.unlock();
}
