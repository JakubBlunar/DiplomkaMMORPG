#ifndef S_NPC_H
#define S_NPC_H
#include <string>
#include <SFML/System/Vector2.hpp>
#include "Map.h"
#include "NpcCommand.h"
#include "EntityPosition.h"
#include "NpcConstants.h"
#include "Lua/sol.hpp"

namespace s {
	class Npc: public EntityPosition, public EntityAttributes {
		NpcState state;
		
		int spawnId;
		int type;

		std::string name;

		sf::Time respawnTime;	
		sf::Time deadTimestamp;

		NpcCommand* command;
		
		sf::Mutex mutex;
	public:
	
		Npc();
		~Npc();

		NpcCommand* getNpcCommand();
		void setNpcCommand(NpcCommand* command);

		void setSpawnId(int id);
		int getSpawnId() const;

		void loadFromJson(std::string file);
		Npc* clone() const;

		json toJson() const;

		void setName(std::string name);
		std::string getName() const;

		void setNpcState(NpcState state);
		NpcState getNpcState() const;

		void setMovement(float movementX, float movementY);
		void setMovement(sf::Vector2f movement, NpcUpdateEvents * npcUpdateEvents);
		sf::Vector2f getMovement() const;

		void setType(int type);
		int getType() const;

		sf::Time getRespawnTime() const;
		void setRespawnTime(sf::Time respawnTime);
		void setDeadTimestamp(sf::Time deadTimestamp);
		sf::Time getDeadTimestamp() const;
		
		sol::state* getLuaState();
		
		bool isAlive() const;

		void setMovementDirection(sf::Vector2f direction, float speed, NpcUpdateEvents * npcUpdateEvents);

		sol::state luaState;
		std::string npc_script;

		void lock();
		void unlock();
	};
}

#endif