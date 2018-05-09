#include "GamePlayScene.h"
#include "IGGameMenu.h"


GamePlayScene::GamePlayScene(std::string name) : Scene(name)
{
	escPressed = false;
	windowManager->addWindow("GameMenu", new IGGameMenu());
}


GamePlayScene::~GamePlayScene()
{
}

void GamePlayScene::beforeChange(Game * g)
{
	Scene::beforeChange(g);
}

void GamePlayScene::afterChange(Game* g)
{
	Scene::afterChange(g);
}

void GamePlayScene::update(Game * g, sf::Time elapsedTime)
{
	Scene::update(g, elapsedTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		if(!escPressed)
		{
			if (windowManager->isVisible("GameMenu"))
			{
				windowManager->close("GameMenu");
			}
			else
			{
				windowManager->Open("GameMenu");
			}
		}	
		escPressed = true;
	}else
	{
		escPressed = false;
	}
}

void GamePlayScene::render(Game * g)
{
	g->window.clear(sf::Color(220, 220, 220));

	sf::Font mFont;
	mFont.loadFromFile("Data/Sansation.ttf");
	sf::Text text;
	text.setString("Game play screen");
	text.setFont(mFont);
	text.setPosition(50, 50);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	g->window.draw(text);

	Scene::render(g);
}
