#ifndef  DATABASE_H
#define DATABASE_H
#include "SFML\System.hpp"
#include <mysql.h>

class ServerSettings;

class Database
{
private:
	static Database *s_instance;
	Database(ServerSettings* settings);
	sf::Mutex dbMutex;
	MYSQL* conn;
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

	MYSQL_RES* executeQuery(std::string query);
	void disconnect();



};

#endif // ! DATABASE_H
