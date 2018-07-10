
#include "ClientSettings.h"
#include "INIReader.h"
#include <iostream>
#include "Game.h"

#include "GameEvent.h"
#include "Subscriber.h"
#include "EventDispatcher.h"
#include "EventMovementChange.h"
#include <thread>
#include "Field.h"
#include "Globals.h"


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
				Sprite.setPosition(METTOPIX * BodyIterator->GetPosition().x, METTOPIX * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * RADTODEG);
				Window.draw(Sprite);
				++BodyCount;
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(680, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * METTOPIX, BodyIterator->GetPosition().y * METTOPIX);
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
	BodyDef.position = b2Vec2(MouseX * PIXTOMET, MouseY * PIXTOMET);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) * PIXTOMET, (32.f / 2) * PIXTOMET);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X * PIXTOMET, Y * PIXTOMET);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((1360 / 2) * PIXTOMET, (16.f / 2) * PIXTOMET);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

using namespace std;

bool initSettings() {
	INIReader reader("config.ini");

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'config.ini'\n";
		return false;
	}

	ClientSettings* settings = ClientSettings::instance();

	settings->port = reader.GetInteger("connection", "port", 55001);
	settings->host = reader.Get("connection", "host", "");

	if (settings->host.empty()) {
		std::cout << "Host is not specified";
		return false;
	}

	return true;
}

class MovementChangeWriter : public Subscriber
{
	void handleEvent(GameEvent* event) override;

};

void MovementChangeWriter::handleEvent(GameEvent * event)
{	
	EventMovementChange *e = (EventMovementChange *) event;
	std::cout << "Handling event with id= "<< std::to_string(event->getId()) << " [" << e->velX << "," << e->velY << "]" << std::endl;
}

int main() {

	//testBox2d();

	//return 0;

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	if (!initSettings()) {
		system("pause");
		return EXIT_FAILURE;
	}

	Game* g = new Game();
	MovementChangeWriter c;
	EventDispatcher<EventMovementChange>::addSubscriber(&c);

	g->run();
	return EXIT_SUCCESS;
}

