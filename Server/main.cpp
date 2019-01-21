#include <iostream>

#include "INIReader.h"
#include "SFML/System.hpp"

#include "ServerSettings.h"
#include "Server.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h" //support for stdout logging
#include "Location.h"
#include "Lua/sol.hpp"

s::ServerSettings* initSettings() {
	INIReader reader("config.ini");

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'config.ini'\n";
		return nullptr;
	}

	s::ServerSettings* settings = new s::ServerSettings();

	settings->port = reader.GetInteger("connection", "port", 55001);
	settings->max_threads = reader.GetInteger("server", "max_thread_count", 1);
	settings->maxNpcThreads = reader.GetInteger("server", "max_npc_thread_count", 1);
	settings->maxSpellThreads = reader.GetInteger("server", "max_spell_thread_count", 1);
	settings->welcomeMessage = reader.Get("server", "welcome_message", "");
	settings->logsEnabled = reader.GetBoolean("server", "logs", false);

	settings->dbHost = reader.Get("database", "host", "");
	settings->dbName = reader.Get("database", "database", "");
	settings->dbUser = reader.Get("database", "user", "");
	settings->dbPassword = reader.Get("database", "password", "");
	settings->dbPort = reader.GetInteger("database", "port", 3306);

	settings->freeAttributesAfterLevelUp = reader.GetInteger("rates", "free_attributes_after_level_up", 5);
	settings->xpRate = (float)reader.GetReal("rates", "xp", 1);
	return settings;
}

void initLogger() {
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	console_sink->set_level(spdlog::level::trace);

	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs.log", false);
	file_sink->set_level(spdlog::level::info);


	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(console_sink);
	sinks.push_back(file_sink);
	auto logger = std::make_shared<spdlog::logger>("log", begin(sinks), end(sinks));
	logger->set_level(spdlog::level::trace);
	register_logger(logger);
}

int main() {
	srand((unsigned int)time(nullptr));
	initLogger();
	s::ServerSettings* settings = initSettings();
	
	if (settings == nullptr) {
		return EXIT_FAILURE;
	}
	/*
	s::Server* ss = new s::Server(settings);

	s::NpcManager npcManager;
	npcManager.init(ss);

	for(int i = 0; i < 10000; i++) {
		npcManager.handleEvent(new s::NpcEventNpcIsIdle());
	}

	sf::sleep(sf::seconds(60));
	system("pause");
	return 0;
	*/
	
	s::Server* s = new s::Server(settings);

	s->init();
	s->start();
	spdlog::drop_all();
	return EXIT_SUCCESS;
}
