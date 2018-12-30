#include "Scene.h"

Scene::Scene(SceneType sceneType, Game* g) {
	this->type = sceneType;
	this->game = g;
	windowManager = new IGManager();
	canChange = true;
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

}

void Scene::render() {
	windowManager->drawAll(game);
}

void Scene::onClick(sf::Mouse::Button event, sf::Vector2f position) {
	
}

void Scene::onKeyPress(sf::Keyboard::Key key) {}

void Scene::onKeyRelease(sf::Keyboard::Key key)
{
}

