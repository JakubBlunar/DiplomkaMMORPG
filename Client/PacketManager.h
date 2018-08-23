#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include <SFML/Network.hpp>
#include "PMStatistics.h"

class Game;

class PacketManager {
public:
	PacketManager(Game* g);
	~PacketManager();

	sf::TcpSocket socket;
	sf::Mutex socketMutex;

	void startRecieve();
	void sendPacket(sf::Packet* packet);

	bool isConnected() const;

	PMStatistics statistics;
	Game* game;
private:
	void latencyCheck();
	sf::Thread* latencyCheckThread;

	bool connected;
};

#endif
