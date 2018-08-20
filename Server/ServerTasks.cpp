#include "ServerTasks.h"
#include "Database.h"
#include "../Client/Player.h"

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
	sf::sleep(sf::seconds(60));
	Database* database = Database::i();
	database->executeQuery("DELETE from onlinePlayers;");

	while (isRunning)
	{
		for_each(server->sessions.begin(), server->sessions.end(), [] (Session* session)
		{
			
		});
		sf::sleep(sf::seconds(120));
	}
}
