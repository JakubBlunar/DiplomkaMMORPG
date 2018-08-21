#ifndef  DATABASE_H
#define DATABASE_H
#include "SFML\System.hpp"
#include <mysql.h>

namespace s {
	class ServerSettings;
}

namespace s
{
	class Database
	{
	private:
		static Database *s_instance;
		Database(s::ServerSettings* settings);
		sf::Mutex dbMutex;
		MYSQL* conn;
	public:
		static Database *i()
		{
			return s_instance;
		}

		static MYSQL* getConnection()
		{
			return s_instance->conn;
		}

		static std::string escapeString(std::string value)
		{
			const char *pStr = value.c_str();
			char *tStr = new char[strlen(pStr)*2+1];
		    mysql_real_escape_string(s_instance->conn, tStr, value.c_str(), strlen(pStr));
			std::string retStr(tStr);
		    delete [] tStr;
		    return retStr;
		}

		static void initDatabase(s::ServerSettings * settings)
		{
			if (s_instance) {
				return;
			}
			s_instance = new Database(settings);
		}

		MYSQL_RES* executeQuery(std::string query);
		int executeModify(std::string query);
		void disconnect();



	};
}


#endif // ! DATABASE_H
