#include "Server.h"
#include "EventId.h"
#include <iostream>
#include "ServerSettings.h"
#include "Database.h"

#include "EventMovementChange.h"
#include "EventLoginRequest.h"
#include "ServerTasks.h"
#include <spdlog/spdlog.h>
#include "EventCharacterChoose.h"

Server::Server(ServerSettings* settings):
	running(false)
{
	serverSettings = settings;
	sessions.reserve(2000);
	tasks = new ServerTasks(this);
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

	tasks->startTasks();

	spdlog::get("log")->info("server started on port: {}", serverSettings->port);

	while (running) {
		
		//update world box2d send events

		sf::sleep(sf::seconds(1));
	}

	for (auto &thread : recieveThreads)
	{
		thread->wait();
	}

	running = false;
	Database::i()->disconnect();
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
	switch (type) {
		case MOVEMENT: {
			EventMovementChange* e = new EventMovementChange();
			if (e->loadFromPacket(packet)) {
				
				spdlog::get("log")->info("Player({}) [{}, {}]", e->playerId, e->velX, e->velY);

				sf::Packet * p = e->toPacket();

				for (unsigned int j = 0; j < sessions.size(); j++)
				{
					if (playerSession != sessions[j])
						sessions[j]->socket->send(*p);
				}
			}
			delete e;
			break;
		}
		case LOGINREQUEST: {
			EventLoginRequest* e = new EventLoginRequest();
			if (e->loadFromPacket(packet)) {
				authManager.handleEvent(e, playerSession, this);
			}
			delete e;
			break;
		}
		case LATENCY: {
			if (*packet >> id) {
				resPacket << EventId::LATENCY << id;
				playerSession->socket->send(resPacket);
			}
			break;
		}
		case CHARACTER_CHOOSE:
			{
				EventCharacterChoose* e = new EventCharacterChoose();
				if(e->loadFromPacket(packet))
				{
					authManager.handleEvent(e, playerSession, this);
				}
			}
			break;
		default:
			spdlog::get("log")->info("Cannot handle packet type {}", type);
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
					
					spdlog::get("log")->info("new connection received from {}", client->getRemoteAddress().toString());

					/*sf::sleep(sf::seconds(0.5f));
					EventLoginRequest e("kubik2405", "123456");
					sf::Packet* tempPacket = e.toPacket();
					int id;
					*tempPacket >> id;
					identifyPacket(LOGINREQUEST, tempPacket, playerSession);
					delete tempPacket;

					EventCharacterChoose ec;
					ec.characterId = 1;
					tempPacket = ec.toPacket();
					*tempPacket >> id;
					identifyPacket(CHARACTER_CHOOSE, tempPacket, playerSession);
					delete tempPacket;

					*/
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
						}
						case sf::Socket::Disconnected:
						{
							spdlog::get("log")->info("disconnect from ", playerSession->socket->getRemoteAddress().toString());

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
		selectorMutex.unlock();
	}

}
