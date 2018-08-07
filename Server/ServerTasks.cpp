#include "ServerTasks.h"
#include "Database.h"
#include <iostream>

ServerTasks::ServerTasks(Server* server):
	fetchOnlineThread(&ServerTasks::fetchOnline, this)
{
	this->isRunning = false;
	this->server = server;
}


ServerTasks::~ServerTasks()
{
}

void ServerTasks::startTasks()
{
	isRunning = true;
	fetchOnlineThread.launch();
}

void ServerTasks::finish()
{
	this->isRunning = false;
	fetchOnlineThread.wait();
}

void ServerTasks::fetchOnline()
{
	sf::sleep(sf::seconds(10));
	Database* database = Database::i();
	database->executeQuery("DELETE from onlinePlayers;");

	while (isRunning)
	{
		
		sf::sleep(sf::seconds(120));
	}
}
