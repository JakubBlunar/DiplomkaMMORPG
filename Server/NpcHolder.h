#ifndef S_NPC_HOLDER_H
#define S_NPC_HOLDER_H

#include "json.hpp"
#include "Npc.h"
using json = nlohmann::json;

namespace CryptoPP {

}


namespace s {
	class NpcHolder {
	public:
		void init();
		int generateSpawnId();
		void freeSpawnId(int id);

		NpcHolder(const NpcHolder&) = delete;
		NpcHolder& operator=(const NpcHolder) = delete;

		static NpcHolder* instance() {
			static NpcHolder instance;
			return &instance;
		}

		Npc* createNpc(int type);

		void read_directory(std::string pattern, std::vector<std::string>& v) const;
	private:
		NpcHolder();
		~NpcHolder();

		std::map<int, Npc*> npcPrototypes;
		IDManager<sf::Int32> idManager;

		sf::Mutex lock;
	};
}


#endif
