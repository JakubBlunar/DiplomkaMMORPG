
#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <string>

using namespace std;

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
	bool logsEnabled;
	
};

#endif // !SERVER_SETTINGS_H