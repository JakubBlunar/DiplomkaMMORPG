#ifndef S_SPELL_H
#define S_SPELL_H

#include "Entity.h"
#include "Effect.h"
#include "EntityAttributes.h"

namespace s {
	class Server;
	class SpellEvent;

	struct SpellInfo {
		std::string name;
		float manaCost;
		sf::Time cooldownTime;
		sf::Time castingTime;
		int id;
		sf::Time globalCooldownTime;
		float maxRange;
	};

	class Spell {
	protected:
		int instanceId;
		sf::Time lastCast;
		Entity* target;
		Entity* owner;
		std::list<Effect*> effects;
	public:
		Spell();
		virtual ~Spell();

		SpellInfo spellInfo;

		void addEffect(Effect* effect);

		virtual void cast(Entity* entity);

		Spell* clone() const;

	
		void setLastCast(sf::Time lastCast) {
			this->lastCast = lastCast;
		}

		void setInstanceId(int id) {
			this->instanceId = id;
		}
		int getInstanceId() const {
			return instanceId;
		}

		void loadFromJson(json data);
	};

}


#endif
