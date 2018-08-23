#include <iostream>

#include "INIReader.h"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

#include "ServerSettings.h"
#include "Server.h"

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h" //support for stdout logging

s::ServerSettings* initSettings() {
	INIReader reader("config.ini");

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'config.ini'\n";
		return nullptr;
	}

	s::ServerSettings* settings = new s::ServerSettings();

	settings->port = reader.GetInteger("connection", "port", 55001);
	settings->max_threads = reader.GetInteger("server", "max_thread_count", 1);

	settings->dbHost = reader.Get("database", "host", "");
	settings->dbName = reader.Get("database", "database", "");
	settings->dbUser = reader.Get("database", "user", "");
	settings->dbPassword = reader.Get("database", "password", "");
	settings->dbPort = reader.GetInteger("database", "port", 3306);

	settings->logsEnabled = reader.GetBoolean("server", "logs", false);
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
	initLogger();

	s::ServerSettings* settings = initSettings();
	if (settings == nullptr) {
		return EXIT_FAILURE;
	}

	s::Server* s = new s::Server(settings);

	s->init();
	s->start();
	spdlog::drop_all();
	return EXIT_SUCCESS;
}
