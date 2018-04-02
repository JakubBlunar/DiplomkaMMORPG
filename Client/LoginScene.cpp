#include "LoginScene.h"

#include "SFML/Graphics.hpp"

LoginScene::LoginScene(std::string name): Scene(name)
{

}

LoginScene::~LoginScene()
{

}

void LoginScene::beforeChange(Game * g)
{
	Scene::beforeChange(g);
}

void LoginScene::afterChange(Game * g)
{
	Scene::afterChange(g);
}

void LoginScene::update(Game * g, sf::Time elapsedTime)
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

	

	sf::Font mFont;
	mFont.loadFromFile("Data/Sansation.ttf");
	sf::Text text;
	text.setString("Loading screen");
	text.setFont(mFont);
	text.setPosition(50, 50);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	g->window.draw(text);
}
