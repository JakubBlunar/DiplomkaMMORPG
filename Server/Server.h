#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include "Session.h"

#include "SFML\Network.hpp"
#include "ServerSettings.h"
#include "EventId.h"
#include "AuthManager.h"
#include "MapsManager.h"
#include "BotManager.h"
#include "NpcManager.h"

namespace s {
	class ServerTasks;
}


namespace s {
	class Server {
	public:
		Server(ServerSettings* settings);
		~Server();
		void init();
		void start();
		void print(const std::string& message);

		ServerSettings* serverSettings;
		std::vector<Session*> sessions;

		AuthManager authManager;
		MapsManager mapsManager;
		BotManager botManager;
		NpcManager npcManager;
		vector<Manager*> managers;

		sf::Time getServerTime() const;


	private:
		void update(sf::Time elapsedTime);

		sf::TcpListener listener;
		sf::SocketSelector selector;
		std::vector<sf::Thread*> recieveThreads;

		bool running;

		sf::Mutex consoleMutex;
		sf::Mutex selectorMutex;

		void recievePackets();
		void identifyPacket(EventId type, sf::Packet* packet, Session* playerSession);

		sf::Packet errorPacket;
		sf::Packet wrongType;
		ServerTasks* tasks;

		sf::Clock serverTime;
	};
}
#endif // !SERVER_H
