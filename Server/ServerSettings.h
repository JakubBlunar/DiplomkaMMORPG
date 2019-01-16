#ifndef SERVER_SETTINGS_H
#define SERVER_SETTINGS_H

#include <string>
#include <SFML/System/Mutex.hpp>

namespace s {
	class ServerSettings {
	public:
		ServerSettings();
		~ServerSettings();
		long port;
		int max_threads;
		int maxNpcThreads;
		int maxSpellThreads;
		std::string dbHost;
		std::string dbUser;
		std::string dbName;
		std::string dbPassword;
		unsigned int dbPort;
		bool logsEnabled;
		sf::Mutex eventMutex;
		std::string welcomeMessage;

		int freeAttributesAfterLevelUp;
		float xpRate;
	};
}


#endif // !SERVER_SETTINGS_H
