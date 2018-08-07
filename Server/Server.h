#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "Session.h"

#include "SFML\Network.hpp"
#include "ServerSettings.h"
#include "EventId.h"
#include "AuthManager.h"

class ServerTasks;

class Server
{
public:
	Server(ServerSettings* settings);
	~Server();
	void init();
	void start();
	void print(const std::string& message);

	ServerSettings* serverSettings;
	std::vector<Session*> sessions;

	AuthManager authManager;
	vector<Manager*> managers;
private:
	void update();
	
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::vector<sf::Thread*> recieveThreads;

	bool running;

	sf::Mutex consoleMutex;
	sf::Mutex selectorMutex;

	void recievePackets();
	void identifyPacket(EventId type, sf::Packet *packet, Session* playerSession);

	sf::Packet errorPacket;
	sf::Packet wrongType;
	ServerTasks* tasks;
};
#endif // !SERVER_H
