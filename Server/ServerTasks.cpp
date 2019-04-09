#include "ServerTasks.h"
#include "Database.h"
#include "Character.h"
#include "Session.h"
#include "Account.h"
#include <spdlog/spdlog.h>

s::ServerTasks::ServerTasks(Server* server) :
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

	Database* database = Database::i();

	spdlog::get("log")->info("Updating online players for web and server status");
	database->executeQuery("DELETE from onlineplayers;");

	std::string query = "INSERT INTO realmStatuses (id, realmName, startTime, endTime, onlineCount, lightFactionOnline, darkFactionOnline, createdAt, updatedAt) VALUES(1, ";
	query.append("'Akalitasia realm', NOW(), NOW() - 20, 0, 0, 0, NOW(), NOW() ) ");
	query.append("ON DUPLICATE KEY UPDATE startTime=NOW(), onlineCount=0, lightFactionOnline=0, darkFactionOnline=0, updatedAt=NOW();");
	database->executeQuery(query);

	sleep(sf::seconds(60));

	spdlog::get("log")->info("Updating online players and status done");

	while (isRunning) {
		spdlog::get("log")->info("Updating online players for web and server status");

		database->executeQuery("DELETE from onlineplayers;");

		int totalCount = 0;
		int lightFactionCount = 0;
		int darkFactionCount = 0;

		for_each(server->sessions.begin(), server->sessions.end(), [&darkFactionCount, &lightFactionCount, &totalCount, database](Session* session) {
			Account* account = session->getAccount();
			if (account && !account->isBot) {
				Character* ch = account->getCharacter();
				if (ch) {
					if (ch->faction == CharacterFaction::DARK) {
						darkFactionCount++;
					}
					if (ch->faction == CharacterFaction::LIGHT) {
						lightFactionCount++;
					}
					totalCount++;

					std::string query = "INSERT INTO onlinePlayers(`id`, `createdAt`, `updatedAt`, `accountId`, `characterId`) VALUES(";
					query.append(Database::escapeString(std::to_string(ch->id) + ", "));
					query.append(Database::escapeString("NOW(), NOW(), "));
					query.append(Database::escapeString(std::to_string(account->id) + ", "));
					query.append(Database::escapeString(std::to_string(ch->id)));
					query.append(");");

					database->executeQuery(query);
				}

			}
		});

		query = "UPDATE realmStatuses SET onlineCount=" + std::to_string(totalCount) + ", lightFactionOnline=" + std::to_string(lightFactionCount) + ", darkFactionOnline=" + std::to_string(darkFactionCount) + ", updatedAt=NOW();";
		database->executeQuery(query);

		spdlog::get("log")->info("Updating online players and status done");
		sf::sleep(sf::seconds(60));
	}
}

void s::ServerTasks::saveCharacters() {
	sf::sleep(sf::seconds(60));

	while (isRunning) {
		spdlog::get("log")->info("Executing save characters worker");
		for_each(server->sessions.begin(), server->sessions.end(), [](Session* session) {
			Account* account = session->getAccount();
			if (account && !account->isBot) {
				Character* ch = account->getCharacter();
				if (ch) {
					ch->save();
				}

			}
		});
		spdlog::get("log")->info("Saving characters done");

		sleep(sf::seconds(120));
	}
}
