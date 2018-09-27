#include "BotGame.h"
#include "SceneManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "PacketManager.h"
#include "ResourceHolder.h"
#include <SFML/Graphics.hpp>
#include "ClientSettings.h"
#include "GameEvent.h"
#include "Map.h"
#include "Account.h"
#include "BotCommandMoveTo.h"
#include "Random.h"


BotGame::BotGame() {
	
}
BotGame::~BotGame() {
	
}

void BotGame::run() {
	running = true;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear(); // clear fonts if you loaded some before (even if only default one was loaded)
	io.Fonts->AddFontFromFileTTF("Data/ProggyTiny.ttf", 100);

	recieveThread = new sf::Thread(&PacketManager::startRecieve, packet_manager);
	recieveThread->launch();

	afterStart();

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (running) {
		const sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) {
			timeSinceLastUpdate -= TimePerFrame;
		}
		update(elapsedTime);
		botUpdatePlayer();
		updateStatistics(elapsedTime);
	}
}

void BotGame::update(sf::Time elapsedTime) {
	ClientSettings::instance()->eventsMutex.lock();

	sceneManager->update(this, elapsedTime);

	if (botCommand) {
		botCommand->update(elapsedTime, this);
	}

	ClientSettings::instance()->eventsMutex.unlock();
}
void BotGame::render() {}

void BotGame::afterStart() {
	
	print("BOT INITED");


}

void BotGame::botUpdatePlayer()
{
	if(!gameMap) return;

	Player* p = gameMap->getPlayer();
	if(!p) {
		return;
	}

	if(!botCommand || botCommand->isFinished()) {
		delete botCommand;
		Random* random = Random::instance();

		float x = random->randomUniformFloat(200 , 1200);
		float y = random->randomUniformFloat(200, 1200);
		sf::Vector2f position = sf::Vector2f(x, y); 

		BotCommandMoveTo* newCommand = new BotCommandMoveTo(position, p, gameMap, this,  sf::seconds(30));
		newCommand->init();

		botCommand = newCommand;
	}
}
