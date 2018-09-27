#ifndef S_NPC_HOLDER_H
#define S_NPC_HOLDER_H

#include "json.hpp"
#include "Npc.h"
using json = nlohmann::json;

namespace s {
	class NpcHolder {
	public:
		void init();

		NpcHolder(const NpcHolder&) = delete;
		NpcHolder& operator=(const NpcHolder) = delete;

		static NpcHolder* instance() {
			static NpcHolder instance;
			return &instance;
		}

		void read_directory(std::string pattern, std::vector<std::string>& v) const;
	private:
		NpcHolder();
		~NpcHolder();

		std::map<int, Npc*> npcPrototypes;
	};
}


#endif
