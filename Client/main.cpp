
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
#include "Matrix.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define METTOPIX 30.f
#define PIXTOMET 0.03333333333333333f

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

