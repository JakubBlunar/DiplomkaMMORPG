#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include <SFML/Network.hpp>
#include "PMStatistics.h"

class Game;

class PacketManager
{
public:
	PacketManager(Game* g, bool statistics);
	~PacketManager();

	sf::TcpSocket socket;
	sf::Mutex socketMutex;

	void startRecieve();
	void sendPacket(sf::Packet* packet);

	bool isConnected() const;
	int getId() const;
	PMStatistics statistics;
	Game* game;
private:
	void latencyCheck();
	sf::Thread* latencyCheckThread;

	int id;
	bool connected;
	bool watchStatistics;
};

#endif
