

#include <iostream>

#include "INIReader.h"
#include "SFML\System.hpp"
#include "SFML\Network.hpp"

#include "ServerSettings.h"
#include "PacketTypes.h"
#include "Server.h"

#include "Crypto.h"

ServerSettings* initSettings() {
	INIReader reader("config.ini");

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'config.ini'\n";
		return nullptr;
	}

	ServerSettings* settings = new ServerSettings();

	settings->port = reader.GetInteger("connection", "port", 55001);
	settings->max_threads = reader.GetInteger("server", "max_thread_count", 1);

	settings->dbHost = reader.Get("database", "host", "");
	settings->dbName = reader.Get("database", "database", "");
	settings->dbUser = reader.Get("database", "user", "");
	settings->dbPassword = reader.Get("database", "password", "");
	return settings;
}

int main() {
	ServerSettings* settings = initSettings();
	if (settings == nullptr) {
		system("pause");
		return EXIT_FAILURE;
	}

	Server* s = new Server(settings);

	s->init();
	s->start();

	system("pause");
	return EXIT_SUCCESS;
}

