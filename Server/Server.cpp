#include "Server.h"
#include "EventId.h"
#include <iostream>
#include "ServerSettings.h"
#include "Database.h"

#include "EventMovementChange.h"
#include "EventLoginRequest.h"

Server::Server(ServerSettings* settings)
{
	serverSettings = settings;
	sessions.reserve(2000);
}


Server::~Server()
{
	delete serverSettings;
}

void Server::init()
{
	Database::initDatabase(serverSettings);

	sessions.clear();
	running = false;

	errorPacket << EventId::ERR << "Error happend";
	wrongType << EventId::WRONGPACKETTYPE;
	
	for (int i = 0; i < serverSettings->max_threads; i++)
	{
		sf::Thread* t = new sf::Thread(&Server::recievePackets, this);
		recieveThreads.push_back(t);
	}

	managers.push_back(&authManager);
}

void Server::start()
{
	running = true;
	listener.listen(static_cast<short>(serverSettings->port));
	selector.add(listener);


	for (unsigned int i = 0; i < recieveThreads.size(); i++)
	{
		recieveThreads[i]->launch();
	}

	print("server started on port " + std::to_string(serverSettings->port));

	while (running) {
		
		//update world box2d send events


	}

	for (auto &thread : recieveThreads)
	{
		thread->wait();
	}

	running = false;
}

void Server::update()
{

}

void Server::print(const std::string& message)
{
	consoleMutex.lock();

	std::cout << message << std::endl;

	consoleMutex.unlock();
}

void Server::identifyPacket(EventId type, sf::Packet *packet, Session* playerSession) {
	sf::Packet resPacket;
	int id;

	if (type == MOVEMENT) {
		EventMovementChange* e = new EventMovementChange();
		if (e->loadFromPacket(packet)) {
			
			print("Player(" + to_string(e->playerId) +") [" + to_string(e->velX) + "," + to_string(e->velY) + "]");

			sf::Packet * p = e->toPacket();

			for (unsigned int j = 0; j < sessions.size(); j++)
			{
				if (playerSession != sessions[j])
					sessions[j]->socket->send(*p);
			}
		}
		delete e;
	}

	if (type == LOGINREQUEST) {
		EventLoginRequest* e = new EventLoginRequest();
		if (e->loadFromPacket(packet)) {
			authManager.handleEvent(e, playerSession, this);
		}
		delete e;
	}

	if (type == LATENCY) {
		if (*packet >> id) {
			resPacket << EventId::LATENCY << id;
			playerSession->socket->send(resPacket);
		}
	}
}

void Server::recievePackets()
{
	while (running)
	{
		selectorMutex.lock();
		if (selector.wait())
		{
			if (selector.isReady(listener))
			{
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done)
				{

					Session* playerSession = new Session();
					playerSession->socket = client;

					sf::Packet packet;
					sessions.push_back(playerSession);

					selector.add(*client);
					
					print("new connection received from " + client->getRemoteAddress().toString());
				}
				else
				{
					print("error : server has no connection");
					delete client;
				}
			}
			else
			{
				for (unsigned int i = 0; i < sessions.size(); i++)
				{
					Session* playerSession = sessions[i];

					if (!selector.isReady(*playerSession->socket))
					{
						continue;
					}

					sf::Packet packet;
					switch (playerSession->socket->receive(packet))
					{
					case sf::Socket::Done:
					{
						int type;

						if (packet >> type)
						{
							EventId pt = static_cast<EventId>(type);
							identifyPacket(pt, &packet, playerSession);
						}
						break;
					case sf::Socket::Disconnected:
					{
						print("disconnect from " + playerSession->socket->getRemoteAddress().toString());

						selector.remove(*playerSession->socket);
						playerSession->socket->disconnect();

						sessions.erase(sessions.begin() + i);
						i--;
						break;
					}
					default:
						break;
					}
					}
				}
				
			}
		}
		selectorMutex.unlock();
	}

}
