#include "LoginScene.h"
#include "IGLoginCredentials.h"

#include "SFML/Graphics.hpp"

LoginScene::LoginScene(SceneType sceneType): Scene(sceneType)
{
	IGLoginCredentials* credWindow = new IGLoginCredentials();

	windowManager->addWindow("loginCredentials", credWindow);
	windowManager->OpenAll();
}

LoginScene::~LoginScene()
{

}

void LoginScene::beforeChange(Game* g)
{
	Scene::beforeChange(g);
}

void LoginScene::afterChange(Game* g)
{
	Scene::afterChange(g);
}

void LoginScene::update(Game* g, sf::Time elapsedTime)
{
	Scene::update(g, elapsedTime);

	if (g->keyboardManager->isKeyPressed(sf::Keyboard::Escape)) {
		g->running = false;
	}
}

void LoginScene::render(Game * g)
{
	g->window.clear(sf::Color::Green);

	Scene::render(g);
}
