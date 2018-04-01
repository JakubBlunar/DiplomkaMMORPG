#include "PacketManager.h"
#include <SFML/Network/SocketSelector.hpp>
#include <iostream>
#include "PacketTypes.h"
#include "Game.h"


PacketManager::PacketManager(Game* g, const bool st) : game(nullptr), id(0)
{
	game = g;

	if (socket.connect(g->clientSettings->host , g->clientSettings->port) != sf::Socket::Done)
	{
		std::cout << "An error ocurred connecting to server" << std::endl;
		connected = false;
	}
	else
	{
		connected = true;
		std::cout << "connected" << std::endl;
	}
	
	socket.setBlocking(false);
	watchStatistics = st;
}


PacketManager::~PacketManager()
{

}

bool PacketManager::isConnected() const
{
	return connected;
}

int PacketManager::getId() const
{
	return id;
}


void PacketManager::startRecieve()
{
	sf::SocketSelector selector;
	selector.add(socket);

	sf::Packet packet;

	while (game->isRunning() && connected)
	{
		if (selector.wait())
		{
			socketMutex.lock();
			if (socket.receive(packet) == sf::Socket::Disconnected) {
				connected = false;
				std::cout << "Disconected from server" << std::endl;
				break;
			}

			socketMutex.unlock();

			int packetType;
			float x, y, velX, velY;

			if (packet >> packetType)
			{
				//const auto duration = statistics.packetRecieve(id);

				pt::PacketType pt = static_cast<pt::PacketType>(packetType);
				switch (pt)
				{
				default:
					consoleMutex.lock();
					std::cout << "I dont know recieve this packet type" << pt << std::endl;
					consoleMutex.unlock();
					break;
				}
			}

		}

	}

}

void PacketManager::sendPacket(sf::Packet* packet, const int id)
{
	if (!connected)
		return;

	socketMutex.lock();
	socket.send(*packet);

	if (watchStatistics && id != -1)
		statistics.packetSend(id);

	socketMutex.unlock();
}
