#include "BotGame.h"

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

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

#include "EventCharacterLogout.h"
#include "EventDispatcher.h"


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
	sf::Time botTimePerFrame = sf::seconds(1.f / 20.f);
	sf::Time sleepTime = sf::seconds(botTimePerFrame.asSeconds() / 3);

	while (running) {
		const sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > botTimePerFrame) {
			timeSinceLastUpdate -= botTimePerFrame;
			sf::sleep(sleepTime);
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

	bool isConsoleWindowFocussed = (GetConsoleWindow() == GetForegroundWindow());
	if(isConsoleWindowFocussed && GetAsyncKeyState(VK_ESCAPE)) {
		EventCharacterLogout e;

		Player* player = account->getPlayerEntity();
		if (player) {
			e.characterId = player->getId();

			sf::Packet* p = e.toPacket();
			packet_manager->sendPacket(p);
			delete p;
		}
		
		running = false;
	}

	ClientSettings::instance()->eventsMutex.unlock();
}
void BotGame::render() {}

void BotGame::afterStart() {
	
	print("BOT INITED");

	EventLoginRequest* req = new EventLoginRequest("bot", "");
	EventDispatcher<EventLoginRequest>::dispatchEvent(req);

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
