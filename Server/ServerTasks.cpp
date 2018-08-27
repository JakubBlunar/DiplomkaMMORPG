#include "ServerTasks.h"
#include "Database.h"
#include "Character.h"
#include "Session.h"
#include "Account.h"
#include <spdlog/spdlog.h>

s::ServerTasks::ServerTasks(Server* server):
	fetchOnlineThread(&ServerTasks::fetchOnline, this),
	saveCharactersThread(&ServerTasks::saveCharacters, this)
{
	this->isRunning = false;
	this->server = server;
}

s::ServerTasks::~ServerTasks() {
}

void s::ServerTasks::startTasks() {
	isRunning = true;
	fetchOnlineThread.launch();
	saveCharactersThread.launch();

}

void s::ServerTasks::finish() {
	this->isRunning = false;
	fetchOnlineThread.wait();
	saveCharactersThread.wait();
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

void s::ServerTasks::saveCharacters() {
	sf::sleep(sf::seconds(60));

	while (isRunning) {
		spdlog::get("log")->info("Executing save characters worker");
		for_each(server->sessions.begin(), server->sessions.end(), [](Session* session) {
			Account* account = session->getAccount();
			if (account) {
				Character* ch = account->getCharacter();
				if (ch) {
					ch->save();
				}

			}
		});
		spdlog::get("log")->info("Saving characters done");

		sf::sleep(sf::seconds(120));
	}
}