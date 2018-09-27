#include "BotGame.h"
#include "../Client/SceneManager.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include "../Client/PacketManager.h"
#include <iostream>
#include "../Client/SceneManager.h"
#include "EventMovementChange.h"
#include "../Client/ClientEventActions.h"
#include "../Client/EventDispatcher.h"
#include "../Client/ResourceHolder.h"
#include <SFML/Graphics.hpp>
#include "../Client/PacketManager.h"
#include "../Client/ClientSettings.h"
#include "../Client/KeyboardManager.h"
#include <queue>
#include "GameEvent.h"
#include "../Client/ClientEventActions.h"
#include "../Client/Subscriber.h"
#include "../Client/Map.h"
#include "../Client/Camera.h"
#include "../Client/Account.h"

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

			//processEvents();
			update(elapsedTime);
			botUpdatePlayer();
		}
		updateStatistics(elapsedTime);
	}
}

void BotGame::update(sf::Time elapsedTime) {
	ClientSettings::instance()->eventsMutex.lock();

	sceneManager->update(this, elapsedTime);

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
	if(p) {
		return;
	}

}
