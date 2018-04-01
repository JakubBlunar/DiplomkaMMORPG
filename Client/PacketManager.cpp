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
		game->print("An error ocurred connecting to server");
		connected = false;
	}
	else
	{
		connected = true;
		game->print("connected");
	}
	
	socket.setBlocking(false);
	watchStatistics = st;
}


PacketManager::~PacketManager()
{
	delete latencyCheckThread;
}

bool PacketManager::isConnected() const
{
	return connected;
}

int PacketManager::getId() const
{
	return id;
}

void PacketManager::latencyCheck()
{
	while (game->isRunning() && connected)
	{
		int id = rand() % INT32_MAX;

		sf::Packet packet;
		packet << pt::LATENCY << id;
		sendPacket(&packet);
		statistics.packetSend(id);
		sf::sleep(sf::seconds(2));
	}
}


void PacketManager::startRecieve()
{
	sf::SocketSelector selector;
	selector.add(socket);

	sf::Packet packet;

	latencyCheckThread = new sf::Thread(&PacketManager::latencyCheck, this);
	latencyCheckThread->launch();

	while (game->isRunning() && connected)
	{
		if (selector.wait())
		{
			socketMutex.lock();
			if (socket.receive(packet) == sf::Socket::Disconnected) {
				connected = false;
				game->print("Disconected from server");
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
				case pt::LATENCY:
					int id;
					if (packet >> id) {
						auto duration = statistics.packetRecieve(id);
						game->print(std::to_string(duration));
					}
					break;
				default:
					game->print("Unknown packet type " + std::to_string(pt));
					break;
				}
			}

		}
	}

	latencyCheckThread->wait();
}

void PacketManager::sendPacket(sf::Packet* packet)
{
	if (!connected)
		return;

	socketMutex.lock();
	socket.send(*packet);
	socketMutex.unlock();
}
