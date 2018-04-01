#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "Session.h"

#include "SFML\Network.hpp"
#include "ServerSettings.h"

class Server
{
public:
	Server(ServerSettings* settings);
	~Server();
	void init();
	void start();
private:
	ServerSettings* serverSettings;
	void update();
	std::vector<Session*> sessions;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::Thread*> recieveThreads;

	bool running;

	sf::Mutex consoleMutex;
	sf::Mutex selectorMutex;

	void recievePackets();

	sf::Packet errorPacket;
	sf::Packet wrongType;
};
#endif // !SERVER_H
