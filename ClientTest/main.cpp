#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Game.h"
#include "b2GLDraw.h"
#include <iostream>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

const float SCALE = 30.f;

void CreateGround(b2World& World, float X, float Y);
void CreateBox(b2World& World, int MouseX, int MouseY);

void testBox2d()
{
	sf::RenderWindow Window(sf::VideoMode(1360, 768, 32), "Test", sf::Style::Close);
	Window.setFramerateLimit(60);

	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);
	World.SetAllowSleeping(true);

	b2GLDraw debugDrawInstance;
	World.SetDebugDraw(&debugDrawInstance);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	debugDrawInstance.SetFlags(flags);

	sf::Texture GroundTexture;
	sf::Texture BoxTexture;
	GroundTexture.loadFromFile("ground.png");
	BoxTexture.loadFromFile("box.png");

	CreateGround(World, 680.f, 768.f);

	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Window.close();
				break;
			}
			int mouseX, mouseY;
			switch (event.type)
			{
			case sf::Event::MouseButtonPressed:
				std::cout << std::to_string(event.key.code) << std::endl;
				mouseX = sf::Mouse::getPosition(Window).x;
				mouseY = sf::Mouse::getPosition(Window).y;
				CreateBox(World, mouseX, mouseY);
				break;
			case sf::Event::Resized:
				Window.setView(sf::View(sf::FloatRect(0, 0, 1360, 768)));
			}
		}
	
		World.Step(1 / 60.f, 8, 3);

		Window.clear(sf::Color::White);

		int BodyCount = 0;
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(BoxTexture);
				Sprite.setOrigin(16.f, 16.f);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * RADTODEG);
				Window.draw(Sprite);
				++BodyCount;
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(680, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(BodyIterator->GetAngle() * RADTODEG);
				Window.draw(GroundSprite);
			}
		}
		World.DrawDebugData();
		Window.display();
	}
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((1360 / 2) / SCALE, (16.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}


int main(int argc, char** argv)
{
	testBox2d();
	
	Game g;
	//g.run();

	return EXIT_SUCCESS;
}

