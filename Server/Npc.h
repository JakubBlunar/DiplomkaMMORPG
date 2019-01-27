#ifndef S_NPC_H
#define S_NPC_H
#include <string>
#include <SFML/System/Vector2.hpp>
#include "Map.h"
#include "NpcCommand.h"
#include "EntityPosition.h"
#include "NpcConstants.h"
#include "Lua/sol.hpp"
#include "NpcLuaConnector.h"
#include "EntityCombat.h"

namespace s {
	class Npc : public Entity {
		NpcState state;
		sf::Vector2f spawnPosition;
		int spawnId;
		int type;

		std::string name;

		sf::Time respawnTime;
		sf::Time deadTimestamp;

		NpcCommand* command;

		sf::Mutex mutex;
		NpcLuaConnector luaConnector;

		Server* server;

		NpcEvent* runningNpcEvent;
	public:
		Npc();
		~Npc();

		EntityAttributes attributes;
		EntityPosition position;
		EntitySpells spells;
		EntityCombat combat;

		void setServer(Server* s) {
			server = s;
		}

		Server* getServer() const { return server; }

		NpcCommand* getNpcCommand();
		void setNpcCommand(NpcCommand* command);

		void setSpawnId(int id);
		int getSpawnId() const;

		void loadFromJson(std::string file);
		Npc* clone();
		json toJson();

		void setName(std::string name);
		std::string getName() const;

		void setNpcState(NpcState state);
		NpcState getNpcState() const;

		void setMovement(float movementX, float movementY);
		void setMovement(sf::Vector2f movement, NpcUpdateEvents* npcUpdateEvents);
		sf::Vector2f getMovement() const;

		void setType(int type);
		int getType() const;

		sf::Time getRespawnTime() const;
		void setRespawnTime(sf::Time respawnTime);
		void setDeadTimestamp(sf::Time deadTimestamp);
		sf::Time getDeadTimestamp() const;

		bool isAlive() const;
		bool hasSpawnPosition() const;
		void setSpawnPosition(sf::Vector2f position);
		sf::Vector2f getSpawnPosition() const;

		void setMovementDirection(sf::Vector2f direction, float speed, NpcUpdateEvents* npcUpdateEvents);

		sol::state luaState;
		std::string npc_script;

		float getAttribute(EntityAttributeType attribute, bool withBonus);

		b2Body* getBody() const override;

		void startCombat(Character* character);

		bool isThinking() const {
			return runningNpcEvent != nullptr;
		}

		void setRunningNpcEvent(NpcEvent* event) {
			sf::Lock lock(mutex);
			runningNpcEvent = event;
		}

		void lock();
		void unlock();
	};
}

#endif
