#include "GamePlayScene.h"
#include "IGGameMenu.h"
#include "Camera.h"

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

	Map* map = g->getMap();

	int width = map->getWidth();
	int height = map->getHeight();

	for(int i = 0; i < map->getWidth(); i++)
	{
		for(int j = 0; j < map->getHeight(); j++)
		{
			Field* field = map->getField(i, j);
			std::vector<RenderSprite*>* layers = field->getLayers();

			for(int k = 0; k < layers->size(); k++)
			{
				RenderSprite* layer = layers->at(k);
				layer->setPosition(i*32, j*32);
				g->window.draw(*layer);
			}

		}
	}
	

	sf::Font mFont;
	mFont.loadFromFile("Data/Sansation.ttf");
	sf::Text text;
	text.setString("Game play screen");
	text.setFont(mFont);
	text.setPosition(50, 50);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	g->window.draw(text);

	g->window.setView(*g->getCamera()->getView());

	Scene::render(g);
}
