#ifndef NPC_COMMAND_MOVE_TO_H
#define NPC_COMMAND_MOVE_TO_H

#include "NpcCommand.h"
#include <list>
#include <SFML/System/Vector2.hpp>

namespace s {
	class Map;

	class NpcCommandMoveTo :
		public NpcCommand
	{
		std::list<sf::Vector2f> path;
		sf::Vector2f endPosition;
		Npc* npc;
		Map* map;
		Server* server;
	public:
		NpcCommandMoveTo(sf::Vector2f position, Npc* npc, Map* m, Server *s, sf::Time maxDuration);
		~NpcCommandMoveTo() override;


		void update(sf::Time elapsedTime) override;

		void init();
	};

}

#endif