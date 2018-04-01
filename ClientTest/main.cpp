#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Game.h"
#include "b2GLDraw.h"
#include <iostream>

#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\statement.h>

#define EXAMPLE_HOST "tcp://127.0.0.1:3306"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS "usbw"
#define EXAMPLE_DB "world"

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
	string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

	try {
		sql::Driver* driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet  *res;

		driver = get_driver_instance();

		con = driver->connect(EXAMPLE_HOST, EXAMPLE_USER, EXAMPLE_PASS);
		stmt = con->createStatement();
		stmt->execute("USE " EXAMPLE_DB);

		res = stmt->executeQuery("SELECT id, login, email, lastActivity from accounts;");
		while (res->next()) {
			cout << res->getString("id").c_str() << res->getString("login").c_str() << "  " << res->getString("lastActivity").c_str() << endl;
		}
		delete res;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException &e) {

		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLStateCStr() << " )" << endl;

		system("pause");
		return EXIT_FAILURE;
	}

	//Game g;
	//g.run();

	system("pause");
	return EXIT_SUCCESS;
}

