#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <string>
#include <SFML/System/Mutex.hpp>

using namespace std;

namespace s {
	class ServerSettings {
	public:
		ServerSettings();
		~ServerSettings();
		long port;
		int max_threads;
		int maxNpcThreads;
		string dbHost;
		string dbUser;
		string dbName;
		string dbPassword;
		unsigned int dbPort;
		bool logsEnabled;
		sf::Mutex eventMutex;
	};
}


#endif // !SERVER_SETTINGS_H
