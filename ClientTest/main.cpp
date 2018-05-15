#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Game.h"
#include "b2GLDraw.h"
#include <iostream>

#include <mysql.h>


#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS "usbw"
#define EXAMPLE_DB "world"
#define EXAMPLE_PORT 3306

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define METTOPIX 30.f
#define PIXTOMET 0.03333333333333333f

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

int main(int argc, char** argv)
{
	const char* url = EXAMPLE_HOST;
	const char* user = EXAMPLE_USER;
	const char* pass = EXAMPLE_PASS;
	const char* database = EXAMPLE_DB;
	unsigned int port = EXAMPLE_PORT;
	char* unix_socket = nullptr;
	unsigned int flag = 0;

	MYSQL* conn = mysql_init(nullptr);

	if(!mysql_real_connect(conn, url, user,pass, database, port, unix_socket, flag ))
	{
		cout << "CANNOT CONNECT TO DATABASE" << mysql_error(conn) << endl;
		system("pause");
		return 1;
	}

	cout << "Conected" << endl;

	mysql_query(conn, "SELECT * FROM accounts;");
	auto* res = mysql_store_result(conn);
	if (res)
	{
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(res)))
		{
			int id = strtol(row[0], nullptr, 10);
			string name(row[1]);

			cout << id << name << endl;
		}
		mysql_free_result(res);
	}

	mysql_close(conn);

	//Game g;
	//g.run();

	system("pause");
	return EXIT_SUCCESS;
}

