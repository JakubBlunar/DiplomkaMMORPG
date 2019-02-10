#ifndef S_NPC_LUA_CONNECTOR_H
#define S_NPC_LUA_CONNECTOR_H
#include "Lua/sol.hpp"

namespace s {
	struct SpellInfo;
	class Npc;
	class Server;

	class NpcLuaConnector {
		Npc* npc;
		Server* server;
	public:
		NpcLuaConnector(Npc* npc);
		virtual ~NpcLuaConnector();

		void connect();

		void sendNpcToRandomPositionInLocation(float maxCommandDuration) const;
		void sendNpcToPosition(float x, float y, float maxCommandDuration) const;
		void makeNpcStay(float duration) const;
		void castRandomSpell() const;
		void castSpell(int spellType, bool onSelf) const;
		void doNothing(int ms) const;
		bool isInSpawnPosition() const;
		void goToSpawnPosition(float maxCommandDuration) const;

		bool canCastSpell(SpellInfo* spell) const;

		float getHp() const;
		float getMaxHp() const;
		float getMp() const;
	};
}
#endif
