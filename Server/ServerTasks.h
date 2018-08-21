#ifndef TASKS_H
#define TASKS_H

#include <SFML/System/Thread.hpp>
#include "Server.h"

namespace s
{
	class ServerTasks
	{
	public:
		ServerTasks(Server* server);
		~ServerTasks();
		void startTasks();
		void finish();

		bool isRunning;
		Server* server;

		void fetchOnline();
	    sf::Thread fetchOnlineThread;
	};
}


#endif