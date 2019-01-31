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
#include "Account.h"
#include "Character.h"
#include "Map.h"
#include "EventCharacterLogout.h"
#include "NpcHolder.h"
#include "SpellHolder.h"
#include "EventPlayerStartCastSpell.h"
#include "EventIncreaseCharacterAttribute.h"

s::Server::Server(ServerSettings* settings):
	running(false) {
	serverSettings = settings;
	sessions.reserve(2000);
	tasks = new s::ServerTasks(this);
}


s::Server::~Server() {
	delete serverSettings;
}

void s::Server::init() {
	Database::initDatabase(serverSettings);

	sessions.clear();
	running = false;

	errorPacket << EventId::ERR << "Error happend";
	wrongType << EventId::WRONGPACKETTYPE;

	for (int i = 0; i < serverSettings->max_threads; i++) {
		sf::Thread* t = new sf::Thread(&Server::recievePackets, this);
		recieveThreads.push_back(t);
	}

	SpellHolder::instance()->init(this);
	NpcHolder::instance()->init();
	
	npcManager.init(this);
	mapsManager.init(this);
	spellManager.init(this);
	characterManager.init(this);

	managers.push_back(&mapsManager);
	managers.push_back(&spellManager);
}

void s::Server::start() {
	running = true;
	serverTime.restart();

	listener.listen(static_cast<short>(serverSettings->port));
	selector.add(listener);

	for (unsigned int i = 0; i < recieveThreads.size(); i++) {
		recieveThreads[i]->launch();
	}

	tasks->startTasks();

	spdlog::get("log")->info("server started on port: {}", serverSettings->port);


	sf::Clock clock;
	while (running) {
		const sf::Time elapsedTime = clock.restart();

		bool isConsoleWindowFocussed = (GetConsoleWindow() == GetForegroundWindow());

		update(elapsedTime);

		if (isConsoleWindowFocussed && GetAsyncKeyState(VK_ESCAPE)) {
			running = false;
		}
	}

	spdlog::get("log")->info("SERVER IS TURNING OFF WAITING FOR WORKERS TO END");
	tasks->finish();

	for (auto& thread : recieveThreads) {
		thread->wait();
	}

	running = false;

	Database* database = Database::i();
	database->executeQuery("UPDATE realmStatuses SET onlineCount=0, lightFactionOnline=0, darkFactionOnline=0, updatedAt=NOW(), endTime=NOW() WHERE id = 1;");
	database->executeQuery("DELETE from onlineplayers;");
	database->disconnect();
}

sf::Time s::Server::getServerTime() const {
	return serverTime.getElapsedTime();
}

void s::Server::update(sf::Time elapsedTime) {
	Server* s = this;
	std::for_each(managers.begin(), managers.end(), [elapsedTime, s](Manager* m) {
		m->update(elapsedTime, s);
	});
}

void s::Server::print(const std::string& message) {
	consoleMutex.lock();

	std::cout << message << std::endl;

	consoleMutex.unlock();
}

void s::Server::identifyPacket(EventId type, sf::Packet* packet, Session* playerSession) {
	sf::Packet resPacket;
	int id;
	switch (type) {
	case MOVEMENT: {
		EventMovementChange* e = new EventMovementChange();
		if (e->loadFromPacket(packet)) {
			Map* map = playerSession->getAccount()->getCharacter()->position.getMap();
			map->handleEvent(e, playerSession, this);
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
			playerSession->sendPacket(&resPacket);
		}
		break;
	}
	case CHARACTER_CHOOSE: {
		EventCharacterChoose* e = new EventCharacterChoose();
		if (e->loadFromPacket(packet)) {
			authManager.handleEvent(e, playerSession, this);
		}
		delete e;
	}
	break;
	case CHARACTER_LOGOUT: {
		EventCharacterLogout* e = new EventCharacterLogout();
		if(e->loadFromPacket(packet)) {
			authManager.handleEvent(e, playerSession, this);
		}
		delete e;
		break;
	}
	case PLAYER_START_CAST_SPELL: {
		EventPlayerStartCastSpell* e = new EventPlayerStartCastSpell();
		if (e->loadFromPacket(packet)) {
			spellManager.handleEvent(e, playerSession, this);	
		}
		delete e;
		break;
	}
	case SEND_MESSAGE: {
		EventSendMessage* e = new EventSendMessage();
		if (e->loadFromPacket(packet)) {
			chatManager.handleEvent(e, playerSession, this);	
		}
		delete e;
		break;
	}
	case INCREASE_CHARACTER_ATTRIBUTE: {
		EventIncreaseCharacterAttribute* e = new EventIncreaseCharacterAttribute();
		if (e->loadFromPacket(packet)) {
			characterManager.handleEvent(e, playerSession, this);
		}
		delete e;
		break;
	}
	default:
		spdlog::get("log")->info("Cannot handle packet type {}", type);
	}
}

void s::Server::recievePackets() {
	while (running) {
		selectorMutex.lock();
		if (selector.wait()) {
			if (selector.isReady(listener)) {
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done) {

					Session* playerSession = new Session();
					playerSession->setSocket(client);

					sf::Packet packet;
					sessions.push_back(playerSession);

					selector.add(*client);

					spdlog::get("log")->info("new connection received from {}", client->getRemoteAddress().toString());


					std::string name;
					std::string pass;
					int characterId;
					if(sessions.size() <= 2) {
						if (sessions.size() == 1) {
							name = "kubik2405";
							pass = "123456";
							characterId = 2;
						}else {
							name = "admin";
							pass = "123456";
							characterId = 3;
						}

						sf::sleep(sf::seconds(0.5f));
						EventLoginRequest e(name, pass);
						sf::Packet* tempPacket = e.toPacket();
						int id;
						*tempPacket >> id;
						identifyPacket(LOGINREQUEST, tempPacket, playerSession);
						delete tempPacket;

						EventCharacterChoose ec;
						ec.characterId = characterId;
						tempPacket = ec.toPacket();
						*tempPacket >> id;
						identifyPacket(CHARACTER_CHOOSE, tempPacket, playerSession);
						delete tempPacket;
					}
					
				}
				else {
					print("error : server has no connection");
					delete client;
				}
			}
			else {
				for (unsigned int i = 0; i < sessions.size(); i++) {
					Session* playerSession = sessions[i];

					if (!selector.isReady(*playerSession->getSocket())) {
						continue;
					}

					sf::Packet packet;
					switch (playerSession->getSocket()->receive(packet)) {
					case sf::Socket::Done: {
						int type;

						if (packet >> type) {
							EventId pt = static_cast<EventId>(type);
							identifyPacket(pt, &packet, playerSession);
						}
						break;
					}
					case sf::Socket::Disconnected: {
						spdlog::get("log")->info("disconnect from ", playerSession->getSocket()->getRemoteAddress().toString());

						Account* a = playerSession->getAccount();
						if (a) {
							Character* ch = a->getCharacter();
							ch->lock();
							if (ch) {
								if(!a->isBot) {
									ch->save();
								}
								ch->position.getMap()->removeCharacter(ch);
							}
							if (a->isBot) {
								botManager.destroyBotAccount(a);
							} else {
								delete a;
							}
							ch->unlock();
						}

						selector.remove(*playerSession->getSocket());
						playerSession->getSocket()->disconnect();

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
