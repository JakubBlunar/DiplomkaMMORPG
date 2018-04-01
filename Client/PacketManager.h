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
	sf::Mutex consoleMutex;

	void startRecieve();
	void sendPacket(sf::Packet* packet, int id);

	bool isConnected() const;
	int getId() const;
	PMStatistics statistics;
	Game* game;
private:
	int id;
	bool connected;
	bool watchStatistics;
};

#endif
