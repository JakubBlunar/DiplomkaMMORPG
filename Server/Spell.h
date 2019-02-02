#ifndef S_SPELL_H
#define S_SPELL_H

#include "Entity.h"
#include <list>
#include "json.hpp"
#include "SpellConstants.h"

using json = nlohmann::json;

namespace s {
	class Server;
	class SpellEvent;
	class Effect;

	struct SpellInfo {
		std::string name;
		float manaCost;
		sf::Time cooldownTime;
		sf::Time castingTime;
		int id;
		sf::Time globalCooldownTime;
		float maxRange;
		EntityType spellCategory;
		SpellType type;
		int levelNeeded;
	};

	class Spell {
	protected:
		int instanceId;
		Entity* target;
		Entity* owner;
		std::list<Effect*> effects;
	public:
		Spell();
		virtual ~Spell();

		void setServer(Server* server) {
			this->server = server;
		}

		SpellInfo spellInfo;
		Server* server;

		void addEffect(Effect* effect);

		virtual void cast(Entity* entity);

		virtual Spell* clone() const;

		void setInstanceId(int id) {
			this->instanceId = id;
		}
		int getInstanceId() const {
			return instanceId;
		}

		void setId(sf::Int32 id) {
			this->instanceId = id;
		}

		void setTarget(Entity* target) {
			this->target = target;
		}

		Entity* getTarget() {
			return target;
		}

		void setOwner(Entity* owner) {
			this->owner = owner;
		}

		Entity* getOwner() {
			return owner;
		};

		virtual void loadFromJson(json data);
	};

}


#endif
