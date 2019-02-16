#include "LoginScene.h"
#include "IGLoginCredentials.h"

#include "SFML/Graphics.hpp"
#include "BotGame.h"
#include "EventLoginResponse.h"
#include "SceneManager.h"
#include "EventCharacterChoose.h"
#include "EventDispatcher.h"


LoginScene::LoginScene(SceneType sceneType, Game* g): Scene(sceneType, g) {
	IGLoginCredentials* credWindow = new IGLoginCredentials();

	background.load("background.png", sf::Vector2i(1360, 768), sf::Vector2i(0,0));
	background.setPosition(0,0);
	windowManager->addWindow("loginCredentials", credWindow);
	windowManager->openAll();
	EventDispatcher<EventLoginResponse>::addSubscriber(this);
}

LoginScene::~LoginScene() {}

void LoginScene::beforeChange() {
	Scene::beforeChange();
}

void LoginScene::afterChange() {
	Scene::afterChange();
}

void LoginScene::update(sf::Time elapsedTime) {
	Scene::update(elapsedTime);
}

void LoginScene::render() {
	game->window->draw(background);

	Scene::render();
}

void LoginScene::onKeyPress(sf::Keyboard::Key key) {
	switch (key) {
		case sf::Keyboard::Escape:
			game->running = false;
			break;
		default: break;
	}
}

void LoginScene::handleEvent(GameEvent* event) {
	switch (event->getId()) {
		case LOGINRESPONSE: {
			EventLoginResponse* e = (EventLoginResponse*)event;
			//game->print(e->toString());
			if (e->status) {
				json jsonData = json::parse(e->account);
				Account* account = new Account();
				account->initFromJson(jsonData);
				game->setAccount(account);

				game->sceneManager->changeScene(SceneType::CHARACTER_CHOOSE);

				if (dynamic_cast<BotGame*>(game) != nullptr) {
					EventCharacterChoose* eventCharacterChoose = new EventCharacterChoose();
					eventCharacterChoose->characterId = 1;
					sf::Packet* packet = eventCharacterChoose->toPacket();
					game->packet_manager->sendPacket(packet);
					delete packet;
				}
			}
			else {
				IGManager* manager = game->sceneManager->getActualScene()->getWindowManager();
				if (manager->getActualPopup()) {
					manager->getActualPopup()->close();
				}
				IGPopup* popup = new IGPopup("Error", e->message, sf::Vector2f(550, 180), "Ok");
				manager->pushPopup(popup);
			}
		}


	}

}
