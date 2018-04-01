#include "Server.h"
#include "PacketTypes.h"
#include <iostream>
#include "ServerSettings.h"

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
	sessions.clear();
	running = false;

	errorPacket << pt::ERR << "Error happend";
	wrongType << pt::WRONGPACKETTYPE;
	
	for (int i = 0; i < serverSettings->max_threads; i++)
	{
		sf::Thread* t = new sf::Thread(&Server::recievePackets, this);
		recieveThreads.push_back(t);
	}

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

	std::cout << "server started on port " << serverSettings->port << std::endl;



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
					consoleMutex.lock();
					std::cout << "new connection received from " << client->getRemoteAddress() << std::endl;
					consoleMutex.unlock();
				}
				else
				{
					std::cout << "error : server has no connection" << std::endl;
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
							pt::PacketType pt = static_cast<pt::PacketType>(type);

							sf::Packet resPacket;

							switch (pt)
							{
							case pt::POSITION:
								float x, y, velX, velY;
								int id;
								if (packet >> x >> y >> velX >> velY)
								{
									consoleMutex.lock();
									std::cout << "Player(" << i << ")" << "[" << velX <<"," << velY <<"]" << std::endl;
									consoleMutex.unlock();
									for (unsigned int j= 0; j < sessions.size(); j++)
									{
										if (i != j)
											sessions[j]->socket->send(resPacket);
									}
								}
								break;
							default:
								playerSession->socket->send(wrongType);
								break;
							}
							break;
						}
					case sf::Socket::Disconnected:
					{
						consoleMutex.lock();
						std::cout << "disconnect from " << playerSession->socket->getRemoteAddress() << std::endl;
						consoleMutex.unlock();

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
