#include "GamePlayScene.h"
#include "IGGameMenu.h"
#include "Camera.h"
#include "Globals.h"
#include "Box2D/Box2D.h"
#include "ResourceHolder.h"
#include "VisibleObjectsCast.h"
#include "Globals.h"
#include <iostream>

GamePlayScene::GamePlayScene(SceneType sceneType) : Scene(sceneType)
{
	escPressed = false;
	fonePressed = false;
	drawDebugData = true;

	windowManager->addWindow("GameMenu", new IGGameMenu());

	BoxTexture.loadFromFile("box.png");
	BoxTexture.setSmooth(true);
	mFont = ResourceHolder<sf::Font>::instance()->get("Sansation.ttf");

	nameOfScene.setString("Game play screen");
	nameOfScene.setFont(mFont);
	nameOfScene.setPosition(50, 50);
	nameOfScene.setCharacterSize(20);
	nameOfScene.setFillColor(sf::Color::Black);

	Sprite.setTexture(BoxTexture);
	Sprite.setTextureRect(sf::IntRect(0,0,32,32));
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
				windowManager->close("GameMenu");
			else
				windowManager->Open("GameMenu");
		}	
		escPressed = true;
	}else
		escPressed = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1))
	{
		if(!fonePressed)
			drawDebugData = !drawDebugData;
		fonePressed = true;
	}else
		fonePressed = false;
}

void GamePlayScene::render(Game * g)
{
	g->window.clear(sf::Color(220, 220, 220));

	Map* map = g->getMap();

	int width = map->getWidth();
	int height = map->getHeight();

	sf::Vector2f offset = g->getCamera()->getOffset();
	sf::Vector2f resolution = g->getCamera()->getResolution();

	int fromX = (int) (offset.x / FIELD_SIZE) - 1;
	int toX = (int) (fromX + (resolution.x / FIELD_SIZE) + 2);

	int fromY = (int) (offset.y / FIELD_SIZE) - 1;
	int toY = (int) (fromY + (resolution.y / FIELD_SIZE) + 2);
	
	if(fromX < 0)
		fromX = 0;

	if (toX > width)
		toX = width;

	if(fromY < 0)
		fromY = 0;

	if (toY > height)
		toY = height;

	for(int i = fromX; i < toX; i++)
	{
		for(int j = fromY; j < toY; j++)
		{
			Field* field = map->getField(i, j);
			std::vector<RenderSprite*>* layers = field->getLayers();

			auto layerSize = layers->size();
			for(unsigned int k = 0; k < layerSize; k++)
			{
				RenderSprite* layer = layers->at(k);
				layer->setPosition(i*FIELD_SIZE + 16, j*FIELD_SIZE + 16);
				g->window.draw(*layer);
			}
		}
	}

	b2World * w = map->getB2World();

	aabb.lowerBound = b2Vec2(offset.x * PIXTOMET, offset.y * PIXTOMET);
	aabb.upperBound = b2Vec2((offset.x + resolution.x) * PIXTOMET, (offset.y + resolution.y) * PIXTOMET);
	queryCallback.foundBodies.clear();
	w->QueryAABB( &queryCallback, aabb );
	
	queryCallback.sortBodies();

	auto bodySize = queryCallback.foundBodies.size();
	for (unsigned int i = 0; i < bodySize; i++) {
		Entity* entity = (Entity*) queryCallback.foundBodies[i]->GetUserData();
		RenderComponent* renderComponent = (RenderComponent*)entity->getComponent(ComponentType::RENDER);
		PositionComponent* positionComponent = (PositionComponent*)entity->getComponent(ComponentType::POSITION);
		if (renderComponent && positionComponent)
		{
			AnimatedSprite* sprite = renderComponent->getCurrentAnimation();

			b2Vec2 position = queryCallback.foundBodies[i]->GetPosition();
			sf::Vector2i renderOffset = renderComponent->getOffset();
			sprite->setPosition(ceilNumber(METTOPIX * position.x + renderOffset.x), ceilNumber(METTOPIX * position.y + renderOffset.y));

			g->window.draw(*sprite);
		}else
		{
			if(entity->getType() == EntityType::PLAYER)
			{
				b2Vec2 position = queryCallback.foundBodies[i]->GetPosition();
				Sprite.setPosition(ceilNumber(METTOPIX * position.x), ceilNumber(METTOPIX * position.y));
				//std::cout << Sprite.getPosition().x << "-" <<Sprite.getPosition().y << std::endl; 
				g->window.draw(Sprite);
			}
			
		}
	}

	g->window.draw(nameOfScene);
	
	if(drawDebugData)
		w->DrawDebugData();

	Scene::render(g);
}
