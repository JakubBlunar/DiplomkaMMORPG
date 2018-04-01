
#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <string>

class ServerSettings
{
public:
	ServerSettings();
	~ServerSettings();
	long port;
	int max_threads;
};

#endif // !SERVER_SETTINGS_H