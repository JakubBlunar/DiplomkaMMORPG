#include "Database.h"
#include "ServerSettings.h"

Database *Database::s_instance = 0;

Database::Database(ServerSettings* settings)
{
	try {
		driver = get_driver_instance();

		con = driver->connect(settings->dbHost.c_str(), settings->dbUser.c_str(), settings->dbPassword.c_str());
		stmt = con->createStatement();
		stmt->execute(("USE " + settings->dbName).c_str());

		/*res = stmt->executeQuery("SELECT id, login, email, lastActivity from accounts;");
		while (res->next()) {
			std::cout << res->getString("id").c_str() << res->getString("login").c_str() << "  " << res->getString("lastActivity").c_str() << std::endl;
		}*/
	}
	catch (sql::SQLException &e) {

		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLStateCStr() << " )" << std::endl;

		system("pause");
		exit(EXIT_FAILURE);
	}

}

