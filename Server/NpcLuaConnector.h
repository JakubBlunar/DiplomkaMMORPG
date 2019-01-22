#ifndef S_NPC_LUA_CONNECTOR_H
#define S_NPC_LUA_CONNECTOR_H
#include "Lua/sol.hpp"

namespace s {
	class Npc;

	class NpcLuaConnector {
		Npc* npc;
	public:
		NpcLuaConnector(Npc* npc);
		virtual ~NpcLuaConnector();

		void connect();

		void sendNpcToRandomPositionInLocation(float maxCommandDuration) const;
		void sendNpcToPosition(float x, float y, float maxCommandDuration) const;
		void makeNpcStay(float duration) const;
	};
}
#endif
