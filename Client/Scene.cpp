#include "Scene.h"

Scene::Scene(SceneType sceneType, Game* g) {
	this->type = sceneType;
	this->game = g;
	windowManager = new IGManager();
}


Scene::~Scene() {
	delete windowManager;
}

SceneType Scene::getType() const {
	return type;
}

IGManager* Scene::getWindowManager() const {
	return windowManager;
}

void Scene::beforeChange() {
	canChange = false;
}

void Scene::afterChange() {
	canChange = true;
}

void Scene::update(sf::Time elapsedTime) {
	
	if (game->window->hasFocus()) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!isLeftClicked && game->window) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*game->window);
				onClick(sf::Mouse::Left, game->window->mapPixelToCoords(pixelPos));
			}
			isLeftClicked = true;
		} else {
			isLeftClicked = false;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			if (!isRightClicked && game->window) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*game->window);
				onClick(sf::Mouse::Right, game->window->mapPixelToCoords(pixelPos));
			}
			isRightClicked = true;
		} else {
			isRightClicked = false;
		}
	}
	

}

void Scene::render() {
	windowManager->drawAll(game);
}

void Scene::onClick(sf::Mouse::Button event, sf::Vector2f position) {
	
}
