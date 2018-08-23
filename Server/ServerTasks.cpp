#include "ServerTasks.h"
#include "Database.h"

s::ServerTasks::ServerTasks(Server* server):
	fetchOnlineThread(&ServerTasks::fetchOnline, this) {
	this->isRunning = false;
	this->server = server;
}

s::ServerTasks::~ServerTasks() {
}

void s::ServerTasks::startTasks() {
	isRunning = true;
	fetchOnlineThread.launch();
}

void s::ServerTasks::finish() {
	this->isRunning = false;
	fetchOnlineThread.wait();
}

void s::ServerTasks::fetchOnline() {
	sf::sleep(sf::seconds(60));
	Database* database = Database::i();
	database->executeQuery("DELETE from onlinePlayers;");

	while (isRunning) {
		for_each(server->sessions.begin(), server->sessions.end(), [](Session* session) {

		});
		sf::sleep(sf::seconds(120));
	}
}
