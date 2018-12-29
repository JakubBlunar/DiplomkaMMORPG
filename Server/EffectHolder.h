#ifndef S_EFFECT_HOLDER_H
#define S_EFFECT_HOLDER_H

#include "json.hpp"
#include "Effect.h"
#include "IDManager.h"
#include <SFML/System.hpp>

using json = nlohmann::json;

namespace s {

	class EffectHolder {
	public:
		void init();
		int generateInstanceId();
		void freeInstanceId(int id);

		EffectHolder(const EffectHolder&) = delete;
		EffectHolder& operator=(const EffectHolder) = delete;

		static EffectHolder* instance() {
			static EffectHolder instance;
			return &instance;
		}

		Effect* createEffect(int type); 

		void read_directory(std::string pattern, std::vector<std::string>& v) const;
	private:
		EffectHolder();
		~EffectHolder();

		std::map<int, Effect*> prototypes;
		IDManager<sf::Int32> idManager;

		sf::Mutex lock;
	};
}

#endif
