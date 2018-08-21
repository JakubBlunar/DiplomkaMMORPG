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
#include "ServerTasks.h"


s::Server::Server(ServerSettings* settings):
	running(false)
{
	serverSettings = settings;
	sessions.reserve(2000);
	tasks = new s::ServerTasks(this);
}


s::Server::~Server()
{
	delete serverSettings;
}

void s::Server::init()
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
	managers.push_back(&mapsManager);
}

void s::Server::start()
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


	sf::Clock clock;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (running)
	{
		const sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
		}
		update(elapsedTime);
	}

	for (auto &thread : recieveThreads)
	{
		thread->wait();
	}

	running = false;
	Database::i()->disconnect();
}

void s::Server::update(sf::Time elapsedTime)
{
	Server* s = this;
	std::for_each(managers.begin(), managers.end(), [elapsedTime, s](Manager* m)
	{
		if(m->isDynamic())
			m->update(elapsedTime, s);
	});
}

void s::Server::print(const std::string& message)
{
	consoleMutex.lock();

	std::cout << message << std::endl;

	consoleMutex.unlock();
}

void s::Server::identifyPacket(EventId type, sf::Packet *packet, Session* playerSession) {
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

void s::Server::recievePackets()
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

					sf::sleep(sf::seconds(0.5f));
					EventLoginRequest e("kubik2405", "123456");
					sf::Packet* tempPacket = e.toPacket();
					int id;
					*tempPacket >> id;
					identifyPacket(LOGINREQUEST, tempPacket, playerSession);
					delete tempPacket;

					EventCharacterChoose ec;
					ec.characterId = 2;
					tempPacket = ec.toPacket();
					*tempPacket >> id;
					identifyPacket(CHARACTER_CHOOSE, tempPacket, playerSession);
					delete tempPacket;
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
