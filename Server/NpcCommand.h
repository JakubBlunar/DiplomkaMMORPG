#ifndef NPC_COMMAND_H
#define NPC_COMMAND_H

#include <SFML/System/Time.hpp>

namespace s {
	class NpcEvent;
	struct NpcUpdateEvents;
	class Npc;
	class Server;

	class NpcCommand
	{
	public:
		NpcCommand();
		NpcCommand(sf::Time maxDuration, Server* s);
		virtual ~NpcCommand();

		sf::Time maxDuration;
		sf::Time duration;
		Server* server;
		bool finished;

		bool isFinished() const;
		virtual void update(sf::Time elapsedTime, NpcUpdateEvents* npcUpdateEvents);

		void dispatchFinishEvent(NpcEvent* e);

	protected:
		bool finishedEventDispatched;
	};
}



#endif