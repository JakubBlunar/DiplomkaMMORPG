#include "Database.h"
#include "ServerSettings.h"
#include <iostream>
#include <spdlog/spdlog.h>

s::Database* s::Database::s_instance = 0;

s::Database::Database(s::ServerSettings* settings):
	conn(mysql_init(nullptr)) {

	const char* url = settings->dbHost.c_str();
	const char* user = settings->dbUser.c_str();
	const char* pass = settings->dbPassword.c_str();
	const char* database = settings->dbName.c_str();
	unsigned int port = settings->dbPort;
	char* unix_socket = nullptr;
	unsigned int flag = 0;

	if (!mysql_real_connect(conn, url, user, pass, database, port, unix_socket, flag)) {
		spdlog::get("log")->error("CANNOT CONNECT TO DATABASE {}", mysql_error(conn));
		exit(EXIT_FAILURE);
	}

	std::cout << "Connected to database " << std::endl;
	std::cout << "Host: " << settings->dbHost << std::endl;
	std::cout << "User: " << settings->dbUser << std::endl;
	std::cout << "Db: " << settings->dbName << std::endl;
	std::cout << "Port: " << settings->dbPort << std::endl << std::endl;
}

MYSQL_RES* s::Database::executeQuery(std::string query) {
	dbMutex.lock();
	spdlog::get("log")->info("EXECUTING QUERY: {}", query);
	if (mysql_query(conn, query.c_str())) {
		dbMutex.unlock();
		spdlog::get("log")->error("QUERY: {} ERR: {}", query, mysql_error(conn));
		return nullptr;
	}

	dbMutex.unlock();
	return mysql_store_result(conn);
}

int s::Database::executeModify(std::string query) {
	dbMutex.lock();
	spdlog::get("log")->info("EXECUTING QUERY: {}", query);
	if (mysql_query(conn, query.c_str())) {
		dbMutex.unlock();
		spdlog::get("log")->error("QUERY: {} ERR: {}", query, mysql_error(conn));
		return 0;
	}

	dbMutex.unlock();
	return (int)mysql_affected_rows(conn);
}

void s::Database::disconnect() {
	dbMutex.lock();
	mysql_close(conn);
	dbMutex.unlock();
}
