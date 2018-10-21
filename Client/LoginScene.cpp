#include "LoginScene.h"
#include "IGLoginCredentials.h"

#include "SFML/Graphics.hpp"

LoginScene::LoginScene(SceneType sceneType, Game* g): Scene(sceneType, g) {
	IGLoginCredentials* credWindow = new IGLoginCredentials();

	windowManager->addWindow("loginCredentials", credWindow);
	windowManager->OpenAll();
}

LoginScene::~LoginScene() {

}

void LoginScene::beforeChange() {
	Scene::beforeChange();
}

void LoginScene::afterChange() {
	Scene::afterChange();
}

void LoginScene::update(sf::Time elapsedTime) {
	Scene::update(elapsedTime);

	if (game->keyboardManager->isKeyPressed(sf::Keyboard::Escape)) {
		game->running = false;
	}
}

void LoginScene::render() {
	game->window->clear(sf::Color::Green);

	Scene::render();
}
