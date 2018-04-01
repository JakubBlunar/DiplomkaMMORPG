
#ifndef CLIENT_SETTINGS_H
#define CLIENT_SETTINGS_H

#include <string>

class ClientSettings
{
public:
	ClientSettings();
	~ClientSettings();
	long port;
	std::string host;
};

#endif // !CLIENT_SETTINGS_H