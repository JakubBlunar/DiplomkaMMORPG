#ifndef CLIENT_SETTINGS_H
#define CLIENT_SETTINGS_H

#include <string>
#include <SFML/System/Mutex.hpp>

class ClientSettings {
	static ClientSettings* p_inst;
	ClientSettings();

public:
	long port;
	std::string host;
	sf::Mutex eventsMutex;

	static ClientSettings* instance() {
		if (!p_inst) {
			p_inst = new ClientSettings();
		}
		return p_inst;
	}
};

#endif // !CLIENT_SETTINGS_H
