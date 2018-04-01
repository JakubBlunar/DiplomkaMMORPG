
#include "ClientSettings.h"
#include "INIReader.h"
#include <iostream>
#include "Game.h"

ClientSettings* initSettings() {
	INIReader reader("config.ini");

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'config.ini'\n";
		return nullptr;
	}

	ClientSettings* settings = new ClientSettings();

	settings->port = reader.GetInteger("connection", "port", 55001);
	settings->host = reader.Get("connection", "host", "");

	if (settings->host == "") {
		std::cout << "Host is not specified";
		return nullptr;
	}

	return settings;
}


int main() {

	ClientSettings* settings = initSettings();
	if (settings == nullptr) {
		system("pause");
		return EXIT_FAILURE;
	}

	Game* g = new Game(settings);
	g->run();
	
	return EXIT_SUCCESS;
}