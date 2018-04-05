#include "PacketManager.h"
#include <SFML/Network/SocketSelector.hpp>
#include <iostream>
#include "EventId.h"
#include "Game.h"

PacketManager::PacketManager(Game* g) : game(nullptr)
{
	game = g;

	if (socket.connect(g->clientSettings->host , static_cast<short>(g->clientSettings->port)) != sf::Socket::Done)
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
}


PacketManager::~PacketManager()
{
	delete latencyCheckThread;
}

bool PacketManager::isConnected() const
{
	return connected;
}

void PacketManager::latencyCheck()
{
	while (game->isRunning() && connected)
	{
		int id = rand() % INT32_MAX;

		sf::Packet packet;
		packet << EventId::LATENCY << id;
		sendPacket(&packet);
		statistics.packetSend(id);
		sf::sleep(sf::seconds(5));
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

			if (packet >> packetType)
			{
				//const auto duration = statistics.packetRecieve(id);

				EventId pt = static_cast<EventId>(packetType);
				switch (pt)
				{
				case EventId::LATENCY:
					int id;
					if (packet >> id) {
						statistics.packetRecieve(id);
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
