#include "Database.h"
#include "ServerSettings.h"
#include <iostream>
#include <spdlog/spdlog.h>

Database *Database::s_instance = 0;

Database::Database(ServerSettings* settings):
	conn(mysql_init(nullptr))
{

	const char* url = settings->dbHost.c_str();
	const char* user = settings->dbUser.c_str();
	const char* pass = settings->dbPassword.c_str();
	const char* database = settings->dbName.c_str();
	unsigned int port = settings->dbPort;
	char* unix_socket = nullptr;
	unsigned int flag = 0;

	if(!mysql_real_connect(conn, url, user,pass, database, port, unix_socket, flag ))
	{
		spdlog::get("log")->error("CANNOT CONNECT TO DATABASE {}", mysql_error(conn));
		exit(EXIT_FAILURE);
	}

	cout << "Connected to database " << endl;
	cout << "Host: " << settings->dbHost << endl;
	cout << "User: " << settings->dbUser << endl;
	cout << "Db: " << settings->dbName << endl;
	cout << "Port: " << settings->dbPort << endl << endl;
}

MYSQL_RES* Database::executeQuery(std::string query)
{
	dbMutex.lock();
	spdlog::get("log")->info("EXECUTING QUERY: {}", query);
	if (mysql_query(conn, query.c_str()))
	{
		dbMutex.unlock();
		spdlog::get("log")->error("QUERY: {} ERR: {}", query, mysql_error(conn));
		return nullptr;
	}

	dbMutex.unlock();
	return mysql_store_result(conn);
}

int Database::executeModify(std::string query)
{
	dbMutex.lock();
	spdlog::get("log")->info("EXECUTING QUERY: {}", query);
	if (mysql_query(conn, query.c_str()))
	{
		dbMutex.unlock();
		spdlog::get("log")->error("QUERY: {} ERR: {}", query, mysql_error(conn));
		return 0;
	}

	dbMutex.unlock();
	return mysql_affected_rows(conn);
}

void Database::disconnect()
{
	dbMutex.lock();
	mysql_close(conn);
	dbMutex.unlock();
}

