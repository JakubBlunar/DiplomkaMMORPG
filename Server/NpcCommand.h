#ifndef NPC_COMMAND_H
#define NPC_COMMAND_H

#include <SFML/System/Time.hpp>

namespace s {
	struct NpcUpdateEvents;
	class Npc;
	class Server;

	class NpcCommand
	{
	public:
		NpcCommand();
		NpcCommand(sf::Time maxDuration);
		virtual ~NpcCommand();

		sf::Time maxDuration;
		sf::Time duration;

		bool finished;

		bool isFinished() const;
		virtual void update(sf::Time elapsedTime, NpcUpdateEvents* npcUpdateEvents);
	};
}



#endif