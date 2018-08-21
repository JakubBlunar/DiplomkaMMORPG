
#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <string>

using namespace std;

namespace s
{
	class ServerSettings
	{
	public:
		ServerSettings();
		~ServerSettings();
		long port;
		int max_threads;
		string dbHost;
		string dbUser;
		string dbName;
		string dbPassword;
		unsigned int dbPort;
		bool logsEnabled;
		
	};
}


#endif // !SERVER_SETTINGS_H