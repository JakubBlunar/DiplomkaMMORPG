#ifndef  DATABASE_H
#define DATABASE_H
#include "SFML\System.hpp"
#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\statement.h>


class ServerSettings;

class Database
{
	static Database *s_instance;
	static sf::Mutex dbMutex;
	Database(ServerSettings* settings);
	
	sql::Driver* driver;
	sql::Connection *con;
	sql::Statement *stmt;
public:
	static Database *i()
	{
		return s_instance;
	}

	static void initDatabase(ServerSettings * settings)
	{
		if (s_instance) {
			return;
		}
		s_instance = new Database(settings);
	}
};

#endif // ! DATABASE_H
